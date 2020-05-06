#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <ctype.h>
#include <utility>
#include <cstdlib>
#include <ctime>

#include "engine.h"
#include "utils.h"
#include "minimax.h"
#include "debugging.h"

using namespace std;

int start_game() {

	ofstream myfile;
	myfile.open ("received_commands.txt");

	srand(1);

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
	bool white_turn = true;
	bool black_turn = false;
	bool starting_white = true;
	bool starting_black = true;
	bool can_do_castling_left = true;
	bool can_do_castling_right = true;

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
			} else {
				make_move(board, time, otim, play_white);
				swap(white_turn, black_turn);
			}
		
		} else if (input == "white") {
			if (starting_white) {
			white_board(board);
			starting_white = false;
			} else if (play_black) {
				flip_board(board);
			}
			play_white = true;
			play_black = false;
		} else if (input == "black") {
			if (starting_black) {
			black_board(board);
			starting_black = false;
			} else if (play_white) {
				flip_board(board);
			}
			play_white = false;
			play_black = true;
		} else if (input ==  "new") {
			return 1;

		} else if (is_promotion(input)) {
			if (play_white) {
				input = convert_move(input);
			}
			int col = coord_to_col(input.at(0));
			int row = coord_to_row(input.at(1));
			int new_col = coord_to_col(input.at(2));
			int new_row = coord_to_row(input.at(3));
			int piece = 0;
			switch (input.at(4)) {
				case 'q':
					piece = op_queen;
					break;
				case 'b':
					piece = op_bishop;
					break;
				case 'r':
					piece = op_rook;
					break;
				case 'n':
					piece = op_knight;
					break;
			}
			board[new_row][new_col] = piece;
			board[row][col] = 0;
			if (play_white) {
				input = convert_move(input);
			}
			// move(input, board);
			make_move(board, time, otim, play_white);

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
			return 0;
		} else if (input == "new") {
			return 1;
		} else if (input == "white") {
			white_board(board);
			*play_white = true;
			*play_black = false;
		} else if (input == "black") {
			black_board(board);
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

void make_move(int board[12][12], int time, int otim, bool play_white) {
	if (search_legal_move(board).size() == 0) {
		cout << "resign\n";
	}
	string next_move = "qqqq";
	print_board_d(board);
	for (string s : search_legal_move(board)) {
		if (play_white) {
			cout << "# " << convert_move(s) << endl;
		} else {
			cout << "# " << s << endl;
		}
	}
	alpha_beta_negamax(-99999, 99999, 4, board, &next_move);

	if (1) {
		move(next_move, board);
		if (play_white) {
			next_move = convert_move(next_move);
		}
		cout << "move " << next_move << "\n";
		return;
	}

}

// updates the internal board representation
int move(string m, int board[12][12]) {
	int col = coord_to_col(m.at(0));
	int row = coord_to_row(m.at(1));
	int new_col = coord_to_col(m.at(2));
	int new_row = coord_to_row(m.at(3));
	int piece = board[row][col];
	int taken_piece;	

	// castling with right rook
	if (board[row][col] == king && new_col - col == 2) {
		board[new_row][new_col] = king;
		board[new_row][new_col - 1] = rook;
		board[row][col] = 0;
		if (col == 6) {
			board[new_row][new_col + 1] = 0;
		} else {
			board[new_row][new_col + 2] = 0;
		}
		taken_piece = king;
	// castling with left rook
	} else if (board[row][col] == king && col - new_col == 2) {
		board[new_row][new_col] = king;
		board[new_row][new_col + 1] = rook;
		board[row][col] = 0;
		if (col == 6) {
			board[new_row][new_col - 2] = 0;
		} else {
			board[new_row][new_col - 1] = 0;
		}
		taken_piece = king;
	// opponent castling with right rook
	} else if (board[row][col] == op_king && new_col - col == 2) {
		board[new_row][new_col] = op_king;
		board[new_row][new_col - 1] = op_rook;
		board[row][col] = 0;
		if (col == 6) {
			board[new_row][new_col + 1] = 0;
		} else {
			board[new_row][new_col + 2] = 0;
		}
		taken_piece = op_king;
	// opponent castling with left rook
	} else if (board[row][col] == op_king && col - new_col == 2) {
		board[new_row][new_col] = op_king;
		board[new_row][new_col + 1] = op_rook;
		board[row][col] = 0;
		if (col == 6) {
			board[new_row][new_col - 2] = 0;
		} else {
			board[new_row][new_col - 1] = 0;
		}
		taken_piece = op_king;
	// basic move
	} else {
		taken_piece = board[new_row][new_col];
		board[row][col] = 0;
		board[new_row][new_col] = piece;
	}
	return taken_piece;
}

void undo_move(string m, int board[12][12], int taken_piece) {
	int col = coord_to_col(m.at(0));
	int row = coord_to_row(m.at(1));
	int new_col = coord_to_col(m.at(2));
	int new_row = coord_to_row(m.at(3));
	int piece = board[new_row][new_col];
	board[row][col] = piece;
	board[new_row][new_col] = taken_piece;
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

void black_board(int board[12][12]) {
	board[2][5] = queen;
	board[2][6] = king;
	board[9][5] = op_queen;
	board[9][6] = op_king;
}

void white_board(int board[12][12]) {
	board[2][5] = king;
	board[2][6] = queen;
	board[9][5] = op_king;
	board[9][6] = op_queen;
}

void initialize_game(int board[12][12], bool *play_white,
	bool *play_black, bool *white_turn, bool *black_turn) {
	*play_white = false;
	*play_black = true;
	*white_turn = true;
	*black_turn = false;
}