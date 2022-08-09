#include "simple.h"

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

int checkLines(int board[SIZE], int player) {
	// Check if the current player can win:
	for (int i = 0; i < 8; i++) {
		int move = checkWinningLine(board, lines[i], player);
		if (move > NO_MOVE) return move;
	}

	return NO_MOVE;
}

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

int nextMove(struct gameState state) {
	// Check where the current player can win in the next move:
	int move = checkLines(state.board, currentPlayer(state));
	if (move > NO_MOVE) return move;
	
	// Check where the current player needs to move to block the other player from winning in the next turn:
	move = checkLines(state.board, (currentPlayer(state) + 1) % 2);
	if (move > NO_MOVE) return move;
	
	// Check where the current player can play to have certain victory in 2 turns:
	move = checkForks(state.board, currentPlayer(state));
	if (move > NO_MOVE) return move;
	
	// Check where the current player needs to move to block the other player from certain victory in 2 turns:
	move = checkForks(state.board, (currentPlayer(state) + 1) % 2);
	if (move > NO_MOVE) return move;

	// Otherwise, move to the first available spot, with priority to center, then corners, then sides:
	// 012
	// 345
	// 678
	if (state.board[4] == EMPTY) return 4;

	// 0,2,4,6,8
	for (int i = 0; i < 9; i += 2) if (state.board[i] == EMPTY) return i;

	// 1,3,5,7
	for (int i = 1; i < 9; i += 2) if (state.board[i] == EMPTY) return i;

	// This should only happen when attempting to make a move in a full board:
	return NO_MOVE;
}