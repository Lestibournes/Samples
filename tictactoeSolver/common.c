#include "common.h"

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

struct gameState newState() {
	struct gameState state;
	
	for (int i = 0; i < SIZE; i++) {
		state.board[i] = EMPTY;
		state.history[i] = EMPTY;
	}

	state.move_count = EMPTY;

	return state;
}

int currentPlayer(struct gameState state) {
	return (state.move_count + 1) % 2;
}

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