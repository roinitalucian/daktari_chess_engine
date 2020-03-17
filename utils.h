#ifndef UTILS_H
#define UTILS_H

using namespace std;

int coord_to_col(char c);
int coord_to_row(char c);
char col_to_coord(char c);
char row_to_coord(char c);

string convert_move(string m);

bool is_move(string input);
bool my_turn(bool white_turn, bool black_turn, bool play_white, bool play_black);

#endif