#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <ctype.h>

#include "engine.h"
#include "utils.h"

using namespace std;

int start_game() {

	ofstream myfile;
	myfile.open ("received_commands.txt");

	int board[12][12] = {
		{7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7},
		{7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7},
		{7, 7, 4, 2, 3, 5, 6, 3, 2, 4, 7, 7},
		{7, 7, 1, 1, 1, 1, 1, 1, 1, 1, 7, 7},
		{7, 7, 0, 0, 0, 0, 0, 0, 0, 0, 7, 7},
		{7, 7, 0, 0, 0, 0, 0, 0, 0, 0, 7, 7},
		{7, 7, 0, 0, 0, 0, 0, 0, 0, 0, 7, 7},
		{7, 7, 0, 0, 0, 0, 0, 0, 0, 0, 7, 7},
		{7, 7,-1,-1,-1,-1,-1,-1,-1,-1, 7, 7},
		{7, 7,-4,-2,-3,-5,-6,-3,-2,-4, 7, 7},
		{7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7},
		{7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7}
	};

	string input;

	bool play_white;
	bool play_black;
	bool white_turn;
	bool black_turn;

	int time, otim;


	initialize_game(board, &play_white, &play_black, &white_turn, &black_turn);


	while (1) {

		print_board(board, myfile);


		getline(cin, input);

		myfile << input << endl;
		
		if (input.substr(0, 4) == "time") {
			time = stoi(input.substr(4));
		} else if (input.substr(0, 4) == "otim") {
			otim = stoi(input.substr(4));
		} else if (input == "force") {
			if (force_mode(board, &play_white, &play_black,
				&white_turn, &black_turn)) {
				return 1;		// returns 1 if "new" command is given during
			} else {			// force mode
				if (my_turn(white_turn, black_turn, play_white, play_black)) {
					make_move(board, time, otim, play_white);
					swap(white_turn, black_turn);
				}
			}

		} else if (input == "go") {
			if(!my_turn(white_turn, black_turn, play_white, play_black)) {
				myfile << "SE FACE SCHIMBAREA" << endl;
				flip_board(board);
				swap(play_white, play_black);
				swap(time, otim);				
			
				make_move(board, time, otim, play_white);
				swap(white_turn, black_turn);
			}
		
		} else if (input ==  "new") {
			return 1;

		} else if (is_move (input)){
			if (play_white) {
				input = convert_move(input);
			}
			move(input, board);
			make_move(board, time, otim, play_white);
			// swap(white_turn, black_turn);
		}
		myfile << "WHITE_TURN: " << white_turn << " | BLACK_TURN: " << black_turn <<
		" | PLAY_WHITE: " << play_white << " | PLAY_BLACK: " << play_black << endl;

	}

    myfile.close();
    return 0;
}

int force_mode(int board[12][12], bool *play_white, bool *play_black,
	bool *white_turn, bool *black_turn) {
	string input;
	
	while (1) {
		getline(cin, input);

		if (input == "go") {
			if (*play_white) {
				*white_turn = true;
				*black_turn = false;
			} else {
				*white_turn = false;
				*black_turn = true;
			}
			// flip_board(board);
			return 0;
		} else if (input == "new") {
			return 1;
		} else if (input == "white") {
			*play_white = true;
			*play_black = false;
		} else if (input == "black") {
			*play_white = false;
			*play_black = true;
		} else if (is_move(input)) {
			if (*play_white) {
				input = convert_move(input);
			}
			move(input, board);
			swap(*white_turn, *black_turn);
		}
	}
	return 0;
}

// outputs the move to xboard
void make_move(int board[12][12], int time, int otim, bool play_white) {
	string next_move;
	// it work just for the pawns atm
	for (int i = 2; i < 10; i++) {
		for(int j = 2; j < 10; j++) {
			if (board[i][j] == pawn) {
				next_move = search_legal_move(board, i, j, pawn);
				if (next_move != "") {
					move(next_move, board);
					if (play_white) {
						next_move = convert_move(next_move);
					}
					cout << "move " << next_move << "\n";
					return;
				}
			}
		}
	}
	cout << "resign\n";
}

// searches all legal moves a piece can make from a given position
string search_legal_move(int board[12][12], int row, int col, int piece) {
	string move = "qqqq";
	int new_row;
	int new_col;
	switch (piece) {
		// search for legal pawn moves
		case pawn:
			if (board[row + 1][col] == empty) {
				new_row = row + 1;
				new_col = col;
			} else if (board[row + 1][col + 1] < 0 && board[row + 1][col + 1] != rim) {
				new_row = row + 1;
				new_col = col + 1;
			} else if (board[row + 1][col - 1] < 0 && board[row + 1][col - 1] != rim) {
				new_row = row + 1;
				new_col = col - 1;
			} else {
				return "";
			}
		break;
		default:
			return "";
			break;
	}

	move.at(0) = col_to_coord(col);
	move.at(1) = row_to_coord(row);
	move.at(2) = col_to_coord(new_col);
	move.at(3) = row_to_coord(new_row);

	return move;
}

// updates the internal board representation
void move(string m, int board[12][12]) {
	int col = coord_to_col(m.at(0));
	int row = coord_to_row(m.at(1));
	int new_col = coord_to_col(m.at(2));
	int new_row = coord_to_row(m.at(3));
	int piece = board[row][col];
	board[row][col] = 0;
	board[new_row][new_col] = piece;
}

// turns the table
void flip_board(int board[12][12]) {
	int aux;
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 12; j++) {
			swap(board[i][j], board[11 - i][11 - j]);
		}
	}

	for (int i = 2; i < 10; i++) {
		for(int j = 2; j < 10; j++) {
			board[i][j] = -board[i][j];
		}
	}
}

void initialize_game(int board[12][12], bool *play_white,
	bool *play_black, bool *white_turn, bool *black_turn) {
	*play_white = false;
	*play_black = true;
	*white_turn = true;
	*black_turn = false;
}

/**
	for debugging
*/
void print_board(int board[12][12], ofstream& f) {
	f << endl;
	for (int i = 2; i < 10; i++) {
		for(int j = 2; j < 10; j++) {
			if (board[i][j] >= 0)
				f << board[i][j] << " | ";
			else
				f << board[i][j] << "| ";
		}
		f << endl << "-------------------------------" << endl;
	}
}