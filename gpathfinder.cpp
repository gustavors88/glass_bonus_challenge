/*
 * File: gpathfinder.cpp
 * ---------------------
 * The gpathfinder.cpp file implements a set of functions that help draw the
 * necessary parts of the Pathfinder maps.
 */

#include "genlib.h"
#include "extgraph.h"
#include "gpathfinder.h"
#include "point.h"
#include "vector.h"

#include <iostream>

/* Constants */

const double WINDOW_WIDTH = 700;
const double WINDOW_HEIGHT = 400;
const double CONTROL_STRIP_HEIGHT = 40;

const string BUTTON_FONT = "Helvetica";
const int BUTTON_STYLE = Bold;
const int BUTTON_POINT_SIZE = 12;
const double MIN_BUTTON_WIDTH = 70;
const double BUTTON_HEIGHT = 24;
const double BUTTON_MARGIN = 6;
const double BUTTON_SEP = 6;

/* Structure for button data */

struct buttonT {
	string name;
	double x, y, width, height;
	bool highlighted;
	ButtonCallbackBase *callback;
};

/* Global data */

static Vector<buttonT> buttons;
ClickCallbackBase *clickHook = NULL;

/* Prototypes */

void DrawBox(double x, double y, double width, double height);
void FillBox(double x, double y, double width, double height);
void DrawButton(buttonT & button);
void DrawNormalButton(buttonT & button);
void DrawHighlightedButton(buttonT & button);
void DrawBackground(buttonT & button);
void DrawOutline(buttonT & button);
void DrawOuterShade(buttonT & button);
void DrawInnerShade(buttonT & button);
void DrawHighlight(buttonT & button);
void DrawButtonText(buttonT & button);
double ComputeNextButtonX();
double ComputeButtonWidth(string name);
int FindButtonIndex(double x, double y);
bool IsInsideButton(buttonT & button, double x, double y);

/* Exported functions */

/*
 * Implementation notes: InitPathfinderGraphics
 * --------------------------------------------
 * This function sets the size of the window and ensures that it operates
 * in screen coordinates, which makes it consistent with the Java graphics
 * library (and most other graphics libraries as well).  It also defines a
 * number of colors for the buttons and creates the control strip at the
 * bottom of the window.
 */

void InitPuzzleGraphics() {
	SetCoordinateSystem("screen");
	SetWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT + CONTROL_STRIP_HEIGHT);
	InitGraphics();
	DefineColor("CONTROL_STRIP", 0.90, 0.90, 0.90);
	DefineColor("STANDARD_BACKGROUND", 0.80, 0.80, 0.80);
	DefineColor("STANDARD_HIGHLIGHT", 1.00, 1.00, 1.00);
	DefineColor("STANDARD_INNERSHADE", 0.72, 0.72, 0.72);
	DefineColor("STANDARD_OUTERSHADE", 0.43, 0.43, 0.43);
	DefineColor("ACTIVATED_BACKGROUND", 0.00, 0.00, 1.00);
	DefineColor("ACTIVATED_BORDER", 0.00, 0.00, 0.90);
	DefineColor("ACTIVATED_INNERSHADE", 0.00, 0.00, 0.80);
	DefineColor("ACTIVATED_TEXT", 1.00, 1.00, 1.00);
	SetPenColor("CONTROL_STRIP");
	FillBox(0, WINDOW_HEIGHT, WINDOW_WIDTH, CONTROL_STRIP_HEIGHT);
	SetPenColor("Black");
	SetWindowTitle("Triangle Puzzle Solver");
}

/*
 * Implementation notes: DrawPathfinderMap, DrawPathfinderNode, DrawPathfinderArc
 * ------------------------------------------------------------------------------
 * These functions are quite straightforward as long as you understand the
 * extgraph.h interface.
 */

void DrawPathfinderMap(string mapFile) {
	SetPenColor("White");
	FillBox(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	SetPenColor("Black");
	if (mapFile != "") {
		MovePen(0, 0);
		DrawNamedPicture(mapFile);
	}
}

void DrawPathfinderNode(pointT center, string color, string label) {
	MovePen(center.x + NODE_RADIUS, center.y);
	SetPenColor(color);
	StartFilledRegion(1.0);
	DrawArc(NODE_RADIUS, 0, 360);
	EndFilledRegion();
	if (!label.empty()) {
		MovePen(center.x + NODE_RADIUS + 2, center.y + 0.4 * GetFontAscent());
		SetFont("Helvetica");
		SetPointSize(FONT_SIZE);
		DrawTextString(label);
	}
}

void DrawPathfinderArc(pointT start, pointT end, string color) {
	SetPenColor(color);
	MovePen(start.x, start.y);
	DrawLine(end.x - start.x, end.y - start.y);
}

/*
 * Implementation notes: UpdatePathfinderDisplay
 * ---------------------------------------------
 * This function simply forwards the UpdateDisplay request to the CS106
 * graphics library.  It is included in this package only to ensure that
 * clients don't need to import any functions from the extgraph.h interface.
 */

void UpdatePathfinderDisplay() {
	UpdateDisplay();
}

/*
 * Implementation notes: AddButton
 * -------------------------------
 * This function creates a button and adds it to the vector of buttons
 * at the next available position in the control strip.  Most of the
 * complexity involved in the AddButton function occurs in the
 * template version of the function, which appears in the file
 * gpathfinderimpl.cpp.
 */

void AddButton(string name, void (*actionFn)()) {
	AddButton(name, new ButtonCallbackBase(actionFn));
}

void AddButton(string name, ButtonCallbackBase *callback) {
	buttonT button;
	button.name = name;
	button.x = ComputeNextButtonX();
	button.y = WINDOW_HEIGHT + (CONTROL_STRIP_HEIGHT - BUTTON_HEIGHT) / 2;
	button.width = ComputeButtonWidth(name);
	button.height = BUTTON_HEIGHT;
	button.highlighted = false;
	button.callback = callback;
	buttons.add(button);
	DrawButton(button);
	UpdateDisplay();
}

void RemoveButton(string name) {
	for (int i = 0; i < buttons.size(); i++) {
		if (buttons[i].name == name) {
			SetPenColor("CONTROL_STRIP");
			FillBox(buttons[i].x - BUTTON_SEP, buttons[i].y - BUTTON_SEP, 
				buttons[i].width + 2*BUTTON_SEP, buttons[i].height + 2*BUTTON_SEP);
			buttons.removeAt(i);
			UpdateDisplay();
			return;
		}
	}
}

/*
 * Implementation notes: DefineClickListener
 * -----------------------------------------
 * This function designates a listener for mouse clicks in the window.
 * Most of the complexity involved in the DefineClickListener function
 * occurs in the template version of the function, which appears in the
 * file gpathfinderimpl.cpp.
 */

void DefineClickListener(void (*clickFn)(pointT pt)) {
	DefineClickListener(new ClickCallbackBase(clickFn));
}

void DefineClickListener(ClickCallbackBase *callback) {
	clickHook = callback;
}

/*
 * Implementation notes: PathfinderEventLoop
 * -----------------------------------------
 * Even though the code for this function is long, its operation is
 * reasonably straightforward.  As soons as the mouse goes down, this
 * function monitors its position, waiting for the mouse to enter or
 * leave the area covered by a button object.  Entering a button
 * highlights it on the screen; leaving the button region removes the
 * highlight.  When the mouse button comes up, the code checks to see
 * whether the release is in a button and, if so, invokes its action
 * function.  If not, the code checks to see if the client has defined
 * a click action, in which case it calls the click function.
 */

void PathfinderEventLoop() {
	while (true) {
		WaitForMouseDown();
		int downButtonIndex = FindButtonIndex(GetMouseX(), GetMouseY());
		if (downButtonIndex != -1) {
			buttons[downButtonIndex].highlighted = true;
			DrawButton(buttons[downButtonIndex]);
			UpdateDisplay();
		}
		bool isDown = true;
		while (isDown) {
			isDown = MouseButtonIsDown();
			int index = FindButtonIndex(GetMouseX(), GetMouseY());
			if (index != downButtonIndex) {
				if (downButtonIndex != -1) {
					buttons[downButtonIndex].highlighted = false;
					DrawButton(buttons[downButtonIndex]);
				}
				if (index != -1 && isDown) {
					buttons[index].highlighted = true;
					DrawButton(buttons[index]);
				}
				downButtonIndex = index;
				UpdateDisplay();
			}
		}
		if (downButtonIndex != -1) {
			buttons[downButtonIndex].highlighted = false;
			DrawButton(buttons[downButtonIndex]);
			UpdateDisplay();
			buttons[downButtonIndex].callback->apply();
		} else {
			pointT pt;
			pt.x = int(GetMouseX());
			pt.y = int(GetMouseY());
			if (pt.y < WINDOW_HEIGHT && clickHook != NULL) {
				clickHook->apply(pt);
			}
		}
	}
}

/*
 * Implementation notes: GetMouseClick
 * -----------------------------------
 * GetMouseClick waits for the mouse button to go down and then up again,
 * at which point the function returns the mouse position at the time of
 * release.
 */

pointT GetMouseClick() {
	WaitForMouseDown();
	WaitForMouseUp();
	pointT pt;
	pt.x = int(GetMouseX());
	pt.y = int(GetMouseY());
	return pt;
}

/* Helper functions */

/*
 * Implementation notes: DrawBox, FillBox
 * --------------------------------------
 * These functions are useful tools that draw an outlined and a filled
 * box, respectively.  If you are extending the Pathfinder assignment,
 * you might well want to export these methods through the gpathfinder.h
 * interface.
 */

void DrawBox(double x, double y, double width, double height) {
	MovePen(x, y);
	DrawLine(width, 0);
	DrawLine(0, height);
	DrawLine(-width, 0);
	DrawLine(0, -height);
}

void FillBox(double x, double y, double width, double height) {
	StartFilledRegion(1.0);
	DrawBox(x, y, width, height);
	EndFilledRegion();
}

/*
 * Implementation notes: DrawButton and its subsidiary functions
 * -------------------------------------------------------------
 * The following set of functions draw a button in the control strip.
 * The decomposition of the drawing operation ensures that the
 * complexity of the drawing code is manageable at every level.
 */

void DrawButton(buttonT & button) {
    SaveGraphicsState();
	if (button.highlighted) {
		DrawHighlightedButton(button);
	} else {
		DrawNormalButton(button);
    }
    RestoreGraphicsState();
}

void DrawNormalButton(buttonT & button) {
    SetPenColor("STANDARD_BACKGROUND");
    DrawBackground(button);
    SetPenColor("STANDARD_OUTERSHADE");
    DrawOuterShade(button);
    SetPenColor("STANDARD_INNERSHADE");
    DrawInnerShade(button);
    SetPenColor("STANDARD_HIGHLIGHT");
    DrawHighlight(button);
    SetPenColor("Black");
    DrawOutline(button);
    DrawButtonText(button);
}

void DrawHighlightedButton(buttonT & button) {
    SetPenColor("ACTIVATED_BACKGROUND");
    DrawBackground(button);
    SetPenColor("ACTIVATED_INNERSHADE");
    DrawInnerShade(button);
    SetPenColor("ACTIVATED_BORDER");
    DrawOutline(button);
    SetPenColor("ACTIVATED_TEXT");
    DrawButtonText(button);
}

void DrawBackground(buttonT & button) {
    FillBox(button.x + 1, button.y + 1, button.width - 2, button.height - 2);
}

void DrawOutline(buttonT & button) {
    MovePen(button.x, button.y + button.height - 2);
    DrawLine(0, 5 - button.height);
    DrawLine(2, -2);
    DrawLine(button.width - 5, 0);
    DrawLine(2, 2);
    DrawLine(0, button.height - 5);
    DrawLine(-2, 2);
    DrawLine(5 - button.width, 0);
    DrawLine(-2, -2);
}

void DrawOuterShade(buttonT & button) {
    MovePen(button.x + 2, button.y + 2);
    DrawLine(button.width - 5, 0);
    DrawLine(0, 1);
    DrawLine(1, 0);
    DrawLine(0, button.height - 5);
}

void DrawInnerShade(buttonT & button) {
    MovePen(button.x + 2, button.y + 3);
    DrawLine(button.width - 6, 0);
    DrawLine(0, 1);
    DrawLine(1, 0);
    DrawLine(0, button.height - 6);
}

void DrawHighlight(buttonT & button) {
    MovePen(button.x + 2, button.y + button.height - 2);
    DrawLine(0, 4 - button.height);
    MovePen(button.x + 2, button.y + button.height - 2);
    DrawLine(button.width - 4, 0);
    MovePen(button.x + 3, button.y + button.height - 3);
    DrawLine(0, 0);
}

void DrawButtonText(buttonT & button) {
    SetFont(BUTTON_FONT);
	SetStyle(BUTTON_STYLE);
    SetPointSize(BUTTON_POINT_SIZE);
    MovePen(button.x + (button.width - TextStringWidth(button.name)) / 2,
            button.y + (button.height + GetFontAscent()) / 2);
    DrawTextString(button.name);
}

/*
 * Implementation notes: ComputeNextButtonX, ComputeButtonWidth
 * ------------------------------------------------------------
 * These helper functions help arrange the buttons in the control
 * strip.  Each button is given a size so that its entire text
 * appears, with a suitable margin on each end.  The buttons are
 * assigned positions from left to right in the control strip
 * with BUTTON_SEP pixels between each one.
 */

double ComputeNextButtonX() {
	double x = BUTTON_SEP;
	for (int i = 0; i < buttons.size(); i++) {
		x += buttons[i].width + BUTTON_SEP;
	}
	return x;
}

double ComputeButtonWidth(string name) {
	SaveGraphicsState();
    SetFont(BUTTON_FONT);
	SetStyle(BUTTON_STYLE);
    SetPointSize(BUTTON_POINT_SIZE);
	double width = 2 * BUTTON_MARGIN + TextStringWidth(name);
	RestoreGraphicsState();
	if (width < MIN_BUTTON_WIDTH) width = MIN_BUTTON_WIDTH;
	return width;
}

/*
 * Implementation notes: FindButtonIndex, IsInsideButton
 * -----------------------------------------------------
 * These functions look through the vector of buttons to determine
 * whether the specified position is inside the area of some button.
 * If so, FindButtonIndex returns the index of that button in the
 * vector; if not, it returns -1.
 */

int FindButtonIndex(double x, double y) {
	for (int i = 0; i < buttons.size(); i++) {
		if (IsInsideButton(buttons[i], x, y)) return i;
	}
	return -1;
}

bool IsInsideButton(buttonT & button, double x, double y) {
	return x >= button.x && x <= button.x + button.width
	    && y >= button.y && y <= button.y + button.height;
}

/*
 * Implementation notes: Implementation of the callback classes
 * ------------------------------------------------------------
 * The remaining functions in this file support the implementation
 * of the various callback classes, which are defined in the file
 * gpathfinderimpl.cpp.
 */

ButtonCallbackBase::ButtonCallbackBase() {
	/* Empty */
}

ButtonCallbackBase::ButtonCallbackBase(void (*actionFn)()) {
	this->actionFn = actionFn;
}

ButtonCallbackBase::~ButtonCallbackBase() {
	/* Empty */
}

void ButtonCallbackBase::apply() {
	actionFn();
}

ClickCallbackBase::ClickCallbackBase() {
	/* Empty */
}

ClickCallbackBase::ClickCallbackBase(void (*clickFn)(pointT pt)) {
	this->clickFn = clickFn;
}

ClickCallbackBase::~ClickCallbackBase() {
	/* Empty */
}

void ClickCallbackBase::apply(pointT pt) {
	clickFn(pt);
}

