#include <stdio.h>
#include <stdlib.h>

// Platform specific libraries ( for Sleep() / usleep() )
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#include "game.h"
#include "grid.h"
#include "runner.h"

// __________________________________________________
//
//			Structs
// __________________________________________________

typedef struct _game {
	int gen_num;
	int num_runners;
	int path_length;
	int steps_taken;
	Runner winner;
	Grid grid;
	Runner *runners;
} game;

// __________________________________________________
//
//			Function Prototypes
// __________________________________________________

static Game game_new_without_runners(int gen_num, 
		int num_rows, 
		int num_columns, 
		int num_runners, 
		int path_length);
static void show_game_state(Game game);
static void mark_borders(Game game);
static void clear_screen();
static void sleep_ms(int sleep_time);
static void print_header(Game game);
static void print_separator(int width);

// __________________________________________________
//
//			Function Implementations
// __________________________________________________

Game game_new(int gen_num, 
		int num_rows, 
		int num_columns, 
		int num_runners, 
		int path_length)
{
	// Set up game attributes except runners array
	game *new = game_new_without_runners(gen_num, 
			num_rows, 
			num_columns, 
			num_runners, 
			path_length);
	
	// Generate random-path runners
	int start_row = num_rows / 2;
	int start_column = num_columns / 2;
	Runner *new_runners = runners_new(num_runners, 
			start_row, 
			start_column, 
			path_length);
	new->runners = new_runners;
	
	return new;
}

void game_drop(Game game)
{
	grid_drop(game->grid);
	runners_drop(game->num_runners, game->runners);
	free(game);
}

Runner get_runner(Game game, int runner_num)
{
	Runner runner = game->runners[runner_num];
	return runner;
}

Runner runner_on_border(Game game)
{
	Runner border_runner = NULL;
	Grid grid = game->grid;
	
	int first_row = 0;
	int last_row = get_num_rows(grid) - 1;
	int first_column = 0;
	int last_column = get_num_columns(grid) - 1;
	
	int row;
	int column;
	Runner runner;
	
	// Iterate through runners, checking if any are on a border
	for (int runner_num = 0; runner_num < game->num_runners; runner_num++)
	{
		runner = get_runner(game, runner_num);
		row = get_row(runner);
		column = get_column(runner);
		
		// Check if runner on first or last row or column (borders)
		if (row == first_row || 
			row == last_row || 
			column == first_column || 
			column == last_column)
		{
			border_runner = runner;
			break;
		}
	}
	
	// Returns NULL automatically if no runners are on a border
	return border_runner;
}

void advance_runners(Game game, int step_num)
{
	Runner runner = NULL;
	for (int runner_num = 0; runner_num < game->num_runners; runner_num++)
	{
		runner = get_runner(game, runner_num);
		advance(runner, step_num);
	}
}

void populate_board(Game game)
{
	Grid grid = game->grid;
	int **board = get_board(grid);
	
	Runner runner;
	
	int row = 0;
	int column = 0;
	for (int runner_num = 0; runner_num < game->num_runners; runner_num++)
	{
		runner = get_runner(game, runner_num);
		row = get_row(runner);
		column = get_column(runner);
		
		// Case: runner is on a border. Increase value to 1.
		if (board[row][column] < 0)
		{
			board[row][column] = 1;
		}
		// Otherwise, increment the cell's stored value
		else
		{
			board[row][column] += 1;			
		}
	}
	puts("");
}

void print_game(Game game)
{
	Grid grid = game->grid;
	
	// Generates a string representing the board and then print it.
	// String generation is required for better visual performance;
	// printing the board using loops causes the screen to flash.
	char *board_str = get_board_str(grid);
	printf("%s", board_str);
	free(board_str);
	puts("");
}

void run_generation(Game game)
{
	int path_length = game->path_length;
	
	// Display the game state before first step is taken
	show_game_state(game);
	sleep_ms(SLEEP_TIME_MS);
	
	while (game->steps_taken < path_length && runner_on_border(game) == NULL)
	{
		advance_runners(game, game->steps_taken);
		game->steps_taken = game->steps_taken + 1;
		
		// Every nth step is shown (#defined in game.h)
		if (game->steps_taken % PRINT_EVERY_NTH_STEP == 0)
		{
			show_game_state(game);
			sleep_ms(SLEEP_TIME_MS);
		}
		
	}
	
	printf("\n~~~ Steps taken: %d ~~~\n", game->steps_taken);
	
	game->winner = runner_on_border(game);
}

Runner get_winner(Game game)
{
	return game->winner;
}

int steps_taken(Game game)
{
	return game->steps_taken;
}

Game game_next(Game game)
{
	Game next_game;
	Runner fittest = game->winner;
	
	if (fittest == NULL)
	{
		next_game = game_new(game->gen_num + 1, 
				get_num_rows(game->grid), 
				get_num_columns(game->grid), 
				game->num_runners, 
				game->path_length);
	}
	else
	{
		next_game = game_evolve(game->gen_num + 1, 
				get_num_rows(game->grid), 
				get_num_columns(game->grid), 
				game->num_runners, 
				game->path_length,
				fittest);
	}
	
	return next_game;
}

Game game_evolve(int gen_num, 
		int num_rows, 
		int num_columns, 
		int num_runners, 
		int path_length, 
		Runner fittest)
{
	game *new = game_new_without_runners(gen_num, 
			num_rows, 
			num_columns, 
			num_runners, 
			path_length);
	
	int start_row = num_rows / 2;
	int start_column = num_columns / 2;
	Runner *new_runners = runners_offspring(num_runners, 
			start_row, 
			start_column, 
			path_length, 
			fittest);
	new->runners = new_runners;
	
	return new;
}

void print_runner_path(Game game, int runner_num, int path_length)
{
	Runner runner = get_runner(game, runner_num);
	print_path(runner, path_length);
}

// __________________________________________________
//
//			Helper Functions
// __________________________________________________

// Creates a new game. Runners will need to be added using either
// runners_new() or runners_offspring() from runner.h
static Game game_new_without_runners(int gen_num, 
		int num_rows, 
		int num_columns, 
		int num_runners, 
		int path_length)
{
	game *new = malloc(sizeof(game));
	if (new == NULL) 
	{
        fprintf(stderr, "Could not allocate memory for game");
    }
	
	new->gen_num = gen_num;
	new->num_runners = num_runners;
	new->path_length = path_length;
	new->steps_taken = 0;
	new->winner = NULL;
	new->grid = grid_new(num_rows, num_columns);
	
	new->runners = NULL;
	return new;
}

// Renders, prints and cleans up
static void show_game_state(Game game)
{
	mark_borders(game);
	populate_board(game);
	char *board_str = get_board_str(game->grid);
	fflush(stdout);
	clear_screen();
	print_header(game);
	printf("%s", board_str);
	puts("");
	free(board_str);
	clear_board(game->grid); 
}

// Sets the border cells of the board to -1
static void mark_borders(Game game)
{
	Grid grid = game->grid;
	int **board = get_board(grid);
	
	int first_row = 0;
	int last_row = get_num_rows(grid) - 1;
	int first_column = 0;
	int last_column = get_num_columns(grid) - 1;
	
	for (int row = 0; row < get_num_rows(grid); row++)
	{
		board[row][first_column] = -1;
		board[row][last_column] = -1;
	}
	for (int column = 0; column < get_num_columns(grid); column++)
	{
		board[first_row][column] = -1;
		board[last_row][column] = -1;
	}
}

static void clear_screen()
{
	#ifdef _WIN32
	system("cls");
	#else
	// Previous board may still be visible with only 1 clear,
	// so clear twice.
	system("clear");
	system("clear");
	#endif
}

static void sleep_ms(int sleep_time)
{
	return;
	#ifdef _WIN32
	Sleep(sleep_time);
	#else
	usleep(sleep_time * 1000);
	#endif
}

// Prints a heading including the generation and round num
static void print_header(Game game)
{
	print_separator(get_num_columns(game->grid));
	printf("    GEN %2d \n", game->gen_num);
	printf("    RUNNERS %2d \n", game->num_runners);
	printf("    ROUND %2d \n", game->steps_taken);
	print_separator(get_num_columns(game->grid));
}

// Prints a separator of a given width. Generally used for headings
static void print_separator(int width)
{
	char separator_char = '=';
	char *separator_str = calloc(width + 1, sizeof(char));
	int index;
	for (index = 0; index < width; index++)
	{
		separator_str[index] = separator_char;
	}	
	printf("%s", separator_str);
	free(separator_str);
	puts("");
}

