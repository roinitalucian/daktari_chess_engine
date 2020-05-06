#ifndef UTILS_H
#define UTILS_H

#include "engine.h"

using namespace std;

int coord_to_col(char c);
int coord_to_row(char c);
char col_to_coord(char c);
char row_to_coord(char c);

string convert_move(string m);

string create_move(int col, int row, int new_col, int new_row);

bool is_move(string input);
bool is_promotion(string input);
bool my_turn(bool white_turn, bool black_turn, bool play_white, bool play_black);

pair<int, int> get_king_coords(int board[12][12]);
pair<int, int> get_op_king_coords(int board[12][12]);
bool is_check (int board[12][12]); 

#endif