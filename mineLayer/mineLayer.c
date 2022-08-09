#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// #define DEBUG

/**
 * @brief Print the board to the CLI.
 * 
 * @param width The width of the board.
 * @param height The height of the board.
 * @param board A 2D array representing the board.
 */
void printBoard(int width, int height, int* board) {
	for (int y = 0; y < height; y++) {
		char buffer[width + 1];

		for (int x = 0; x < width; x++) {
			buffer[x] = board[y * width + x] == -1 ? 'x' : board[y * width + x] + 48;
		}

		buffer[width] = '\0';

		printf("%s\n", buffer);
	}
}

/**
 * @brief Create a minesweaper board. Each cell will contain either a mine or the number of adjacent mines.
 * 
 * @param width The width of the board.
 * @param height The height of the board.
 * @param mines The number of mines
 * @return A 2D array representing the board.
 */
int* fillBoard(int width, int height, int mines) {
	int* board = calloc(width * height, sizeof(int));

	#ifdef DEBUG
	char separator[width + 4];

	for (int i = 1; i < width + 1; i++) {
		separator[i] = '=';
	}
	separator[0] = '\n';
	separator[width + 1] = '\n';
	separator[width + 2] = '\n';
	separator[width + 3] = '\0';
	#endif

	/*
	-1 means there is a mine.
	0 and above is the number of neighboring mines.
	*/

	for (int i = 0; i < mines; i++) {
		int place = rand() % (width * height);
		/*
		Example:
		w: 7, h: 9, p: 27
		size: 63
		*/
		int y = place / width; // Row: 27 / 7 = 3
		int x = place % width; // Col: 27 % 7 = 6

		if (board[y * width + x] == -1) i--; // If the randomly selected location already has a mine, try again.
		else {
			board[y * width + x] = -1; // Set the randomly selected location to be a mine.
			// Go over all the neighbors, check if they are in bounds and if they're not a mine, and if so increment their neighboring mine count:
			for (int r = y - 1; r <= y + 1; r++) {
				if (r >= 0 && r < height) {
					for (int c = x - 1; c <= x + 1; c++) {
						if (c >= 0 && c < width && board[r * width + c] >= 0) {
							board[r * width + c]++;
						}
					}
				}
			}
		}

		#ifdef DEBUG
		printBoard(width, height, board);
		printf("%s", separator);
		#endif
	}

	return board;
}

int main(int argc, char* argv[]) {
	if (argc < 4) {
		printf("Not enough arguments.\nPlease specify height, width, and number of mines, separated by spaces.\nThe number of mines must be equal to or less than the size of the minefield.\n");
		exit(EXIT_FAILURE);
	}

	int width = atoi(argv[1]);
	int height = atoi(argv[2]);
	int mines = atoi(argv[3]);

	if (width == 0 || height == 0|| mines == 0) {
		printf("The width, height, and number of mines all must be a number greater than 0.\n");
		exit(EXIT_FAILURE);
	}

	if (mines > width * height) {
		printf("The number of mines must be equal to or less than the size of the minefield.\n");
		exit(EXIT_FAILURE);
	}

	srand(time(NULL));
	int* board = fillBoard(width, height, mines);
	printBoard(width, height, board);

	return 0;
}