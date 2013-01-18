/*
 * File: gpathfinder.h
 * -------------------
 * The gpathfinder.h file defines the interface for a set of functions
 * that help draw the necessary parts of the Pathfinder maps.  The
 * gpathfinder.h interface uses screen coordinates in which distances
 * on the screen are expressed in pixels and in which the origin is the
 * upper left corner of the window.  Several of the functions defined
 * in this package use values of type pointT, which is simply a pair of
 * x and y coordinate values, as defined in the point.h interface.

 * Note that calls to the drawing functions in this interface are not
 * immediately reflected on the screen.  To get the changes to appear,
 * you need to call UpdatePathfinderDisplay.  This design is more
 * efficient in terms of graphics processing, because it allows the
 * client to make many calls to the various Draw operations and then
 * update the display all at once.
 *
 * This interface also exports several methods for creating buttons
 * in a control strip and for responding to button and mouse clicks.
 * The general approach parallels the standard model used in modern
 * event-driven systems.  The client application creates a series of
 * buttons, each of which supplies a callback function that is
 * invoked whenever that button is clicked.  The application then
 * calls PathfinderEventLoop, which waits for events generated in
 * response to user actions.
 */

#ifndef _gpathfinder_h
#define _gpathfinder_h

#include "genlib.h"
#include "point.h"

/*
 * Constants
 * ---------
 * A few program-wide constants concerning the graphical display.
 * All coordinate values and distances are expressed in pixels.
 */

const double NODE_RADIUS = 5;             /* Radius of the node circle     */
const double PAUSE_TIME = .05;            /* Controls speed of animation   */
const string ARC_COLOR = "Dark gray";     /* Normal arc color              */
const string NODE_COLOR = "Black";        /* Normal node color             */
const string HIGHLIGHT_COLOR = "Red";     /* Color of chosen path/nodes    */
const string DIM_COLOR = "Gray";          /* Color of unchosen path/nodes  */
const int FONT_SIZE = 14;                 /* Font size for node labels     */

void DrawBox(double x, double y, double width, double height);
void FillBox(double x, double y, double width, double height);

/*
 * Function: InitPathfinderGraphics
 * Usage: InitPathfinderGraphics();
 * --------------------------------
 * Initializes the graphics window for Pathfinder.  This call should
 * be the first statement in main.
 */

void InitPuzzleGraphics();

/*
 * Function: DrawPathfinderMap
 * Usage: DrawPathfinderMap(mapFile);
 * ----------------------------------
 * Clears the graphics window and then draws the image contained in
 * the specified image file, which will typically live in a Pictures
 * subdirectory of the project directory.
 */

void DrawPathfinderMap(string mapFile);

/*
 * Function: DrawPathfinderNode
 * Usage: DrawPathfinderNode(center, color, label);
 * ------------------------------------------------
 * Draw a node circle whose center is at the coordinate position
 * specified by the first argument and that is filled in the specified
 * color.  If a third argument is provided, the function draws a label
 * to the right of the circle containing the specified text.
 */

void DrawPathfinderNode(pointT center, string color, string label = "");

/*
 * Function: DrawPathfinderArc
 * Usage: DrawPathfinderArc(start, end, color);
 * --------------------------------------------
 * Draws a line on the screen connecting the two specified coordinate
 * positions using the indicated color.
 */

void DrawPathfinderArc(pointT start, pointT end, string color);

/*
 * Function: UpdatePathfinderDisplay
 * Usage: UpdatePathfinderDisplay();
 * ---------------------------------
 * Updates the display to reflect recent calls to drawing functions
 * such as DrawPathfinderNode and DrawPathfinderArc.
 */

void UpdatePathfinderDisplay();

/*
 * Function: AddButton
 * Usage: AddButton(name, actionFn);
 *       AddButton(name, actionFn, data);
 * ---------------------------------------
 * Adds a button to the window and assigns it an action function.
 * When the button is clicked, the program will invoke
 *
 *       actionFn()
 *
 * or
 *
 *       actionFn(data)
 *
 * depending on whether the data parameter is supplied.  The data
 * parameter is passed by reference, so that the action function
 * can modify the program state.
 */

void AddButton(string name, void (*actionFn)());

template <typename ClientDataType>
void AddButton(string name,
               void (*actionFn)(ClientDataType & data),
               ClientDataType & data);

void RemoveButton(string name);

/*
 * Function: DefineClickListener
 * Usage: DefineClickListener(clickFn);
 *        DefineClickListener(clickFn, data);
 * ------------------------------------------
 * Designates a function that will be called whenever the user
 * clicks the mouse in the graphics window.  If a click listener
 * has been specified by the program, the event loop will invoke
 *
 *       clickFn(pt)
 *
 * or
 *
 *       clickFn(pt, data)
 *
 * depending on whether the data parameter is supplied.  In either
 * case, pt is the pointT at which the click occurred and data
 * is a parameter of any type appropriate to the application.  The
 * data parameter is passed by reference, so that the click function
 * can modify the program state.
 */

void DefineClickListener(void (*actionFn)(pointT pt));

template <typename ClientDataType>
void DefineClickListener(void (*actionFn)(pointT pt, ClientDataType & data),
                         ClientDataType & data);

/*
 * Function: PathfinderEventLoop
 * Usage: PathfinderEventLoop();
 * -----------------------------
 * Initiates a loop that repeatedly waits for the user to click
 * on a button and calls the action function associated with that
 * button.  Moreover, if the client has registered a click listener,
 * PathfinderEventLoop will call that listener whenever the mouse is
 * clicked inside the window.
 *
 * Note that PathfinderEventLoop never returns, so programs that need
 * to exit on user command need to call the exit() function in the
 * standard libraries.
 */

void PathfinderEventLoop();

/*
 * Function: GetMouseClick
 * Usage: pt = GetMouseClick();
 * ----------------------------
 * Waits for the user to click somewhere on the graphics window
 * and returns the coordinate of where the click took place.
 */

pointT GetMouseClick();

/* Include the implementation of the template functions */

#include "gpathfinderimpl.cpp"

#endif
