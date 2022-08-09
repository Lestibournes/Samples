#ifndef SIMPLE
#define SIMPLE

#include "common.h"

int checkWinningLine(int board[SIZE], int line[3], int player);

/**
 * @brief Check each row, column, and diagonal to see if the given player can win by playing in it.
 * 
 * @param board A 1D array representing the 2D array of the board.
 * @param player The player for which we want to check if he can win.
 * @return int The place where the player needs to play in order to complete or block a line. NO_MOVE (-1) if there is no winning move.
 */
int checkLines(int board[SIZE], int player);

/**
 * @brief Find a location where player can play to create a fork, a situation where he has 2 options for victory in the next move.
 * 
 * @param board The current board.
 * @param player The player for which we are checking.
 * @return int The index in the board where the move can be played, or NO_MOVE (-1) if there is no possibility of a fork.
 */
int checkForks(int board[SIZE], int player);

/**
 * @brief Get the next move for a CPU player.
 * 
 * @param state The game.
 * @return int The index in the board array where the next move should be played.
 */
int nextMove(struct gameState state);

#endif