#ifndef GAME_H
#define GAME_H

#include "grid.h"
#include "runner.h"

// Increase PRINT_EVERY_NTH_STEP to speed up simulation.
// With sane grid size and number of runners, a sufficiently high (>30) 
// PRINT_EVERY_NTH_STEP will allow the simulation to run in only a few seconds
#define PRINT_EVERY_NTH_STEP 1
#define SLEEP_TIME_MS 10

typedef struct _game *Game;

// Returns a new game with randomised paths. 
// Generally used only for the first generation.
Game game_new(int gen_num, 
		int num_rows, 
		int num_columns, 
		int num_runners, 
		int path_length);
	
// Frees all memory associated with a game
void game_drop(Game game);

// Returns the runner at index 'runner_num'
Runner get_runner(Game game, int runner_num);

// Returns a pointer to the first runner in the runners array who 
// is on a border. This indicates that the game is over.
// Returns NULL if no runner is on a border.
Runner runner_on_border(Game game);

// Prompts every runner to take their next step
void advance_runners(Game game, int step_num);

// Places numbers on the board to indicate the number of runners at 
// each location.
void populate_board(Game game);

// Clears the screen, then prints a header showing the generation and round. 
// Also visually displays the arrangement of runners on the board.
void print_game(Game game);

// Runs a full single generation to run.
// The game stated will be displayed automatically.
// The game object will update appropriately and remain intact for analysis.
void run_generation(Game game);

// Returns a pointer to the winning runner if present.
// Otherwise returns NULL.
Runner get_winner(Game game);

// Creates and returns the next game to be played.
// Handles games with winners and without winners.
Game game_next(Game game);

// Returns the number of steps that have been taken in the game.
int steps_taken(Game game);

// Returns a new game with runners based on a given 'fittest' runner
// (each cloned and mutated). Used by game_next().
Game game_evolve(int gen_num, 
		int num_rows, 
		int num_columns, 
		int num_runners, 
		int path_length, 
		Runner fittest);

// Prints a runner's path. Unlikely to be used except for debugging.
void print_runner_path(Game game, int runner_num, int path_length);

#endif