#include <stdio.h>
#include <stdlib.h>

#include "runner.h"

#define NUM_DIRECTIONS 4

// __________________________________________________
//
//			Typedefs
// __________________________________________________

typedef enum direction {
	UP,
	RIGHT,
	DOWN,
	LEFT
} direction;

typedef struct _runner {
	int row;
	int column;
	direction *path;
} runner;

// __________________________________________________
//
//			Function Prototypes
// __________________________________________________

static Runner *runners_memory_alloc(int num_runners);
static Runner runner_new(int start_row, int start_column, int path_length);
static void randomise_path(Runner runner, int path_length);
static Runner runner_clone(int start_row, int start_column, int path_length, Runner parent);
static Runner runner_new_without_path(int start_row, int start_column, int path_length);
static direction *clone_direction_array(int length, direction *source);
static void mutate_path(Runner runner, int path_length);
static void runner_drop(Runner runner);

// __________________________________________________
//
//			Function Implementations
// __________________________________________________

Runner *runners_new(int num_runners, int start_row, int start_column, int path_length)
{
	runner **runners = runners_memory_alloc(num_runners);
	
	for (int runner_num = 0; runner_num < num_runners; runner_num++)
	{
		runners[runner_num] = runner_new(start_row, start_column, path_length);
		randomise_path(runners[runner_num], path_length);
	}
	
	return runners;
}

void runners_drop(int num_runners, Runner *runners)
{
	for (int runner_num = 0; runner_num < num_runners; runner_num++)
	{
		runner_drop(runners[runner_num]);
	}
	free(runners);
}

Runner *runners_offspring(int num_runners, int start_row, int start_column, int path_length, Runner parent)
{
	runner **runners = runners_memory_alloc(num_runners);
	
	// First runner is a clone with no mutations
	runners[0] = runner_clone(start_row, start_column, path_length, parent);
	
	// Mutate other runner paths
	for (int runner_num = 1; runner_num < num_runners; runner_num++)
	{
		runners[runner_num] = runner_clone(start_row, start_column, path_length, parent);
		mutate_path(runners[runner_num], path_length);
	}
	
	return runners;
}

void advance(Runner runner, int step_num)
{
	direction *path = runner->path;
	direction dir = path[step_num];
	int row = runner-> row;
	int column = runner->column;
	
	switch (dir)
	{
		case UP:
			runner->row = row - 1;
			break;
		case RIGHT:
			runner->column = column + 1;
			break;
		case DOWN:
			runner->row = row + 1;
			break;
		case LEFT:
			runner->column = column - 1;
			break;
		default:
			fprintf(stderr, "Unexpected direction");
	}
}

int get_row(Runner runner)
{
	return runner->row;
}

int get_column(Runner runner)
{
	return runner->column;
}

void print_path(Runner runner, int path_length)
{
	direction *path = runner->path;
	for (int i = 0; i < path_length; i++)
	{
		printf("%d -> ", path[i]);
	}
	printf("\n");
}

// __________________________________________________
//
//			Helper Functions
// __________________________________________________

static Runner *runners_memory_alloc(int num_runners)
{
	runner **runners = malloc(num_runners * sizeof(runner *));
	if (runners == NULL)
	{
        fprintf(stderr, "Could not allocate memory for runners");
    }
	
	return runners;
}

// Creates a single runner
static Runner runner_new(int start_row, int start_column, int path_length)
{
	runner *new = runner_new_without_path(start_row, start_column, path_length);
	
	direction *path = calloc(path_length, sizeof(direction));
	if (path == NULL) 
	{
        fprintf(stderr, "Could not allocate memory for path");
    }
	
	new->path = path;
	
	return new;
}

// Given a runner, this will populate its path with random directions
static void randomise_path(Runner runner, int path_length)
{
	direction *path = runner->path;
	direction random_direction;
	
	for (int i = 0; i < path_length; i++)
	{
		random_direction = (direction) (rand() % NUM_DIRECTIONS);
		path[i] = random_direction;
	}
}

// Returns a new clone of a runner
static Runner runner_clone(int start_row, int start_column, int path_length, Runner parent)
{
	runner *new = runner_new_without_path(start_row, start_column, path_length);
	
	direction *path = clone_direction_array(path_length, parent->path);
	new->path = path;
	
	return new;
}

// Returns a new runner with the given attributes; does not create a path
static Runner runner_new_without_path(int start_row, int start_column, int path_length)
{
	runner *new = malloc(sizeof(runner));
	if (new == NULL) 
	{
        fprintf(stderr, "Could not allocate memory for runner");
    }
	
	new->row = start_row;
	new->column = start_column;
	new->path = NULL;
	
	return new;
}

// Returns a new clone of a given path
static direction *clone_direction_array(int length, direction *source)
{
	direction *new = calloc(length, sizeof(direction));
	if (new == NULL) 
	{
        fprintf(stderr, "Could not allocate memory for path clone");
    }
	
	for (int index = 0; index < length; index++)
	{
		new[index] = source[index];
	}
	return new;
}

// Randomly mutates up to 1/PATH_MUTATION_DIVISOR steps of a runner's path
static void mutate_path(Runner runner, int path_length)
{
	direction *path = runner->path;
	int random_index;
	direction random_direction;
	
	for (int i = 0; i < path_length / PATH_MUTATION_DIVISOR; i++)
	{
		random_index = rand() % path_length;
		random_direction = (direction) (rand() % NUM_DIRECTIONS);
		path[random_index] = random_direction;
	}
}

// Frees all memory associated with a runner
static void runner_drop(Runner runner)
{
	free(runner->path);
	free(runner);
}



