#ifndef ENGINE_H
#define ENGINE_H

#include <vector>
#include <utility>

using namespace std;

const int pawn = 1;
const int knight = 2;
const int bishop = 3;
const int rook = 4;
const int queen = 5;
const int king = 6;

const int op_pawn = -1;
const int op_knight = -2;
const int op_bishop = -3;
const int op_rook = -4;
const int op_queen = -5;
const int op_king = -6;

const int rim = 7;
const int empty = 0;

int start_game(); // returns 1 if replay is active
void flip_board(int board[12][12]);
int move(string m, int board[12][12]);
void undo_move(string m, int board[12][12], int taken_piece);
void initialize_game(int board[12][12], bool *play_white,
	bool *play_black, bool *white_turn, bool *black_turn);
pair<int, int> get_king_coords(int board[12][12]);
pair<int, int> get_op_king_coords(int board[12][12]);
bool is_check (int board[12][12]); 
void add_simple_move (vector<string> &moves, int board[12][12], int row, int col,
	int row_mod, int col_mod);
void add_king_move (vector<string> &moves, int board[12][12], int row, int col,
	int row_mod, int col_mod);
void make_move(int board[12][12], int time, int otim, bool play_white);
int force_mode(int board[12][12], bool *play_white, bool *play_black,
	bool *white_turn, bool *black_turn);
vector<string> search_legal_move(int board[12][12]);


int alpha_beta_negamax(int alpha, int beta, int depth, int board[12][12], string *m);
int evaluate_move(int board[12][12]);
bool game_over(int board[12][12]);


void print_board(int board[12][12], ofstream& f);
void print_board_d(int board[12][12]);

#endif