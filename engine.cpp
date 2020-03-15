#include <iostream>
#include <fstream>
#include <string>

#include "engine.h"
#include "utils.h"

using namespace std;

void start_game() {

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
		myfile << time << " " << otim;
		if (input.substr(0, 4) == "time") {
			time = stoi(input.substr(4));
		} else if (input.substr(0, 4) == "otim") {
			otim = stoi(input.substr(4));
		} else if (input == "force") {
			// TODO

		} else if (input == "go") {
			// TODO

		} else if (input == "black") {
			// TODO

		} else if (input == "white") {
			// TODO

		} else if (input == "random") {
			continue;
		} else if (input.substr(0, 5) == "level") {
			continue;
		} else if (input == "hard") {
			continue;
		} else if (input == "post") {
			continue;
		} else {
			// if (input == "b2b4") {
			// 	cout << "move b7b6\n";
			// } else if (input == "h2h3") {
			// 	cout << "move g8f6\n";
			// } else if (input == "g2g4") {
			// 	cout << "move d7d6\n";
			// }
			move(input, board);
			make_move(board, time, otim);
		}

	}

    myfile.close();
}

void make_move(int board[12][12], int time, int otim) {
	string next_move;
	// it work just for the pawns atm
	for (int i = 2; i < 10; i++) {
		for(int j = 2; j < 10; j++) {
			if (board[i][j] == pawn) {
				next_move = search_legal_move(board, i, j, pawn);
				if (next_move != "") {
					move(next_move, board);
					cout << "move " << next_move << "\n";
					return;
				}
			}
		}

	}
}

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

void move(string m, int board[12][12]) {
	int col = coord_to_col(m.at(0));
	int row = coord_to_row(m.at(1));
	int new_col = coord_to_col(m.at(2));
	int new_row = coord_to_row(m.at(3));
	int piece = board[row][col];
	board[row][col] = 0;
	board[new_row][new_col] = piece;
}

void flip_board(int board[12][12]) {
	int aux;
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 12; j++) {
			aux = board[i][j];
			board[i][j] = board[11 - i][j];
			board[11 - i][j] = aux;
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
	// board[12][12] = {
	// 	{7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7},
	// 	{7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7},
	// 	{7, 7,-4,-2,-3,-5,-6,-3,-2,-4, 7, 7},
	// 	{7, 7,-1,-1,-1,-1,-1,-1,-1,-1, 7, 7},
	// 	{7, 7, 0, 0, 0, 0, 0, 0, 0, 0, 7, 7},
	// 	{7, 7, 0, 0, 0, 0, 0, 0, 0, 0, 7, 7},
	// 	{7, 7, 0, 0, 0, 0, 0, 0, 0, 0, 7, 7},
	// 	{7, 7, 0, 0, 0, 0, 0, 0, 0, 0, 7, 7},
	// 	{7, 7, 1, 1, 1, 1, 1, 1, 1, 1, 7, 7},
	// 	{7, 7, 4, 2, 3, 5, 6, 3, 2, 4, 7, 7},
	// 	{7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7},
	// 	{7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7}
	// };

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