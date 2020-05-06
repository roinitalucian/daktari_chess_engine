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


// returns 1 if replay is active
int start_game();


// the first function that should be called inside start_game()
void initialize_game(int board[12][12], bool *play_white,
	bool *play_black, bool *white_turn, bool *black_turn);


// puts the engine into force mode
// returns 1 if "new" is given during it
// returns 0 for "go"
int force_mode(int board[12][12], bool *play_white, bool *play_black,
	bool *white_turn, bool *black_turn);


// the engine starts computing its next move
// calls the negamax function
void make_move(int board[12][12], int time, int otim, bool play_white);


// updates the internal representation of the board
int move(string m, int board[12][12]);


// undoes the changes made by move()
void undo_move(string m, int board[12][12], int taken_piece);


// turns the board 180 degrees
void flip_board(int board[12][12]);


// initializes the board for playing white
// only called at the begginig of the game
void white_board(int board[12][12]);


// same but for playing white
void black_board(int board[12][12]);


#endif