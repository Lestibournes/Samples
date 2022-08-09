#ifndef COMMON
#define COMMON

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
extern int lines[8][3];

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

struct gameState newState();
/**
 * @brief Calculates the current player.
 * 
 * @param state The game.
 * @return int 0 is the first player (player X). 1 is the second player (player O).
 */
int currentPlayer(struct gameState state);

/**
 * @brief Make a move. This function does not mutate the given state, but returns a new one.
 * 
 * @param state The current state of the game.
 * @param move The index of where the piece is to be placed.
 * @return int The state of the board after the move was made. If the move was invalid, the state will be the same.
 */
struct gameState addMove(struct gameState state, int move);

/**
 * @brief Get the Winner of the game.
 * 
 * @param board The current state of the game.
 * @return int The player in the winning position, which can be PLAYER_X, PLAYER_O, or NO_WINNER if there isn't currently a winner.
 */
int getWinner(int board[SIZE]);

#endif