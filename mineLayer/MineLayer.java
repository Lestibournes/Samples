package mineLayer;

import java.util.Random;

public class MineLayer {
	static final int FULL = -1;

	/**
	 * Print the board to the CLI.
	 * 
	 * @param width The width of the board.
	 * @param height The height of the board.
	 * @param board A 2D array representing the board.
	 */
	public static void printBoard(int width, int height, int[] board) {
		StringBuilder sb = new StringBuilder();

		// Column numbers:
		sb.append("  |");

		for (int i = 0; i < width; i++) {
			sb.append(" ");
			sb.append(Character.toChars(65 + i));
			sb.append(" |");
		}
		
		sb.append("\n");
		
		// Separator:
		sb.append("--");

		for (int i = 0; i < width * 4; i++) {
			sb.append("-");
		}

		sb.append("\n");

		for (int y = 0; y < height; y++) {
			sb.append(y + " |"); //Row number

			for (int x = 0; x < width; x++) {
				int value = board[y * width + x];
				sb.append(" ");
				sb.append(value == FULL ? "X" : String.valueOf(value));
				sb.append(" |");
			}

			sb.append("\n");

			// Separator:
			sb.append("--");

			for (int i = 0; i < width * 4; i++) {
				sb.append("-");
			}

			sb.append("\n");
		}

		System.out.println(sb);
	}

	/**
	 * Create a minesweaper board. Each cell will contain either a mine or the number of adjacent mines.
	 * @param width The width of the board.
	 * @param height The height of the board.
	 * @param mines The number of mines
	 * @return A 2D array representing the board.
	 */
	public static int[] fillBoard(int width, int height, int mines) {
		int[] board = new int[width * height];
		Random random = new Random();

		for (int i = 0; i < mines; i++) {
			int place = random.nextInt(board.length);
			int y = place / width;
			int x = place % width;

			if (board[place] == FULL) i--;
			else {
				board[place] = FULL;

				for (int r = y - 1; r <= y + 1; r++) {
					if (r >= 0 && r < height) {
						for (int c = x - 1; c <= x + 1; c++) {
							if (c >= 0 && c < width && board[r * width + c] != FULL)
								board[r * width + c]++;
						}
					}
				}
			}
		}

		return board;
	}

	public static void main(String[] args) {
		if (args.length < 3) {
			System.out.println("Not enough arguments.\nPlease specify height, width, and number of mines, separated by spaces.\nThe number of mines must be equal to or less than the size of the minefield.\n");
			System.exit(1);
		}
	
		int width = Integer.parseInt(args[0]);
		int height = Integer.parseInt(args[1]);
		int mines = Integer.parseInt(args[2]);
	
		if (width == 0 || height == 0|| mines == 0) {
			System.out.println("The width, height, and number of mines all must be a number greater than 0.\n");
			System.exit(1);
		}
	
		if (mines > width * height) {
			System.out.println("The number of mines must be equal to or less than the size of the minefield.\n");
			System.exit(1);
		}
	
		int[] board = fillBoard(width, height, mines);
		printBoard(width, height, board);
	}
}