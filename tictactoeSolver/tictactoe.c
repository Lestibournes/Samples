#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define SIZE 9
const int WIDTH = 3, HEIGHT = 3, EMPTY = -1, NO_MOVES = -1, MALLOC_ERROR = -2;
int lines[8][3] = {
	{0, 1, 2},
	{3, 4, 5},
	{6, 7, 8},

	{0, 3, 6},
	{1, 4, 7},
	{2, 5, 8},

	{0, 4, 8},
	{2, 4, 6}
};

/**
 * @brief Represents the state of the game.
 * 
 */
struct gameState {
	/**
	 * @brief A 1D array representing a 2D grid. The values are the order in which the move was made (starting from 0), or -1 if empty.
	 * 
	 */
	int board[SIZE];

	/**
	 * @brief A 1D array where the index is the order which the moves were made. The values are the location of the move.
	 * 
	 */
	int history[SIZE];

	/**
	 * @brief The index of the last move that was made. Before the first move is made the value will be -1.
	 * 
	 */
	int move_count;

	/**
	 * @brief The next player to make a move.
	 * 0 for the first player, 1 for the second player.
	 * It's redundant and can simply be calculated as (move_count + 1) % 2.
	 * 
	 */
	int current_player;
};

struct gameState newState() {
	struct gameState state;
	
	for (int i = 0; i < SIZE; i++) {
		state.board[i] = EMPTY;
		state.history[i] = EMPTY;
	}

	state.move_count = EMPTY;
	state.current_player = 0;

	return state;
}

/**
 * @brief Make a move. This function does not mutate the given state, but returns a new one.
 * 
 * @param state The current state of the game.
 * @param move The index of where the piece is to be placed.
 * @return int The state of the board after the move was made. If the move was invalid, the state will be the same.
 */
struct gameState addMove(struct gameState state, int move) {
	struct gameState new_state = state;

	if (new_state.board[move] == EMPTY) {
		new_state.move_count++;
		new_state.board[move] = new_state.move_count;
		new_state.history[new_state.move_count] = move;
		new_state.current_player = (new_state.current_player + 1) % 2;

		return new_state;
	}

	return state;
}

int getWinner(int board[SIZE]) {
	int position, value, x, o;

	for (int line = 0; line < 8; line++){
		x = 0;
		o = 0;

		for (int i = 0; i < HEIGHT; i++) {
			position = lines[line][i];
			value = board[position];
			
			if (value % 2 == 0) x++;
			else if (value % 2 == 1) o++;
		}

		if (x == 3) return 0; // X won.
		if (0 == 3) return 1; // O won.
	}

	return -1; // There is no winner.
}

int checkWinningLine(int board[SIZE], int line[3], int player) {
	int available = NO_MOVES;
	int position;
	int value;

	for (int i = 0; i < HEIGHT; i++) {
		position = line[i];
		value = board[position];
		
		if (value == EMPTY) {
			// If the spot in the line is the only empty spot found so far:
			if (available == NO_MOVES) available = position;
			// If there's more than one empty spot, then it's not a winning line:
			else return NO_MOVES;
		}

		// If the piece belongs to the opposing player then it's not a winning line:
		else if ((value + 1) % 2 == player) return NO_MOVES;
	}

	return available;
}

/**
 * @brief Check each row, column, and diagonal to see if the given player can win by playing in it.
 * 
 * @param board A 1D array representing the 2D array of the board.
 * @param player The player for which we want to check if he can win.
 * @return int The place where the player needs to play in order to complete or block a line. NO_MOVE (-1) if there is no winning move.
 */
int checkLines(int board[SIZE], int player) {
	// Check if the current player can win:
	for (int i = 0; i < 8; i++) {
		int move = checkWinningLine(board, lines[i], player);
		if (move > NO_MOVES) return move;
	}

	return NO_MOVES;
	
	// int count;
	// int available;

	// // Checking rows:
	// for (int r = 0; r < HEIGHT; r++) {
	// 	count = 0;
	// 	available = -1;

	// 	for (int c = 0; c < WIDTH; c++) {
	// 		int position = r * WIDTH + c;

	// 		// If the spot on the board is empty:
	// 		if (board[position] == EMPTY) {
	// 			available = position;
	// 			continue;
	// 		}

	// 		// If the piece belongs to the current player:
	// 		if (board[position] % 2 == player) count++;

	// 		//If the piece belongs to the other player:
	// 		if ((board[position] + 1) % 2 == player) {
	// 			count = -1;
	// 			break;
	// 		}
	// 	}

	// 	if (count == 2) return available;
	// }

	// // Checking columns:
	// for (int c = 0; c < WIDTH; c++) {
	// 	count = 0;
	// 	available = -1;

	// 	for (int r = 0; r < HEIGHT; r++) {
	// 		int position = r * WIDTH + c;
			
	// 		// If the spot on the board is empty:
	// 		if (board[position] == EMPTY) {
	// 			available = position;
	// 			continue;
	// 		}

	// 		// If the piece belongs to the current player:
	// 		if (board[position] % 2 == player) count++;

	// 		//If the piece belongs to the other player:
	// 		if ((board[position] + 1) % 2 == player) {
	// 			count = -1;
	// 			break;
	// 		}
	// 	}

	// 	if (count == 2) return available;
	// }

	// // TODO Diagonals don't work. Need to be fixed.

	// // Checking diagonals:
	// // First diagonal:
	// count = 0;
	// available = -1;

	// for (int i = 0; i < HEIGHT; i++) {
	// 	int position = i * WIDTH + i;

	// 	// If the spot on the board is empty:
	// 	if (board[position] == EMPTY) {
	// 		available = position;
	// 		continue;
	// 	}

	// 	// If the piece belongs to the current player:
	// 	if (board[position] % 2 == player) count++;

	// 	//If the piece belongs to the other player:
	// 	if ((board[position] + 1) % 2 == player) {
	// 		count = -1;
	// 		break;
	// 	}

	// 	if (count == 2) return available;
	// }

	// // Second diagonal:
	// count = 0;
	// available = -1;

	// for (int i = 0; i < HEIGHT; i++) {
	// 	int position = i * WIDTH + (2 - i);

	// 	// If the spot on the board is empty:
	// 	if (board[position] == EMPTY) {
	// 		available = position;
	// 		continue;
	// 	}

	// 	// If the piece belongs to the current player:
	// 	if (board[position] % 2 == player) count++;

	// 	//If the piece belongs to the other player:
	// 	if ((board[position] + 1) % 2 == player) {
	// 		count = -1;
	// 		break;
	// 	}

	// 	if (count == 2) return available;
	// }

	// return NO_MOVES;
}

int nextMove(struct gameState state) {
	// Check each row, column, and diagonal if the current player can win by playing in them:
	int move = checkLines(state.board, state.current_player);
	printf("Winning move: %s\n", (move > NO_MOVES ? "Yes" : "No"));
	if (move > NO_MOVES) return move;
	
	// Check each row, column, and diagonal if the next player can win by playing in them:
	move = checkLines(state.board, (state.current_player + 1) % 2);
	printf("Blocking move: %s\n", (move > NO_MOVES ? "Yes" : "No"));
	if (move > NO_MOVES) return move;
	
	// Check if the current player can create a fork:
	// Check if the next player can create a fork:

	return NO_MOVES;
}

void printBoard(struct gameState state) {
	printf("\n");
	
	for (int r = 0; r < HEIGHT; r++) {
		for (int c = 0; c < WIDTH; c++) {
			int value = state.board[r * WIDTH + c];
			char label;
			char separator = ' ';
			
			if (value == EMPTY) label = ' ';
			else if (value % 2 == 0) label = 'X';
			else label = 'O';

			if (c < WIDTH - 1) separator = '|';
			printf(" %c %c", label, separator);
		}

		if (r < HEIGHT - 1) printf("\n-----------\n");
		else printf("\n");
	}
}

struct gameState copyBoard(struct gameState state, int board[SIZE]) {
	state.move_count = -1;

	for (int i = 0; i < SIZE; i++) {
		state.board[i] = board[i];

		if (board[i] > EMPTY) {
			state.history[board[i]] = i;

			if (state.move_count < board[i]) state.move_count = board[i];
		}
	}

	state.current_player = (state.move_count + 1) % 2;
	
	return state;
}

int main(int argc, char const *argv[])
{
	struct gameState state = newState();

	int board[9][9] = {
		{
			1, EMPTY, EMPTY,
			EMPTY, 0, EMPTY,
			2, EMPTY, EMPTY
		},
		{
			2, EMPTY, 1,
			EMPTY, 0, EMPTY,
			EMPTY, EMPTY, EMPTY
		},
		{
			1, EMPTY, EMPTY,
			2, 0, EMPTY,
			EMPTY, EMPTY, EMPTY
		},
		{
			1, 2, EMPTY,
			EMPTY, 0, EMPTY,
			EMPTY, EMPTY, EMPTY
		},
		{
			EMPTY, EMPTY, EMPTY,
			1, 0, EMPTY,
			EMPTY, EMPTY, 2
		},
		{
			EMPTY, EMPTY, EMPTY,
			1, 0, EMPTY,
			EMPTY, 2, 3
		},
		{
			EMPTY, EMPTY, EMPTY,
			1, 0, EMPTY,
			3, 2, EMPTY
		},
		{
			EMPTY, EMPTY, EMPTY,
			EMPTY, EMPTY, EMPTY,
			EMPTY, EMPTY, EMPTY
		},
		{
			EMPTY, EMPTY, EMPTY,
			EMPTY, EMPTY, EMPTY,
			EMPTY, EMPTY, EMPTY
		}
	};

	int winner = -1;

	for (int test = 0; test < 7; test++) {
		printf("\n#####################\n");
		printf("TEST %d\n", test + 1);
		printf("#####################\n\n");

		state = copyBoard(state, board[test]);

		while (getWinner(state.board) == -1 && state.move_count < 8) {
			printf("Move %d:\n", state.move_count + 2);
			int position = nextMove(state);

			if (position == NO_MOVES) {
				for (int i = 0; i < SIZE; i++) {
					if (state.board[i] == EMPTY) position = i;
				}
			}

			state = addMove(state, position);

			printBoard(state);
			printf("\n\n");
		}

		switch (getWinner(state.board))
		{
		case 0:
			printf("Winner: X\n");
			break;
		case 1:
			printf("Winner: O\n");
			break;
		default:
			printf("Tie Game\n");
			break;
		}
	}

	// state = addMove(state, 6);
	// printBoard(state);
	// printf("\n\n");

	// state = addMove(state, 3);
	// printBoard(state);
	// printf("\n\n");

	// state = addMove(state, 7);
	// printBoard(state);
	// printf("\n\n");

	// state = addMove(state, 4);
	// printBoard(state);
	// printf("\n\n");

	return 0;
}

/////////////////////////////////////////////////////////////////

// int* initBoard(int width, int height) {
// 	int size = width * height;
// 	int* board = malloc(size * sizeof(int));

// 	if (board == NULL) {
// 		printf("Malloc error in initBoard.");
// 		exit(0);
// 	}

// 	for (int i = 0; i < size; i++) board[i] = EMPTY;

// 	return board;
// }

// // int getNextMove2()

// /**
//  * @brief Get the next move the computer should make
//  * 
//  * @param board A 1D array representing a 2D grid. Values represent the move order.
//  * @param width The width of the board.
//  * @param height The height of the board.
//  * @param player Which player is the current player, for which we are searching for a next move.
//  * @return The array index of where the current player should place his next move.
//  */
// int getNextMove(int* board, int width, int height, int player) {
// 	const int size = width * height;

// 	// Construct move history:
// 	int last_move = EMPTY;
// 	int* history = malloc(sizeof(int) * size);

// 	if (history == NULL) return MALLOC_ERROR;

// 	for (int i = 0; i < size; i++) {
// 		if (board[i] > EMPTY) {
// 			history[board[i]] = i;
// 			if (board[i] > last_move) last_move = board[i];
// 		}
// 	}

// 	// Breadth-first search:
// 	int* virtual_board = malloc(size * sizeof(int));
	
// 	if (virtual_board == NULL) return MALLOC_ERROR;

// 	memcpy(virtual_board, board, size * sizeof(int));

// 	for (int depth = 0; depth < 9; depth++) {
// 		for (int i = 0; i < 9; i++) {
// 			if (virtual_board[i] == EMPTY) {
// 				virtual_board[i] = (last_move + depth);

// 				// Check for victory:
// 				int current_player = (last_move + depth) % 2;

// 				for (int y = 0; y < 3; y++) {
// 					for (int x = 0; x < 3; y++) {
// 						if (virtual_board[y * width + x] % 2 != current_player) break;
// 						else if (x == 2) {
// 							// Victory!
// 						}
// 					}
// 				}
// 			}
// 		}
// 	}
// }