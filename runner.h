#ifndef RUNNER_H
#define RUNNER_H

typedef struct _runner *Runner;

#define DEFAULT_RUNNERS 50

// Number of mutations to a path is inversely proportional 
// to PATH_MUTATION_DIVISOR.
#define PATH_MUTATION_DIVISOR 8

// Creates an array of runners with randomised paths
Runner *runners_new(int num_runners, 
		int start_row, 
		int start_column, 
		int path_length);

// Frees runner memory
void runners_drop(int num_runners, Runner *runners);

// Creates an array of runners with paths mutated from a given parent
Runner *runners_offspring(int num_runners, 
		int start_row, 
		int start_column, 
		int path_length, 
		Runner parent);

// Each runner takes their next step
void advance(Runner runner, int step_num);

// Returns a runner's row
int get_row(Runner runner);

// Returns a runner's column
int get_column(Runner runner);

// Prints a runner's path. Currently unused
void print_path(Runner runner, int path_length);

#endif