#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "common.h"
#include "simple.h"

void printBoard(struct gameState state) {
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

int main(int argc, char const *argv[]) {
	struct gameState state = newState();

	printf("\n");
	
	while (getWinner(state.board) == NO_WINNER && state.move_count < 8) {
		state = addMove(state, nextMove(state));

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

	printf("\n");

	return 0;
}