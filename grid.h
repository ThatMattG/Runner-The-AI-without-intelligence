#ifndef GRID_H
#define GRID_H

// Rows and columns should be odd to allow a centre point to be set
#define DEFAULT_ROWS 45
#define DEFAULT_COLUMNS 45

#include "runner.h"

typedef struct _grid *Grid;

// Creates a new grid
Grid grid_new(int num_rows, int num_columns);

// Frees all memory associated with a grid
void grid_drop(Grid grid);

// Returns a pointer to the board stored in a grid
int **get_board(Grid grid);

// Returns the number of rows in a grid
int get_num_rows(Grid grid);

// Returns the number of columns in a grid
int get_num_columns(Grid grid);

// Creates and returns a string which visually represents the current board
char *get_board_str(Grid grid);

// Removes runner and border info from the board
void clear_board(Grid grid);

#endif