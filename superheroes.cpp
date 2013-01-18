/* 
 * File: superheroes.cpp
 * -----------------------
 * Author: Joshua Grinberg
 * Date: 12-23-2011
 * -----------------------
 * This program determines how to reassemble a glass window broken into triangular shards by
 * dropping them in from above, one at a time. The user has the option of loading a puzzle
 * from a saved file, or creating his own by drawing triangles on a grid. Once a puzzle is
 * loaded or created, the user can then watch an animation of the triangles being dropped 
 * from above in the correct order.
 *
 * To solve the puzzle, the program first looks through all the triangles to find the
 * "starting triangles", or the ones that lie on the bottom side of the rectangular border
 * and that are concave, meaning that if they are inserted now, they will not block any 
 * triangles later on. (It will be described later how to determine if a triangle will block
 * another triangle later on.)
 * 
 * Below is an example of two valid starting triangles. They both lie on the bottom face and
 * will not block any triangles later on.
 *       ______________________
 *      |                      |
 *      |                      |
 *      |                /\    |
 *      |    /\         /  \   |
 *      |___/__\_______/____\__| FIGURE 1
 *
 *       
 * Below is an example of two invalid starting triangles. The first does not lie on the bottom
 * face and the second will block a triangle later on (the one that touches its ride side).
 *       ______________________
 *      |                 _//  |
 *      |   /\          _/ /   |
 *      |  /__\       _/  /    |
 *      |           _/   / X   |
 *      |__________/___ /______| FIGURE 2
 *
 *
 * The program then stores a list of all the exposed, upward-facing sides of these starting 
 * triangles. The lines in this list are called boundary lines because they describe the 
 * boundary of all the triangles inserted so far. 
 * 
 * There are four boundary lines in the example below (lines touching the border are not
 * included).
 *       ______________________
 *      |                      |
 *      |                      |
 *      |                /\    |
 *      |  1 /\ 2     3 /  \ 4 |
 *      |___/__\_______/____\__| FIGURE 3
 *
 * 
 * Next, the program recursively searches through all the unused triangles until it finds one with 
 * a side that is also a boundary line. This triangle is inserted next if it will not block a
 * triangle later on. (It will be described later how to determine if a triangle will block
 * another triangle later on.) The boundary lines list is updated so that the contact line is
 * removed (since it is no longer exposed), and any new, exposed, upward-facing sides of the newly
 * inserted triangle are added to the boundary lines list. 
 *
 * In the example below, first there are three boundary lines. Once the triangle is inserted, boundary
 * line #1 is removed since it is the line of contact, and boundary line #4 is added since it is
 * exposed and upward-facing.
 * 
 *       ______________________                          ______________________
 *      |                      |                        |                      |
 *      |                      |                        |                      |
 *      |                      |         _______        |_4_____               |
 *      |     _/\             /|        |     _/        |     _/\             /|
 *      | 1 _/   \ 2       3 / |   +    |   _/     =    |   _/   \ 2       3 / |
 *      | _/      \         /  |        | _/            | _/      \         /  |
 *      |/_________\_______/___|        |/              |/_________\_______/___| FIGURE 4
 * 
 *
 * Note that the list of boundary lines will always describe the exposed boundary of all the 
 * triangles inserted so far, as depicted in the example below.
 *       ______________________                          ______________________
 *      |                      |                        |                      |
 *      |                      |                        |                      |
 *      |_4_____               |                        |_4_____               |
 *      |     _/\             /|        \\__            |     _/\\__          /|
 *      |   _/   \ 2       3 / |   +     \  \__      =  |   _/   \  \__5   3 / |
 *      | _/      \         /  |          \    \__      | _/      \    \__  /  |
 *      |/_________\_______/___|           \______\     |/_________\______\/___| FIGURE 5
 *
 *
 * This recursive process is repeated until the list of boundary lines is reduced to a size of zero,
 * at which point the puzzle is solved. There are no boundary lines in the example below (recal that
 * lines on the rectangular border are not considered boundary lines).
 *       ______________________
 *      |  /\       __/ \__    |
 *      | /  \   __/       \__ |
 *      |/____\_/_____________\|
 *      |      /\\__          /|
 *      |    /   \  \__      / |
 *      |  /      \    \__  /  |
 *      |/_________\______\/___| FIGURE 6
 *
 *
 * It was mentioned earlier that a new triangle is inserted only if one (or more) of its sides 
 * is a boundary line, and if inserting the triangle now will not block any triangles later on. 
 * How is it determined if inserting the triangle now will not block any triangles later on? Look 
 * at the point of the triangle that is not an endpoint of the contact line. In the examples
 * below, the contact lines are between points A and B, and the third points are points C.
 *       ___________________________                     
 *      |                          |                      
 *      |                          |                        
 *      |_______ A                 |    A _______ C    
 *      |      /\\__               |      \__    \
 *      |    /   \  \__            |   +     \__  \
 *      |  /      \    \__         |            \__\
 *      |/_________\______\_B______|               \\ B        FIGURE 7
 *
 *       __________________________                     
 *      |                          |                      
 *      |                          |                        
 *      |_______ A                 |    A ______________ C    
 *      |      /\\__               |      \__          /    
 *      |    /   \  \__            |   +     \__      /   
 *      |  /      \    \__         |            \__  /     
 *      |/_________\______\_B______|               \/ B        FIGURE 8
 *
 * 
 * Is the x-coordinate of this third point (C) between the x-coordinates of the contact line endpoints 
 * (A and B)? In FIGURE 7, C is between A and B. This means that the triangle is safe to add now. 
 * In FIGURE 8, C is not between A and B. This means further tests need to be conducted to determine 
 * if the triangle is safe to add now, because this triangle might block another triangle with a side 
 * between B and C.
 *       __________________________                           __________________________
 *      |                          |                         |                          |
 *      |                          |                         |                          |
 *      |_______                   |      ______________ C   |______________________    |
 *      |      /\\__               |      \__          /     |      /\\__          /    |
 *      |    /   \  \__            |   +     \__      /   =  |    /   \  \__      /     |
 *      |  /      \    \__         |            \__  /       |  /      \    \__  / X    |
 *      |/_________\______\________|               \/ B      |/_________\______\/_______| FIGURE 9
 *
 *
 * A triangle with an over-extending third point (C) will not bock any triangles later on
 * if the line between the third point (C) and the lower contact line endpoint (B) is
 * already a boundary line (since this means the triangle which would have been blocked has
 * already been added). In the example below, the line between C and B is already a boundary 
 * line (4), so even though the x-coordinate of C is not between the x-coordinates of A and B, 
 * it is safe to add the triangle now.
 *       __________________________                           __________________________
 *      |                          |                         |                          |
 *      |                          |                         |                          |
 *      |___1___                   |    A ______________ C   |___1___________6______    |
 *      |      /\\__          /\   |      \__          /     |      /\\__          /\   |
 *      |    /   \  \__2   4 /  \ 5|   +     \__2     /   =  |    /   \  \__      /  \ 5|
 *      |  /      \    \__  /    \ |            \__  /       |  /      \    \__  /    \ |
 *      |/_________\______\/______\|               \/ B      |/_________\______\/______\| FIGURE 10
 * 
 *
 * That's it! This algorithm will work for any combination of triangles, no matter how complicated.
 * Try the pre-loaded puzzles, or create your own! 
 * 
 */

#include "genlib.h"
#include "gpathfinder.h"
#include <iostream>
#include "vector.h"
#include "scanner.h"
#include "simpio.h" 
#include "extgraph.h"
#include "graphics.h"



/*****************/
/*** CONSTANTS ***/
/*****************/

const double WINDOW_WIDTH = 700;
const double WINDOW_HEIGHT = 400;
const double CONTROL_STRIP_HEIGHT = 40;
const int CREATE_PUZZLE_X_MARGIN = 50;
const int CREATE_PUZZLE_Y_MARGIN = 50;
const int CREATE_PUZZLE_STEP = 30;



/******************/
/*** STRUCTURES ***/
/******************/
 
/*
 * lineT represents the line between two endpoints.
 */
struct lineT {
	int x1, x2, y1, y2;
};

/*
 * triangleT represents a numbered triangle defined by three lineT's.
 */
struct triangleT {
	Vector<lineT> sides;
	int num;
};

/*
 * puzzeBorderT represents the min/max coordinates of the puzzle's rectangular border.
 */
struct puzzleBorderT {
	int minX, minY, maxX, maxY;
};



/*****************************/
/*** FUNCTION DECLARATIONS ***/
/**************** *************/

void welcomeTitle();
void quitAction(Vector<triangleT> & triangles);
void loadSavedPuzzleAction(Vector<triangleT> & triangles);
void createPuzzleModeAction(Vector<triangleT> & triangles);
void analyzeModeAction(Vector<triangleT> & triangles);
void drawTriangleAction(Vector<triangleT> & triangles);
void clearAction(Vector<triangleT> & triangles);
void saveAction(Vector<triangleT> & triangles);
void solveAction(Vector<triangleT> & triangles);
void solveStepByStepAction(Vector<triangleT> & triangles);
void drawPuzzle(Vector<triangleT> & triangles, double pause, string highlightColor, string fillColor, string outlineColor);
void fillBackground(string color);
void drawGrid();
pointT getGridClick(int xMargin, int yMargin, int step);
void animateFallingTriangle(triangleT & triangle, double scaleFactor, double xMargin, double yMargin, double height, double density = 1.0);
void drawFilledTriangle(triangleT & triangle, double scaleFactor, double xMargin, double yMargin, double height, double density = 1.0);
void drawFilledTriangle(triangleT & triangle, puzzleBorderT & puzzleBorder);
void drawFilledTriangle(triangleT & triangle);
void drawTriangle(triangleT & triangle, double scaleFactor, double xMargin, double yMargin, double height);
void drawTriangle(triangleT & triangle, puzzleBorderT & puzzleBorder);
void drawTriangle(triangleT & triangle);
void drawLine(lineT & line, double scaleFactor, double xMargin, double yMargin, double height);
void drawPoint(pointT point, string color);
void drawTriangleNum(Vector<triangleT> triangles, int triangleCounter);
void saveFile(ofstream & outfile, Vector<triangleT> & triangles);
void outfilePutInt(ofstream & outfile, int num);
void readFile(ifstream & infile, Vector<triangleT> & triangles);
string getInputFileName(ifstream & infile);
string getOutputFileName(ofstream & outfile);
triangleT createTriangle(Vector<pointT> points);
lineT createLine(pointT a, pointT b);
Vector<triangleT> solve(Vector<triangleT> triangles);
void solveRec(Vector<triangleT> & trianglesLeft, Vector<triangleT> & trianglesSoFar, Vector<lineT> & boundaryLines, puzzleBorderT & puzzleBorder);
puzzleBorderT findPuzzleBorder(Vector<triangleT> triangles);
void findStartingTriangles(Vector<triangleT> & triangles, Vector<triangleT> & trianglesSoFar, Vector<lineT> & boundaryLines, puzzleBorderT puzzleBorder);
bool compareLines(lineT a, lineT b);
bool isOnBorder(lineT possibleBorder, puzzleBorderT puzzleBorder);
bool isConcave(triangleT triangle, lineT baseLine);
bool isConcave(triangleT triangle, lineT contactLine, lineT & obstructedLine);
bool doesNotBlock(Vector<lineT> & boundaryLines, triangleT triangle, lineT line, lineT & obstructedLine);
void defineObstructedLine(triangleT triangle, lineT & obstructedLine);
bool obstructedLineIsBoundaryLine(Vector<lineT> & boundaryLines, lineT & obstructedLine);
void removeDuplicateBoundaryLines(Vector<lineT> & boundaryLines);
string trianglesToString(Vector<triangleT> triangles);
void printTriangle(triangleT triangle);
void printLine(lineT line, int num);
void printSolution(Vector<triangleT> solution);



/*****************/
/*** FUNCTIONS ***/
/*****************/

int main() {
	InitPuzzleGraphics();
	Vector<triangleT> triangles;
	welcomeTitle();
    AddButton("Quit", quitAction, triangles);
	AddButton("Load Saved Puzzle", loadSavedPuzzleAction, triangles);
    AddButton("Create Puzzle Mode", createPuzzleModeAction, triangles);
	AddButton("Analyze Mode", analyzeModeAction, triangles);
	PathfinderEventLoop();
	return 0;
}

/*
 * Displays a welcome title when the program is launched. 
 */
void welcomeTitle() {
	SetPointSize(24);
	SetPenColor("black");
	SetFont("Helvetica");
	SetStyle(1);
	MovePen((WINDOW_WIDTH - TextStringWidth("Welcome to Triangle Puzzle Solver!")) / 2, WINDOW_HEIGHT / 3);
	DrawTextString("Welcome to Triangle Puzzle Solver!");
	cout << "Prepare to be amazed as the Triangle Puzzle Solver figures out how to piece" << endl
		<< "together a glass window broken into triangular shards by dropping them in" << endl
		<< "from above.\n" << endl
		<< "Click \"LOAD SAVED PUZZLE\" to open a saved file." << endl
		<< "Click \"CREATE PUZZLE MODE\" to draw your own puzzle." << endl
		<< "Click \"ANALYZE MODE\" once you've loaded or created a puzzle to solve it." << endl;
}

/*********************/
/* Main Menu Buttons */
/*********************/

/*
 * Quits the program.
 */
void quitAction(Vector<triangleT> & triangles) {
	cout << "Thanks for playing!" << endl;
	exit(0);
}

/*
 * Loads a saved puzzle from the specified file name. If the file name "demo" is given, automatically opens the
 * file "shattered-glass.in.txt".
 */
void loadSavedPuzzleAction(Vector<triangleT> & triangles) {
	fillBackground("white");
	RemoveButton("Analyze Mode");
	RemoveButton("Solve");
	RemoveButton("Solve Step-by-Step");
	RemoveButton("Create Puzzle Mode");
	RemoveButton("Draw Triangle");
	RemoveButton("Clear");
	RemoveButton("Save");
	AddButton("Create Puzzle Mode", createPuzzleModeAction, triangles);
	AddButton("Analyze Mode", analyzeModeAction, triangles);
	triangles.clear();
	ifstream infile;
	string fileName = getInputFileName(infile);
	if (fileName.empty()) return;
    readFile(infile, triangles);
	drawPuzzle(triangles, -1, "green", "blue", "black");
}

/*
 * Loads the "Create Puzzle Mode", which enables the user to draw a new puzzle by creating triangles one at a time.
 */
void createPuzzleModeAction(Vector<triangleT> & triangles) {
	fillBackground("white");
	RemoveButton("Create Puzzle Mode");
	RemoveButton("Analyze Mode");
	RemoveButton("Solve");
	RemoveButton("Solve Step-by-Step");
	AddButton("Analyze Mode", analyzeModeAction, triangles);
	AddButton("Draw Triangle", drawTriangleAction, triangles);
	AddButton("Clear", clearAction, triangles);
	AddButton("Save", saveAction, triangles);
	triangles.clear();
	fillBackground("white");
	drawGrid();
	cout << "\nHere's how to create your own puzzle!" << endl
		<< "--> Draw triangles one at a time anywhere in the grid above." << endl
		<< "--> Repeat until all the triangles put together completely define a rectangle." << endl
		<< "--> Make sure there are no empty spaces, because if there are, the puzzle will" << endl
		<< "    not be solvable." << endl
		<< "--> There cannot be any overlaps, so if two triangles intersect, click \"CLEAR\"" << endl
		<< "    to restart." << endl
		<< "--> Once you finish, you can save your puzzle for later." << endl;
}

/*
 * Loads the "Analyze Mode", which enables the user to view an animation of the puzzle solution and analyze the results.
 */
void analyzeModeAction(Vector<triangleT> & triangles) {
	if (triangles.isEmpty()) return;
	RemoveButton("Create Puzzle Mode");
	RemoveButton("Analyze Mode");
	RemoveButton("Draw Triangle");
	RemoveButton("Clear");
	RemoveButton("Save");
	AddButton("Create Puzzle Mode", createPuzzleModeAction, triangles);
	AddButton("Solve", solveAction, triangles);
	AddButton("Solve Step-by-Step", solveStepByStepAction, triangles);
	cout << "\nClick \"SOLVE\" to watch an animation of the puzzle solution." << endl
		<< "Click \"SOLVE STEP-BY-STEP\" to walk through the solution at your own pace," << endl
		<< "   separating each step with a click." << endl;
}

/******************************/
/* Create Puzzle Mode Buttons */
/******************************/

/* 
 * Enables the user to define a triangle by clicking three points on a grid.
 */
void drawTriangleAction(Vector<triangleT> & triangles) {
	cout << "\nClick three points in the grid to define a triangle's vertices." << endl;
	Vector<pointT> points;
	for (int vertex = 0; vertex < 3; vertex++) {
		points.add(getGridClick(50, 50, 30));
		drawPoint(points[vertex], "green");
	}
	triangleT newTriangle = createTriangle(points);
	newTriangle.num = triangles.size();
	triangles.add(newTriangle);
	SetPenColor("green");
	drawFilledTriangle(newTriangle);
	Pause(0.5);
	SetPenColor("blue");
	drawFilledTriangle(newTriangle);
	SetPenColor("black");
	drawTriangle(newTriangle);
	for (int vertex = 0; vertex < 3; vertex++)
		drawPoint(points[vertex], "black");
}

/*
 * Clears the screen and the saved puzzle-in-progress in case the user makes a mistake.
 */
void clearAction(Vector<triangleT> & triangles) {
	fillBackground("white");
	drawGrid();
	triangles.clear();
}

/*
 * Saves the current state of the created puzzle into a file. If the user inputs the name of an already
 * existing file, the puzzle is saved in that file. If the user inputs the name of a nonexistant file,
 * a new file of that name is created.
 */
void saveAction(Vector<triangleT> & triangles) {
	ofstream outfile;
	string fileName = getOutputFileName(outfile);
	if (fileName.empty()) return;
	saveFile(outfile, triangles);
	cout << "\nSaved!" << endl;
}

/************************/
/* Analyze Mode Buttons */
/************************/

/*
 * Animates a solution to the puzzle.
 */
void solveAction(Vector<triangleT> & triangles) {
	if (triangles.isEmpty()) return;
	fillBackground("white");
	Vector<lineT> boundaryLines;
	Vector<triangleT> solution = solve(triangles);
	printSolution(solution);
	drawPuzzle(solution, 0.4, "green", "blue", "black");
}

/*
 * Enables the user to walk through the solution step-by-step, separated by clicks.
 */
void solveStepByStepAction(Vector<triangleT> & triangles) {
	if (triangles.isEmpty()) return;
	fillBackground("white");
	Vector<lineT> boundaryLines;
	Vector<triangleT> solution = solve(triangles);
	printSolution(solution);
	drawPuzzle(solution, 0, "green", "blue", "black");
}



/************************/
/*** HELPER FUNCTIONS ***/
/************************/

/************/
/* Graphics */
/************/

/*
 * Fills the background with the specified color.
 */
void fillBackground(string color) {
	SetPenColor(color);
	FillBox(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT - CONTROL_STRIP_HEIGHT);
}

/*
 * Draws the puzzle border and triangles. The puzzle is centered in the middle of the screen and scaled
 * by a scaleFactor, calculated such that the minimum margin on any side is 50 pixels. A pause value of 
 * -1 skips an animation of the triangles falling into place, and a pause value of 0 separates the
 * animation of each triangle with a mouse click.
 */
void drawPuzzle(Vector<triangleT> & triangles, double pause, string highlightColor, string fillColor, string outlineColor) {
	fillBackground("white");
	puzzleBorderT puzzleBorder = findPuzzleBorder(triangles);
	int rawWidth = puzzleBorder.maxX - puzzleBorder.minX;
	int rawHeight = puzzleBorder.maxY - puzzleBorder.minY;
	double xScaleFactor = (WINDOW_WIDTH - 100)/rawWidth;
	double yScaleFactor = (WINDOW_HEIGHT - 100)/rawHeight;
	double scaleFactor = min(xScaleFactor, yScaleFactor);
	double width = scaleFactor*rawWidth;
	double height = scaleFactor*rawHeight;
	double xMargin = (WINDOW_WIDTH-width)/2;
	double yMargin = (WINDOW_HEIGHT-height)/2;
	for (int triangleCounter = 0; triangleCounter < triangles.size(); triangleCounter++) {
		if (pause != -1) {
			drawTriangleNum(triangles, triangleCounter);
			SetPenColor(highlightColor);	
			animateFallingTriangle(triangles[triangleCounter], scaleFactor, xMargin - puzzleBorder.minX*scaleFactor, 
				yMargin + puzzleBorder.minY*scaleFactor, height);
			Pause(pause);
			if (pause == 0) GetMouseClick();
		}
		SetPenColor(fillColor);
		drawFilledTriangle(triangles[triangleCounter], scaleFactor, xMargin - puzzleBorder.minX*scaleFactor, 
			yMargin + puzzleBorder.minY*scaleFactor, height);
		SetPenColor(outlineColor);
		drawTriangle(triangles[triangleCounter], scaleFactor, xMargin - puzzleBorder.minX*scaleFactor, 
			yMargin + puzzleBorder.minY*scaleFactor, height);

	}
	SetPenColor("white");
	FillBox(0, WINDOW_HEIGHT - CONTROL_STRIP_HEIGHT, WINDOW_WIDTH, 30);
}

/*
 * Animates a triangle falling from the top of the screen.
 */
void animateFallingTriangle(triangleT & triangle, double scaleFactor, double xMargin, double yMargin, double height, double density) {
	for (int y = -WINDOW_HEIGHT; y <= yMargin; y++) {
		drawFilledTriangle(triangle, scaleFactor, xMargin, y, height, density);
		Pause(0.00025);
		if (y != yMargin) {
			SetEraseMode(true);
			drawFilledTriangle(triangle, scaleFactor, xMargin, y, height, density);
			SetEraseMode(false);
		}
	}
}

/*
 * Draws a filled triangle with the specified scaleFactor, which adjusts for the size of the graphics window.
 */
void drawFilledTriangle(triangleT & triangle, double scaleFactor, double xMargin, double yMargin, double height, double density) {
	if (density != 0) StartFilledRegion(density);
	MovePen(triangle.sides[0].x1*scaleFactor + xMargin, height + yMargin - triangle.sides[0].y1*scaleFactor);
	for (int side = 0; side < 3; side++) {
		lineT line = triangle.sides[side];
		DrawLine((line.x2 - line.x1)*scaleFactor, -(line.y2 - line.y1)*scaleFactor);
	}
	if (density != 0) EndFilledRegion();
}

/*
 * Draws a filled triangle with a scaleFactor calculated from a given puzzleBorder.
 */
void drawFilledTriangle(triangleT & triangle, puzzleBorderT & puzzleBorder) {
	int rawWidth = puzzleBorder.maxX - puzzleBorder.minX;
	int rawHeight = puzzleBorder.maxY - puzzleBorder.minY;
	double xScaleFactor = (WINDOW_WIDTH - 100)/rawWidth;
	double yScaleFactor = (WINDOW_HEIGHT - 100)/rawHeight;
	double scaleFactor = min(xScaleFactor, yScaleFactor);
	double width = scaleFactor*rawWidth;
	double height = scaleFactor*rawHeight;
	double xMargin = (WINDOW_WIDTH-width)/2;
	double yMargin = (WINDOW_HEIGHT-height)/2;
	drawFilledTriangle(triangle, scaleFactor, xMargin - puzzleBorder.minX*scaleFactor, 
			yMargin + puzzleBorder.minY*scaleFactor, height);
}

/*
 * Draws a filled triangle whose scaleFactor is calculated by the the default puzzleBorder used when creating 
 * a new puzzle.
 */
void drawFilledTriangle(triangleT & triangle) {
	puzzleBorderT puzzleBorder;
	puzzleBorder.minX = 0;
	puzzleBorder.minY = 0;
	puzzleBorder.maxX = (WINDOW_WIDTH - 2*CREATE_PUZZLE_X_MARGIN) / CREATE_PUZZLE_STEP;
	puzzleBorder.maxY = (WINDOW_HEIGHT - 2*CREATE_PUZZLE_Y_MARGIN) / CREATE_PUZZLE_STEP;
	drawFilledTriangle(triangle, puzzleBorder);
}

/*
 * Draws the outline of a triangle with the specified scaleFactor, which adjusts for the size of the graphics window.
 */
void drawTriangle(triangleT & triangle, double scaleFactor, double xMargin, double yMargin, double height) {
	drawFilledTriangle(triangle, scaleFactor, xMargin, yMargin, height, 0);
}

/*
 * Draws the outline triangle with a scaleFactor calculated from a given puzzleBorder.
 */
void drawTriangle(triangleT & triangle, puzzleBorderT & puzzleBorder) {
	int rawWidth = puzzleBorder.maxX - puzzleBorder.minX;
	int rawHeight = puzzleBorder.maxY - puzzleBorder.minY;
	double xScaleFactor = (WINDOW_WIDTH - 100)/rawWidth;
	double yScaleFactor = (WINDOW_HEIGHT - 100)/rawHeight;
	double scaleFactor = min(xScaleFactor, yScaleFactor);
	double width = scaleFactor*rawWidth;
	double height = scaleFactor*rawHeight;
	double xMargin = (WINDOW_WIDTH-width)/2;
	double yMargin = (WINDOW_HEIGHT-height)/2;
	drawTriangle(triangle, scaleFactor, xMargin - puzzleBorder.minX*scaleFactor, 
			yMargin + puzzleBorder.minY*scaleFactor, height);
}

/*
 * Draws the outline of a triangle whose scaleFactor is calculated by the default puzzleBorder used when creating 
 * a new puzzle.
 */
void drawTriangle(triangleT & triangle) {
	puzzleBorderT puzzleBorder;
	puzzleBorder.minX = 0;
	puzzleBorder.minY = 0;
	puzzleBorder.maxX = (WINDOW_WIDTH - 2*CREATE_PUZZLE_X_MARGIN) / CREATE_PUZZLE_STEP;
	puzzleBorder.maxY = (WINDOW_HEIGHT - 2*CREATE_PUZZLE_Y_MARGIN) / CREATE_PUZZLE_STEP;
	drawTriangle(triangle, puzzleBorder);
}

/*
 * Draws a line with the specified scaleFactor, which adjusts for the size of the graphics window.
 */
void drawLine(lineT & line, double scaleFactor, double xMargin, double yMargin, double height) {
	MovePen(line.x1*scaleFactor + xMargin, height + yMargin - line.y1*scaleFactor);
	DrawLine((line.x2 - line.x1)*scaleFactor, -(line.y2 - line.y1)*scaleFactor);
}

/*
 * Draws a point with the default scaleFactor when creating a new puzzle.
 */
void drawPoint(pointT point, string color) {
	pointT scaledPoint;
	scaledPoint.x = point.x * CREATE_PUZZLE_STEP + CREATE_PUZZLE_X_MARGIN;
	scaledPoint.y = WINDOW_HEIGHT - (point.y * CREATE_PUZZLE_STEP + CREATE_PUZZLE_Y_MARGIN);
	DrawPathfinderNode(scaledPoint, color);
}

/*
 * Draws the number of the triangle currently being animated centered at the bottom of the screen.
 */
void drawTriangleNum(Vector<triangleT> triangles, int triangleCounter) {
	string solutionSoFar = "";
	for (int i = 0; i < triangleCounter; i++) solutionSoFar += (IntegerToString(triangles[i].num) + "  ");
	string solution = solutionSoFar;
	for (int i = triangleCounter; i < triangles.size(); i++) solution += (IntegerToString(triangles[i].num) + "  ");
	solution = solution.substr(0, solution.length() - 2);
	SetPointSize(20);
	SetPenColor("black");
	SetFont("Helvetica");
	SetStyle(1);
	MovePen((WINDOW_WIDTH - TextStringWidth(solution))/2 + TextStringWidth(solutionSoFar), WINDOW_HEIGHT - CONTROL_STRIP_HEIGHT + 30);
	DrawTextString(IntegerToString(triangles[triangleCounter].num));
}

/*******************************/
/* Create New Puzzle Functions */
/*******************************/

/*
 * Draws the grid on which the user will create a new puzzle.
 */
void drawGrid() {
	SetPenColor("gray");
	int width = (WINDOW_WIDTH - 2*CREATE_PUZZLE_X_MARGIN)/CREATE_PUZZLE_STEP*CREATE_PUZZLE_STEP;
	int height = (WINDOW_HEIGHT - 2*CREATE_PUZZLE_Y_MARGIN)/CREATE_PUZZLE_STEP*CREATE_PUZZLE_STEP;
	for (int row = CREATE_PUZZLE_Y_MARGIN; row <= WINDOW_HEIGHT - CREATE_PUZZLE_Y_MARGIN; row+=CREATE_PUZZLE_STEP) {
		MovePen(CREATE_PUZZLE_X_MARGIN, row);
		DrawLine(width, 0);
	}
	for (int col = CREATE_PUZZLE_X_MARGIN; col <= WINDOW_WIDTH - CREATE_PUZZLE_X_MARGIN; col+=CREATE_PUZZLE_STEP) {
		MovePen(col, CREATE_PUZZLE_Y_MARGIN);
		DrawLine(0, height);
	}
}

/*
 * Returns the cartesian grid coordinates of a user's click. 
 */
pointT getGridClick(int xMargin, int yMargin, int step) {
	while (true) {
		pointT click = GetMouseClick();
		int xCoord = (click.x - xMargin + step/2) / step;
		int yCoord = (WINDOW_HEIGHT - click.y - yMargin + step/2) / step;
		int xError = abs(click.x - xMargin - xCoord*step);
		int yError = abs(WINDOW_HEIGHT - click.y - yMargin - yCoord*step);
		if (click.x > (xMargin - (step/3)) && click.x < (WINDOW_WIDTH - xMargin + (step/3)) &&
			click.y > (yMargin - (step/3)) && click.y < (WINDOW_HEIGHT - yMargin + (step/3)) &&
			xError <= step/3 && yError <= step/3) {
			click.x = xCoord;
			click.y = yCoord;
			return click;
		}
	}
}

/*
 * Returns the name of an output file into which the puzzle will be saved.
 */
string getOutputFileName(ofstream & outfile) {
    cout << "\nType the name of file in which to save this puzzle: ";
	while (true) {
        string answer = GetLine();
		outfile.open(answer.c_str());
        if (!outfile.fail() || answer.empty()) return answer;
        outfile.clear();
        cout << "Invalid file! Try again, please: ";
    }
}

/*
 * Writes the necessary information of the created puzzle to a given file. First, writes the number
 * of triangles in the puzzle on the first line, then writes the coordinates of each triangle in the
 * puzzle on the following lines. Coordinates are separated by spaces and triangles are separated by
 * new lines.
 */
void saveFile(ofstream & outfile, Vector<triangleT> & triangles) {
	outfilePutInt(outfile, triangles.size());
	outfile.put('\n');
	for (int triangle = 0; triangle < triangles.size(); triangle++) {
		outfilePutInt(outfile, triangles[triangle].sides[0].x1);
		outfile.put(' ');
		outfilePutInt(outfile, triangles[triangle].sides[0].y1);
		outfile.put(' ');
		outfilePutInt(outfile, triangles[triangle].sides[0].x2);
		outfile.put(' ');
		outfilePutInt(outfile, triangles[triangle].sides[0].y2);
		outfile.put(' ');
		outfilePutInt(outfile, triangles[triangle].sides[1].x2);
		outfile.put(' ');
		outfilePutInt(outfile, triangles[triangle].sides[1].y2);
		outfile.put('\n');
	}
}

/*
 * Writes an int to a given ofstream.
 */
void outfilePutInt(ofstream & outfile, int num) {
	string numString = IntegerToString(num);
	for (int digit = 0; digit < numString.size(); digit++)
		outfile.put(numString[digit]);
}

/*******************************/
/* Load Saved Puzzle Functions */
/*******************************/

/*
 * Loads the contents of a specified file into a vector called triangles.
 */
void readFile(ifstream & infile, Vector<triangleT> & triangles) {
	string nextLine;
	getline(infile, nextLine);
	if (nextLine == "")	getline(infile, nextLine);
	int numTriangles = StringToInteger(nextLine);
	Scanner scanner;
	scanner.setSpaceOption(Scanner::IgnoreSpaces);
	for (int t = 0; t < numTriangles; t++) {
		getline(infile, nextLine);
		scanner.setInput(nextLine);
		Vector<pointT> points;
		for (int pointCounter = 0; pointCounter < 3; pointCounter++) {
			pointT point;
			point.x = StringToInteger(scanner.nextToken());
			point.y = StringToInteger(scanner.nextToken());
			points.add(point);
		}
		triangleT newTriangle = createTriangle(points);
		newTriangle.num = t;
		triangles.add(newTriangle);
	}
}

/*
 * Prompts the user for a file name. 
 */
string getInputFileName(ifstream & infile) {
    cout << "\nType the name of a file to load its saved puzzle: ";
	while (true) {
        string answer = GetLine();
        if (answer == "demo") answer = "shattered-glass.in.txt";
		infile.open(answer.c_str());
        if (!infile.fail() || answer.empty()) return answer;
        infile.clear();
        cout << "Invalid file! Try again, please: ";
    }
}

/*
 * Returns a new triangle defined by a Vector of three points.
 */
triangleT createTriangle(Vector<pointT> points) {
	triangleT newTriangle;
	newTriangle.sides.add(createLine(points[0], points[1]));
	newTriangle.sides.add(createLine(points[1], points[2]));
	newTriangle.sides.add(createLine(points[2], points[0]));
	return newTriangle;
}

/*
 * Returns a new line defined by two points.
 */
lineT createLine(pointT a, pointT b) {
	lineT newLine;
	newLine.x1 = a.x;
	newLine.y1 = a.y;
	newLine.x2 = b.x;
	newLine.y2 = b.y;
	return newLine;
}

/**************************/
/* Analyze Mode Functions */
/**************************/

/*
 * Solves the given puzzle.
 */
Vector<triangleT> solve(Vector<triangleT> triangles) {
	puzzleBorderT puzzleBorder = findPuzzleBorder(triangles);
	Vector<triangleT> trianglesSoFar;
	Vector<lineT> boundaryLines;
	findStartingTriangles(triangles, trianglesSoFar, boundaryLines, puzzleBorder);
	solveRec(triangles, trianglesSoFar, boundaryLines, puzzleBorder);
	return trianglesSoFar;
}

/*
 * Recursive implementation of the solving algorithm. See the comments at the beginning of this file for an explanation
 * of the solving algorithm used.
 */
void solveRec(Vector<triangleT> & trianglesLeft, Vector<triangleT> & trianglesSoFar, Vector<lineT> & boundaryLines, puzzleBorderT & puzzleBorder) {
	if (boundaryLines.size() == 0) return;
	
	for (int boundaryLineCounter = 0; boundaryLineCounter < boundaryLines.size(); boundaryLineCounter++) {	
		lineT boundaryLine = boundaryLines[boundaryLineCounter];

		for (int trianglesLeftCounter = 0; trianglesLeftCounter < trianglesLeft.size(); trianglesLeftCounter++) {			
			triangleT triangle = trianglesLeft[trianglesLeftCounter];

			for (int sideCounter = 0; sideCounter < 3; sideCounter++) {			
				lineT line = triangle.sides[sideCounter];
				lineT obstructedLine;

				// If the current triangle's side is a boundaryLine, and if the triangle will not block any later triangles, 
				// use it next.
				if (compareLines(boundaryLine, line) && doesNotBlock(boundaryLines, triangle, line, obstructedLine)) {
					
					// Update boundaryLines.
					boundaryLines.removeAt(boundaryLineCounter);	
					for (int sideCounter2 = 0; sideCounter2 < 3; sideCounter2++)
						if (sideCounter2 != sideCounter && !isOnBorder(triangle.sides[sideCounter2], puzzleBorder))
							boundaryLines.add(triangle.sides[sideCounter2]);
					for (int boundaryLineCounter2 = 0; boundaryLineCounter2 < boundaryLines.size(); boundaryLineCounter2++) {
						if (compareLines(boundaryLines[boundaryLineCounter2], obstructedLine)) {
							boundaryLines.removeAt(boundaryLineCounter2);
							boundaryLineCounter2--;
						}
					}
					removeDuplicateBoundaryLines(boundaryLines);

					// Update trianglesSoFar.
					trianglesSoFar.add(triangle);

					// Update trianglesLeft.
					trianglesLeft.removeAt(trianglesLeftCounter);

					solveRec(trianglesLeft, trianglesSoFar, boundaryLines, puzzleBorder);
					return;
				}
			}
		}
	}

	Error("No solution! The puzzle cannot be solved.");
	return;
}

/*
 * Searches through all the triangles and finds the min/max x/y coordinates, then returns a puzzleBorderT specified by
 * those values.
 */
puzzleBorderT findPuzzleBorder(Vector<triangleT> triangles) {
	puzzleBorderT puzzleBorder;
	puzzleBorder.minX = triangles[0].sides[0].x1;
	puzzleBorder.minY = triangles[0].sides[0].y1;
	puzzleBorder.maxX = triangles[0].sides[0].x1;
	puzzleBorder.maxY = triangles[0].sides[0].y1;
	for (int triangle = 0; triangle < triangles.size(); triangle++) {
		for (int side = 0; side < 3; side++) {
			if (min(triangles[triangle].sides[side].x1, triangles[triangle].sides[side].x2) < puzzleBorder.minX) 
				puzzleBorder.minX = min(triangles[triangle].sides[side].x1, triangles[triangle].sides[side].x2);
			if (max(triangles[triangle].sides[side].x1, triangles[triangle].sides[side].x2) > puzzleBorder.maxX) 
				puzzleBorder.maxX = max(triangles[triangle].sides[side].x1, triangles[triangle].sides[side].x2);
			if (min(triangles[triangle].sides[side].y1, triangles[triangle].sides[side].y2) < puzzleBorder.minY) 
				puzzleBorder.minY = min(triangles[triangle].sides[side].y1, triangles[triangle].sides[side].y2);
			if (max(triangles[triangle].sides[side].y1, triangles[triangle].sides[side].y2) > puzzleBorder.maxY) 
				puzzleBorder.maxY = max(triangles[triangle].sides[side].y1, triangles[triangle].sides[side].y2);
		}
	}
	return puzzleBorder;
}

/*
 * Finds all the triangles that lie on the bottom side of the border and that are concave.
 */
void findStartingTriangles(Vector<triangleT> & triangles, Vector<triangleT> & trianglesSoFar, Vector<lineT> & boundaryLines, puzzleBorderT puzzleBorder) {
	for (int triangleCounter = 0; triangleCounter < triangles.size(); triangleCounter++) {
		for (int sideCounter = 0; sideCounter < 3; sideCounter++) {
			triangleT triangle = triangles[triangleCounter];
			if (triangle.sides[sideCounter].y1 == puzzleBorder.minY && triangle.sides[sideCounter].y2 == puzzleBorder.minY) {
				if (isConcave(triangle, triangle.sides[sideCounter])) {
					trianglesSoFar.add(triangle);
					for (int nonBorderSide = 0; nonBorderSide < 3; nonBorderSide++)
						if (!isOnBorder(triangle.sides[nonBorderSide], puzzleBorder)) 
							boundaryLines.add(triangle.sides[nonBorderSide]);
					triangles.removeAt(triangleCounter);
					triangleCounter--;
					break;
				}

			}
		}
	}
}

/*
 * Returns true if the two given lines have the same endpoints.
 */
bool compareLines(lineT a, lineT b) {
	if (a.x1 == b.x1 && a.y1 == b.y1 && a.x2 == b.x2 && a.y2 == b.y2) return true;
	if (a.x1 == b.x2 && a.y1 == b.y2 && a.x2 == b.x1 && a.y2 == b.y1) return true;
	return false;
}

/*
 * Returns true if the given line lies on the puzzle's border.
 */
bool isOnBorder(lineT possibleBorder, puzzleBorderT puzzleBorder) {
	if (possibleBorder.x1 == puzzleBorder.minX && possibleBorder.x2 == puzzleBorder.minX) return true;
	if (possibleBorder.x1 == puzzleBorder.maxX && possibleBorder.x2 == puzzleBorder.maxX) return true;
	if (possibleBorder.y1 == puzzleBorder.minY && possibleBorder.y2 == puzzleBorder.minY) return true;
	if (possibleBorder.y1 == puzzleBorder.maxY && possibleBorder.y2 == puzzleBorder.maxY) return true;
	return false;
}

/*
 * Returns true if the given triangle is concave.
 */
bool isConcave(triangleT triangle, lineT baseLine) {
	lineT tmpLine;
	return isConcave(triangle, baseLine, tmpLine);
}

/*
 * Returns true if the given triangle is concave, as specified by a triangleT. Updates obstructedLine by reference
 * in order to remember which of this triangle's lines, if any, would be obstructed by adding this triangle.
 */
bool isConcave(triangleT triangle, lineT contactLine, lineT & obstructedLine) {
	int minX = min(contactLine.x1, contactLine.x2);
	int maxX = max(contactLine.x1, contactLine.x2);
	for (int side = 0; side < 3; side++) {
		if (triangle.sides[side].x1 < minX || triangle.sides[side].x1 > maxX) {
			obstructedLine = triangle.sides[side];
			return false;
		} else if (triangle.sides[side].x2 < minX || triangle.sides[side].x2 > maxX) {
			obstructedLine.x1 = triangle.sides[side].x2;
			obstructedLine.y1 = triangle.sides[side].y2;
			obstructedLine.x2 = triangle.sides[side].x1;
			obstructedLine.y2 = triangle.sides[side].y1;
			return false;
		}
	}
	return true;
}

/*
 * Returns true if the given triangle will not block any of the remaining triangles. contactLine is the line defined by
 * the contact between this triangle and the triangles used so far. obstructedLine is the line, if it exists, which would
 * be obstructed by adding this triangle.
 */
bool doesNotBlock(Vector<lineT> & boundaryLines, triangleT triangle, lineT contactLine, lineT & obstructedLine) {
	if (isConcave(triangle, contactLine, obstructedLine)) return true;
	else defineObstructedLine(triangle, obstructedLine);
	return obstructedLineIsBoundaryLine(boundaryLines, obstructedLine);
}

/*
 * Returns the bottom-facing line of a triangle which would be obstructed if this triangle were to be inserted.
 */
void defineObstructedLine(triangleT triangle, lineT & obstructedLine) {
	for (int side = 0; side < 3; side++) {
		if (triangle.sides[side].x1 == obstructedLine.x1 && 
			triangle.sides[side].y1 == obstructedLine.y1 && 
			triangle.sides[side].y2 < obstructedLine.y2) {
			obstructedLine.x2 = triangle.sides[side].x2;
			obstructedLine.y2 = triangle.sides[side].y2;
		} else if (triangle.sides[side].x2 == obstructedLine.x1 && 
			triangle.sides[side].y2 == obstructedLine.y1 && 
			triangle.sides[side].y1 < obstructedLine.y2) {
			obstructedLine.x2 = triangle.sides[side].x1;
			obstructedLine.y2 = triangle.sides[side].y1;
		}
	}
}

/*
 * Returns true if a given obstructedLine is on the puzzle boundary.
 */
bool obstructedLineIsBoundaryLine(Vector<lineT> & boundaryLines, lineT & obstructedLine) {
	for (int boundary = 0; boundary < boundaryLines.size(); boundary++)
		if ((obstructedLine.y1 == 0 && obstructedLine.y2 == 0)|| compareLines(boundaryLines[boundary], obstructedLine)) 
			return true;
	lineT newLine;
	obstructedLine = newLine;
	return false;
}

/*
 * Removes all the duplicates from boundaryLines.
 */
void removeDuplicateBoundaryLines(Vector<lineT> & boundaryLines) {
	for (int boundaryLine1 = 0; boundaryLine1 < boundaryLines.size(); boundaryLine1++) {
		for (int boundaryLine2 = boundaryLine1 + 1; boundaryLine2 < boundaryLines.size(); boundaryLine2++) {
			if (compareLines(boundaryLines[boundaryLine1], boundaryLines[boundaryLine2])) {
				boundaryLines.removeAt(boundaryLine1);
				boundaryLine2--;				
				boundaryLines.removeAt(boundaryLine2);
				boundaryLine1--;
				boundaryLine2--;
			}
		}
	}
}

/**************************************/
/* Print Functions (For Testing Only) */
/**************************************/

/*
 * Prints a triangle's name and a description of each of its sides.
 */
void printTriangle(triangleT triangle) {
	cout << "******triangleT " << triangle.num << "******" << endl;
	printLine(triangle.sides[0], 1);
	printLine(triangle.sides[1], 2);
	printLine(triangle.sides[2], 3);
	cout << endl;
}

/*
 * Prints the coordinates of a line's endpoints.
 */
void printLine(lineT line, int num) {
	cout << "Line " << num << ": " << "(" << line.x1 << "," << line.y1 << ") --> (" << line.x2 << "," << line.y2 << ")" << endl;
}

/*
 * Prints the order in which the triangles should be inserted in order to solve the puzzle.
 */
void printSolution(Vector<triangleT> solution) {
	cout << "\nSolution: ";
	foreach (triangleT curr in solution)
		cout << curr.num << " ";
	cout << endl;
}