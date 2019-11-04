#include <stdio.h>
#include <stdlib.h>

#include "grid.h"

// __________________________________________________
//
//			Structs
// __________________________________________________

typedef struct _grid {
	int rows;
	int columns;
	int **board;
} grid;

// __________________________________________________
//
//			Function Prototypes
// __________________________________________________

static int **new_2d_array(int num_rows, int num_columns, size_t size);

// __________________________________________________
//
//			Function Implementations
// __________________________________________________

Grid grid_new(int num_rows, int num_columns)
{
	Grid new = malloc(sizeof(grid));
	
	new->rows = num_rows;
	new->columns = num_columns;
	new->board = new_2d_array(num_rows, num_columns, sizeof(int));
	
	return new;
}

void grid_drop(Grid grid)
{
	int **board = grid->board;
	for (int row = 0; row < grid->rows; row++)
	{
		free(board[row]);
	}
	
	free(board);
	
	free(grid);
}

int **get_board(Grid grid)
{
	return grid->board;
}

int get_num_rows(Grid grid)
{
	return grid->rows;
}

int get_num_columns(Grid grid)
{
	return grid->columns;
}

char *get_board_str(Grid grid)
{
	// The symbols string must have a space at index 0 (for empty ints)
	char symbols[] = " 123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	char border = '#';
	int **board = grid->board;
	
	int num_rows = grid->rows;
	int chars_per_row = (grid->columns) + 1;
	int str_len = num_rows * (chars_per_row) * 2;
	char *board_str = calloc(str_len, sizeof(char));
	if (board_str == NULL) 
	{
        fprintf(stderr, "Could not allocate memory for board string");
    }
	
	int row = 0;
	int column = 0;
	int item;
	for (row = 0; row < grid->rows; row++)
	{
		for (column = 0; column < grid->columns; column++)
		{
			item = board[row][column];
			
			// Render border
			if (item == -1)
			{
				board_str[row * (chars_per_row) + column] = border;
			}
			// For empty cells and cells with runners
			else
			{
				board_str[row * (chars_per_row) + column] = symbols[item];
			}
		}
		board_str[row * (chars_per_row) + column] = '\n';
	}
	
	return board_str;
}

void clear_board(Grid grid)
{
	int **board = grid->board;
	int num_rows = grid->rows;
	int num_columns = grid->columns;
	
	// Set board elements
	for (int row = 0; row < num_rows; row++)
	{
		for (int column = 0; column < num_columns; column++)
		{
			board[row][column] = 0;
		}
	}
}

// __________________________________________________
//
//			Helper Functions
// __________________________________________________

// Returns a 2d array to be used as a grid's board
static int **new_2d_array(int num_rows, int num_columns, size_t size)
{	
	int **board = calloc(num_rows, sizeof(int *));
	if (board == NULL)
	{
        fprintf(stderr, "Could not allocate memory for 2D board array");
    }
	
	for (int row = 0; row < num_rows; row++)
	{
		board[row] = calloc(num_columns, sizeof(int));
	}
	
	return board;
}
