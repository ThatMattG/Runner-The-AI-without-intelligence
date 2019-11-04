// 
// A very simple program demonstrating the basics of artificial
// intelligence. The runners on the board will (hopefully) improve 
// over time through mutation of the best performer's path.
// 
// Written by ThatMattG (www.github.com/ThatMattG) in May 2019.
// 
// 
// TODO:
// - Makefile
// 

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "game.h"
#include "grid.h"
#include "runner.h"

#define NUM_GENERATIONS 20

// __________________________________________________
//
//			Function Prototypes
// __________________________________________________

void print_summary(int *game_finish_steps, 
		int num_generations, 
		int num_rows, 
		int num_columns);

// __________________________________________________
//
//			Main Function
// __________________________________________________

int main(int argc, char *argv[])
{
	srand(time(NULL));
	
	// Set values using defaults. Update to handle command-line args
	int num_rows		= DEFAULT_ROWS;
	int num_columns		= DEFAULT_COLUMNS;
	int num_runners		= DEFAULT_RUNNERS;
	
	// This path length formula is generally allows a runner to reach a border
	int path_length		= num_rows * num_columns / 5 + 10;
	int num_generations	= NUM_GENERATIONS;
	int gen_num			= 0;
	
	// game_finish_steps will store the number of steps run in each gen
	int game_finish_steps[NUM_GENERATIONS] = {0};
	
	Game game = game_new(0, num_rows, num_columns, num_runners, path_length);
	
	Game next_game;
	
	// Run the simulation repeatedly based on num_generations
	for (gen_num = 0; gen_num < num_generations; gen_num++)
	{
		run_generation(game);
		game_finish_steps[gen_num] = steps_taken(game);
		
		next_game = game_next(game);
		
		game_drop(game);
		game = next_game;
	}
	game_drop(game);
	
	print_summary(game_finish_steps, num_generations, num_rows, num_columns);
	
	return 0;
}

// __________________________________________________
//
//			Helper Functions
// __________________________________________________

void print_summary(int *game_finish_steps, 
		int num_generations, 
		int num_rows, 
		int num_columns)
{
	puts("");
	puts("Summary of the simulation:\n");
	
	printf(" - Generations: %d \n\n", num_generations);
	
	// Print the steps taken for each generation
	printf(" - Steps taken: ");
	for (int i = 0; i < NUM_GENERATIONS; i++)
	{
		printf(" %d ->", game_finish_steps[i]);
	}
	puts(" x\n");
	
	// Calculate and display the best possible score
	int best_score;
	if (num_rows < num_columns)
	{
		best_score = num_rows / 2;
	}
	else
	{
		best_score = num_columns / 2;
	}
	printf(" - Minimum possible steps: %d\n", best_score);
}









