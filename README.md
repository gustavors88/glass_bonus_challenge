glass_bonus_challenge
=====================
Bonus challenge offered to CS106B students winter 2012. Was the only student to solve it.

Author: Joshua Grinberg
Date: 12-23-2011

Description
-----------------------
This program determines how to reassemble a glass window broken into triangular shards by
dropping them in from above, one at a time. The user has the option of loading a puzzle
from a saved file, or creating his own by drawing triangles on a grid. Once a puzzle is
loaded or created, the user can then watch an animation of the triangles being dropped 
from above in the correct order.

To solve the puzzle, the program first looks through all the triangles to find the
"starting triangles", or the ones that lie on the bottom side of the rectangular border
and that are concave, meaning that if they are inserted now, they will not block any 
triangles later on. (It will be described later how to determine if a triangle will block
another triangle later on.)

Below is an example of two valid starting triangles. They both lie on the bottom face and
will not block any triangles later on.
     ______________________
    |                      |
    |                      |
    |                /\    |
    |    /\         /  \   |
    |___/__\_______/____\__| FIGURE 1


Below is an example of two invalid starting triangles. The first does not lie on the bottom
face and the second will block a triangle later on (the one that touches its ride side).
       ______________________
      |                 _//  |
      |   /\          _/ /   |
      |  /__\       _/  /    |
      |           _/   / X   |
      |__________/___ /______| FIGURE 2


The program then stores a list of all the exposed, upward-facing sides of these starting 
triangles. The lines in this list are called boundary lines because they describe the 
boundary of all the triangles inserted so far. 
 
There are four boundary lines in the example below (lines touching the border are not
included).
       ______________________
      |                      |
      |                      |
      |                /\    |
      |  1 /\ 2     3 /  \ 4 |
      |___/__\_______/____\__| FIGURE 3

 
Next, the program recursively searches through all the unused triangles until it finds one with 
a side that is also a boundary line. This triangle is inserted next if it will not block a
triangle later on. (It will be described later how to determine if a triangle will block
another triangle later on.) The boundary lines list is updated so that the contact line is
removed (since it is no longer exposed), and any new, exposed, upward-facing sides of the newly
inserted triangle are added to the boundary lines list. 

In the example below, first there are three boundary lines. Once the triangle is inserted, boundary
line #1 is removed since it is the line of contact, and boundary line #4 is added since it is
exposed and upward-facing.
 
       ______________________                          ______________________
      |                      |                        |                      |
      |                      |                        |                      |
      |                      |         _______        |_4_____               |
      |     _/\             /|        |     _/        |     _/\             /|
      | 1 _/   \ 2       3 / |   +    |   _/     =    |   _/   \ 2       3 / |
      | _/      \         /  |        | _/            | _/      \         /  |
      |/_________\_______/___|        |/              |/_________\_______/___| FIGURE 4
 

Note that the list of boundary lines will always describe the exposed boundary of all the 
triangles inserted so far, as depicted in the example below.
      ______________________                          ______________________
      |                      |                        |                      |
      |                      |                        |                      |
      |_4_____               |                        |_4_____               |
      |     _/\             /|        \\__            |     _/\\__          /|
      |   _/   \ 2       3 / |   +     \  \__      =  |   _/   \  \__5   3 / |
      | _/      \         /  |          \    \__      | _/      \    \__  /  |
      |/_________\_______/___|           \______\     |/_________\______\/___| FIGURE 5


This recursive process is repeated until the list of boundary lines is reduced to a size of zero,
at which point the puzzle is solved. There are no boundary lines in the example below (recal that
lines on the rectangular border are not considered boundary lines).
       ______________________
      |  /\       __/ \__    |
      | /  \   __/       \__ |
      |/____\_/_____________\|
      |      /\\__          /|
      |    /   \  \__      / |
      |  /      \    \__  /  |
      |/_________\______\/___| FIGURE 6


It was mentioned earlier that a new triangle is inserted only if one (or more) of its sides 
is a boundary line, and if inserting the triangle now will not block any triangles later on. 
How is it determined if inserting the triangle now will not block any triangles later on? Look 
at the point of the triangle that is not an endpoint of the contact line. In the examples
below, the contact lines are between points A and B, and the third points are points C.
       ___________________________                     
      |                          |                      
      |                          |                        
      |_______ A                 |    A _______ C    
      |      /\\__               |      \__    \
      |    /   \  \__            |   +     \__  \
      |  /      \    \__         |            \__\
      |/_________\______\_B______|               \\ B        FIGURE 7

       __________________________                     
      |                          |                      
      |                          |                        
      |_______ A                 |    A ______________ C    
      |      /\\__               |      \__          /    
      |    /   \  \__            |   +     \__      /   
      |  /      \    \__         |            \__  /     
      |/_________\______\_B______|               \/ B        FIGURE 8

 
Is the x-coordinate of this third point (C) between the x-coordinates of the contact line endpoints 
(A and B)? In FIGURE 7, C is between A and B. This means that the triangle is safe to add now. 
In FIGURE 8, C is not between A and B. This means further tests need to be conducted to determine 
if the triangle is safe to add now, because this triangle might block another triangle with a side 
between B and C.
       __________________________                           __________________________
      |                          |                         |                          |
      |                          |                         |                          |
      |_______                   |      ______________ C   |______________________    |
      |      /\\__               |      \__          /     |      /\\__          /    |
      |    /   \  \__            |   +     \__      /   =  |    /   \  \__      /     |
      |  /      \    \__         |            \__  /       |  /      \    \__  / X    |
      |/_________\______\________|               \/ B      |/_________\______\/_______| FIGURE 9


A triangle with an over-extending third point (C) will not bock any triangles later on
if the line between the third point (C) and the lower contact line endpoint (B) is
already a boundary line (since this means the triangle which would have been blocked has
already been added). In the example below, the line between C and B is already a boundary 
line (4), so even though the x-coordinate of C is not between the x-coordinates of A and B, 
it is safe to add the triangle now.
       __________________________                           __________________________
      |                          |                         |                          |
      |                          |                         |                          |
      |___1___                   |    A ______________ C   |___1___________6______    |
      |      /\\__          /\   |      \__          /     |      /\\__          /\   |
      |    /   \  \__2   4 /  \ 5|   +     \__2     /   =  |    /   \  \__      /  \ 5|
      |  /      \    \__  /    \ |            \__  /       |  /      \    \__  /    \ |
      |/_________\______\/______\|               \/ B      |/_________\______\/______\| FIGURE 10
 

That's it! This algorithm will work for any combination of triangles, no matter how complicated.
Try the pre-loaded puzzles, or create your own! 
