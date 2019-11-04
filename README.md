# Runner-The-AI-without-intelligence
Armed with a rudimentary understanding of basic AI concepts, I've created this simple machine learning demonstration.

NOTE: The game board will be shown and updated in the terminal window - rapid flickering is likely to occur. Adjust SLEEP_TIME_MS in game.h to a suitable level if required.

This project is in a basic working state. The final version may allow for modification of default values via command-line arguments. 

Testing and working on both Windows and Linux.

Summary:
 - A grid is created and a number of "runners" are spawned at the centre.
 - These runners are provided with a list of random directional instructions (Up, Down, Left, Right)
 - The following steps are repeated multiple times:
   - The runners will follow their path until one runner reaches the border of the grid
   - The current round will now end, and all runners will be removed except for the one which reached the border first
   - This winner will now be cloned, with each of these runner clones being assigned a modified version of the winner's path
