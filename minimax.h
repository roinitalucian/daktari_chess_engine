#ifndef MINIMAX_H
#define MINIMAX_H

using namespace std;

// returns a list containing all the legal moves
// board has positive integers for the player at turn
vector<string> search_legal_move(int board[12][12]);


// used inside search_legal_move to add simple moves for king and knight
void add_simple_move (vector<string> &moves, int board[12][12], int row, int col,
	int row_mod, int col_mod);
void add_king_move (vector<string> &moves, int board[12][12], int row, int col,
	int row_mod, int col_mod);


// minimax algorithm
// uses alpha-beta pruning
// set depth to the desired level of analysis (3-5 recommended)
int alpha_beta_negamax(int alpha, int beta, int depth, int board[12][12], string *m);


// the returned value should be proportional to the chance of winning
// the game at that certain state
int evaluate_move(int board[12][12]);


// return true when the game ends (mate or concede)
bool game_over(int board[12][12]);

#endif