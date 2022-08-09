#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define SIZE 9
#define WIDTH 3
#define HEIGHT 3
#define EMPTY -1
#define NO_MOVE -1
#define NO_MOVES -1
#define NO_WINNER -1
#define PLAYER_X 0
#define PLAYER_O 1

/**
 * @brief All the different winning formations, in the form of arrays of indices.
 * 
 */
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
};

struct gameState newState() {
	struct gameState state;
	
	for (int i = 0; i < SIZE; i++) {
		state.board[i] = EMPTY;
		state.history[i] = EMPTY;
	}

	state.move_count = EMPTY;

	return state;
}

/**
 * @brief Calculates the current player.
 * 
 * @param state The game.
 * @return int 0 is the first player (player X). 1 is the second player (player O).
 */
int currentPlayer(struct gameState state) {
	return (state.move_count + 1) % 2;
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

	return NO_WINNER; // There is no winner.
}

int checkWinningLine(int board[SIZE], int line[3], int player) {
	int available = NO_MOVE;
	int position;
	int value;

	for (int i = 0; i < HEIGHT; i++) {
		position = line[i];
		value = board[position];
		
		if (value == EMPTY) {
			// If the spot in the line is the only empty spot found so far:
			if (available == NO_MOVE) available = position;
			// If there's more than one empty spot, then it's not a winning line:
			else return NO_MOVE;
		}

		// If the piece belongs to the opposing player then it's not a winning line:
		else if ((value + 1) % 2 == player) return NO_MOVE;
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
		if (move > NO_MOVE) return move;
	}

	return NO_MOVE;
}

/**
 * @brief Find a location where player can play to create a fork, a situation where he has 2 options for victory in the next move.
 * 
 * @param board The current board.
 * @param player The player for which we are checking.
 * @return int The index in the board where the move can be played, or NO_MOVE (-1) if there is no possibility of a fork.
 */
int checkForks(int board[SIZE], int player) {
	// For each line, check if it has exactly 1 friendly piece and no enemy pieces.
	int options[8], count = 0, position1, position2, value, friendly, empty;

	for (int line = 0; line < 8; line++) {
		friendly = 0;
		empty = 0;

		for (int i = 0; i < HEIGHT; i++) {
			position1 = lines[line][i];
			value = board[position1];

			if (value == player) friendly++;
			else if (value == EMPTY) empty++;
			else break;
		}

		if (friendly == 1 && empty == 2) {
			options[count] = line;
			count++;
		}
	}

	// For every 2 lines in options, check if their intersection point is empty:
	// The maximum number of comparisons is 8 * 7 = 56
	for (int line1 = 0; line1 < count; line1++) {
		for (int line2 = 0; line2 < count; line2++) {
			for (int i = 0; i < HEIGHT; i++) {
				for (int j = 0; j < HEIGHT; j++) {
					position1 = lines[line1][i];
					position2 = lines[line2][j];

					value = board[position1];

					if (line1 != line2 && position1 == position2 && value == EMPTY) return position1;
				}
			}
		}
	}

	return NO_MOVE;
}

/**
 * @brief Get the next move for a CPU player.
 * 
 * @param state The game.
 * @return int The index in the board array where the next move should be played.
 */
int nextMove(struct gameState state) {
	// Check where the current player can win in the next move:
	int move = checkLines(state.board, currentPlayer(state));
	printf("Winning move: %s\n", (move > NO_MOVE ? "Yes" : "No"));
	if (move > NO_MOVE) return move;
	
	// Check where the current player needs to move to block the other player from winning in the next turn:
	move = checkLines(state.board, (currentPlayer(state) + 1) % 2);
	printf("Blocking move: %s\n", (move > NO_MOVE ? "Yes" : "No"));
	if (move > NO_MOVE) return move;
	
	// Check where the current player can play to have certain victory in 2 turns:
	move = checkForks(state.board, currentPlayer(state));
	printf("Creating fork: %s\n", (move > NO_MOVE ? "Yes" : "No"));
	if (move > NO_MOVE) return move;
	
	// Check where the current player needs to move to block the other player from certain victory in 2 turns:
	move = checkForks(state.board, (currentPlayer(state) + 1) % 2);
	printf("Blocking fork: %s\n", (move > NO_MOVE ? "Yes" : "No"));
	if (move > NO_MOVE) return move;

	return NO_MOVE;
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
	state.move_count = NO_MOVES;

	for (int i = 0; i < SIZE; i++) {
		state.board[i] = board[i];

		if (board[i] > EMPTY) {
			state.history[board[i]] = i;

			if (state.move_count < board[i]) state.move_count = board[i];
		}
	}
	
	return state;
}

int main(int argc, char const *argv[])
{
	struct gameState state = newState();

	int tests[9][9] = {
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

	int winner = NO_WINNER;

	for (int test = 0; test < 7; test++) {
		printf("\n#####################\n");
		printf("TEST %d\n", test + 1);
		printf("#####################\n\n");

		state = copyBoard(state, tests[test]);

		while (getWinner(state.board) == NO_WINNER && state.move_count < 8) {
			printf("Move %d:\n", state.move_count + 2);
			int position = nextMove(state);

			if (position == NO_MOVE) {
				for (int i = 0; i < SIZE; i++) {
					if (state.board[i] == EMPTY) position = i;
				}
			}

			state = addMove(state, position);

			printBoard(state);
			printf("\n\n");
		}

		switch (getWinner(state.board)) {
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

	return 0;
}