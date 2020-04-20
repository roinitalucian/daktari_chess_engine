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

using namespace std;

int start_game() {

	ofstream myfile;
	myfile.open ("received_commands.txt");

	srand(time(NULL));

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

int evaluate_move(int board[12][12]) {
	return rand() % 15 + 1;
}

bool game_over(int board[12][12]) {
	return false;
}

int alpha_beta_negamax(int alpha, int beta, int depth, int board[12][12], string* m) {
	if (depth == 0 || game_over(board)) {
		return evaluate_move(board);
	} 

	int max = -9999999;

	for (string next_move : search_legal_move(board)) {
		int taken_piece = move(next_move, board);

		int score = -alpha_beta_negamax(-beta, -alpha, depth - 1, board, m);

		if (score >= max) {
			max = score;
		}

		if (max > alpha) {
			alpha = max;
			*m = next_move;
		}

		if (alpha >= beta) {
			break;
		}

		undo_move(next_move, board, taken_piece);
	}

	return alpha;
}

void make_move(int board[12][12], int time, int otim, bool play_white) {
	string next_move = "qqqq";
	print_board_d(board);
	for (string s : search_legal_move(board)) {
		if (play_white) {
			cout << "# " << convert_move(s) << endl;
		} else {
			cout << "# " << s << endl;
		}
	}
	cout <<"# "<< search_legal_move(board)[0]<<endl;
	alpha_beta_negamax(-99999, 99999, 1, board, &next_move);

	if (1) {
		move(next_move, board);
		if (play_white) {
			next_move = convert_move(next_move);
		}
		cout << "move " << next_move << "\n";
		return;
	}

}

// // outputs the move to xboard
// void make_move(int board[12][12], int time, int otim, bool play_white) {
// 	string next_move;
// 	// it work just for the pawns atm
// 	for (int i = 2; i < 10; i++) {
// 		for(int j = 2; j < 10; j++) {
// 			if (board[i][j] > 0 && board[i][j] != rim) {
// 				// to be modified
// 				next_move = search_legal_move(board, i, j, board[i][j])
// 				[0];
// 				if (next_move != "") {
// 					move(next_move, board);
// 					if (play_white) {
// 						next_move = convert_move(next_move);
// 					}
// 					cout << "move " << next_move << "\n";
// 					return;
// 				}
// 			}
// 		}
// 	}
// 	cout << "resign\n";
// }

pair<int, int> get_king_coords(int board[12][12]) {
	for (int i = 2; i < 10; i++) {
		for (int j = 2; j < 10; j++) {
			if (board[i][j] == king) {
				return make_pair(i, j);
			}
		}
	}
}

pair<int, int> get_op_king_coords(int board[12][12]) {
	for (int i = 2; i < 10; i++) {
		for (int j = 2; j < 10; j++) {
			if (board[i][j] == op_king) {
				return make_pair(i, j);
			}
		}
	}
}

bool is_check (int board[12][12]) {
	int row = get_king_coords(board).first;
	int col = get_king_coords(board).second;
	for (int i = row - 1, j = col - 1; i > 1 && j > 1; i--, j--) {
		if ((i - j) == (row - col)) {
			if (board[i][j] == op_bishop || board[i][j] == op_queen) {
				return true;
			} else if (board[i][j] > 0) {
				break;
			}
		}
	}
	for (int i = row + 1, j = col + 1; i < 10 && j < 10; i++, j++) {
		if ((i - j) == (row - col)) {
			if (board[i][j] == op_bishop || board[i][j] == op_queen) {
				return true;
			} else if (board[i][j] > 0) {
				break;
			}
		}
	}
	for (int i = row - 1, j = col + 1; i > 1 && j < 10; i--, j++) {
		if ((i + j) == (row + col)) {
			if (board[i][j] == op_bishop || board[i][j] == op_queen) {
				return true;
			} else if (board[i][j] > 0) {
				break;
			}
		}
	}
	for (int i = row + 1, j = col - 1; i < 10 && j > 1; i++, j--) {
		if ((i + j) == (row + col)) {
			if (board[i][j] == op_bishop || board[i][j] == op_queen) {
				return true;
			} else if (board[i][j] > 0) {
				break;
			}
		}
	}

	for (int i = col + 1; i < 10; i++) {
		if (board[row][i] == op_rook || board[row][i] == op_queen) {
			return true;
		} else if (board[row][i] > 0) {
			break;
		}
	}
	for (int i = col - 1; i > 1; i--) {
		if (board[row][i] == op_rook || board[row][i] == op_queen) {
			return true;
		} else if (board[row][i] > 0) {
			break;
		}
	}
	for (int i = row + 1; i < 10; i++) {
		if (board[i][col] == op_rook ||board[i][col] == op_queen) {
			return true;
		} else if (board[i][col] > 0) {
			break;
		}
	}
	for (int i = row - 1; i > 1; i--) {
		if (board[i][col] == op_rook ||board[i][col] == op_queen) {
			return true;
		} else if (board[i][col] > 0) {
			break;
		}
	}

	if (board[row + 1][col - 1] == op_pawn || board[row + 1][col + 1] == op_pawn) {
		return true;
	}

	if (board[row + 2][col + 1] == op_knight || board[row + 2][col - 1] == op_knight ||
		board[row + 1][col + 2] == op_knight || board[row + 1][col - 2] == op_knight ||
		board[row - 2][col + 1] == op_knight || board[row - 2][col - 1] == op_knight ||
		board[row - 1][col + 2] == op_knight || board[row - 1][col - 2] == op_knight) {
		return true;
	}

	return false;
}

// moves for king and knight
void add_simple_move (vector<string> &moves, int board[12][12], int row, int col,
	int row_mod, int col_mod) {
	int new_row, new_col;
	if (board[row + row_mod][col + col_mod] <= 0) {
		new_row = row + row_mod;
		new_col = col + col_mod;
		moves.push_back(create_move(col, row, new_col, new_row));
	}
}

void add_king_move (vector<string> &moves, int board[12][12], int row, int col,
	int row_mod, int col_mod) {
	int x = get_op_king_coords(board).first;
	int y = get_op_king_coords(board).second;
	int new_row, new_col;
	if (board[row + row_mod][col + col_mod] <= 0) {
		new_row = row + row_mod;
		new_col = col + col_mod;
		if (!(new_row == x + 1 && new_col == y + 1 ||
			new_row == x + 1 && new_col == y ||
			new_row == x + 1 && new_col == y - 1 ||
			new_row == x - 1 && new_col == y + 1 ||
			new_row == x - 1 && new_col == y ||
			new_row == x - 1 && new_col == y - 1 ||
			new_row == x && new_col == y - 1 ||
			new_row == x && new_col == y + 1))
		moves.push_back(create_move(col, row, new_col, new_row));
	}
}


// searches all legal moves a piece can make from a given position
vector<string> search_legal_move(int board[12][12]) {
	vector<string> moves;
	int new_row;
	int new_col;
	for (int row = 2; row < 10; row++) {
		for(int col = 2; col < 10; col++) {
			if (board[row][col] > 0 && board[row][col] != rim) {
	switch (board[row][col]) {
		case pawn:
			if (row == 2 && board[row + 2][col] == empty) {
				new_row = row + 2;
				new_col = col;
				moves.push_back(create_move(col, row, new_col, new_row));
			}
			if (board[row + 1][col] == empty) {
				new_row = row + 1;
				new_col = col;
				moves.push_back(create_move(col, row, new_col, new_row));
			}
			if (board[row + 1][col + 1] < 0 && board[row + 1][col + 1] != rim) {
				new_row = row + 1;
				new_col = col + 1;
				moves.push_back(create_move(col, row, new_col, new_row));
			}
			if (board[row + 1][col - 1] < 0 && board[row + 1][col - 1] != rim) {
				new_row = row + 1;
				new_col = col - 1;
				moves.push_back(create_move(col, row, new_col, new_row));
			}
		break;

		case bishop:
			// make main diagonal moves
			for (int i = row - 1, j = col - 1; i > 1 && j > 1; i--, j--) {
				if ((i - j) == (row - col)) {
					if (board[i][j] == 0) {
						moves.push_back(create_move(col, row, j, i));
					} else if (board[i][j] < 0) {
						moves.push_back(create_move(col, row, j, i));
						break;
					} else if (board[i][j] > 0 && board[i][j] != rim) {
						break;
					}
				}
			}
			for (int i = row + 1, j = col + 1; i < 10 && j < 10; i++, j++) {
				if ((i - j) == (row - col)) {
					if (board[i][j] == 0) {
						moves.push_back(create_move(col, row, j, i));
					} else if (board[i][j] < 0) {
						moves.push_back(create_move(col, row, j, i));
						break;
					} else if (board[i][j] > 0 && board[i][j] != rim) {
						break;
					}
				}
			}
			// make secondary diagonal moves
			for (int i = row - 1, j = col + 1; i > 1 && j < 10; i--, j++) {
				if ((i + j) == (row + col)) {
					if (board[i][j] == 0) {
						moves.push_back(create_move(col, row, j, i));
					} else if (board[i][j] < 0) {
						moves.push_back(create_move(col, row, j, i));
						break;
					} else if (board[i][j] > 0 && board[i][j] != rim) {
						break;
					}
				}
			}
			for (int i = row + 1, j = col - 1; i < 10 && j > 1; i++, j--) {
				if ((i + j) == (row + col)) {
					if (board[i][j] == 0) {
						moves.push_back(create_move(col, row, j, i));
					} else if (board[i][j] < 0) {
						moves.push_back(create_move(col, row, j, i));
						break;
					} else if (board[i][j] > 0 && board[i][j] != rim) {
						break;
					}
				}
			}
			
		break;

		case rook:
			// make column moves
			for (int i = col + 1; i < 10; i++) {
				if (board[row][i] == 0) {
					moves.push_back(create_move(col, row, i, row));
				} else if (board[row][i] < 0) {
					moves.push_back(create_move(col, row, i, row));
					break;
				} else if (board[row][i] > 0) {
					break;
				}
			}
			for (int i = col - 1; i > 1; i--) {
				if (board[row][i] == 0) {
					moves.push_back(create_move(col, row, i, row));
				} else if (board[row][i] < 0) {
					moves.push_back(create_move(col, row, i, row));
					break;
				} else if (board[row][i] > 0) {
					break;
				}
			}
			// make row moves
			for (int i = row + 1; i < 10; i++) {
				if (board[i][col] == 0) {
					moves.push_back(create_move(col, row, col, i));
				} else if (board[i][col] < 0) {
					moves.push_back(create_move(col, row, col, i));
					break;
				} else if (board[i][col] > 0) {
					break;
				}
			}
			for (int i = row - 1; i > 1; i--) {
				if (board[i][col] == 0) {
					moves.push_back(create_move(col, row, col, i));
				} else if (board[i][col] < 0) {
					moves.push_back(create_move(col, row, col, i));
					break;
				} else if (board[i][col] > 0) {
					break;
				}
			}
			
		break;

		case queen:
			// bishop + rook
			// bishop
			for (int i = row - 1, j = col - 1; i > 1 && j > 1; i--, j--) {
				if ((i - j) == (row - col)) {
					if (board[i][j] == 0) {
						moves.push_back(create_move(col, row, j, i));
					} else if (board[i][j] < 0) {
						moves.push_back(create_move(col, row, j, i));
						break;
					} else if (board[i][j] > 0) {
						break;
					}
				}
				
			}
			for (int i = row + 1, j = col + 1; i < 10 && j < 10; i++, j++) {
				if ((i - j) == (row - col)) {
					if (board[i][j] == 0) {
						moves.push_back(create_move(col, row, j, i));
					} else if (board[i][j] < 0) {
						moves.push_back(create_move(col, row, j, i));
						break;
					} else if (board[i][j] > 0) {
						break;
					}
				}
			}
			for (int i = row - 1, j = col + 1; i > 1 && j < 10; i--, j++) {
				if ((i + j) == (row + col)) {
					if (board[i][j] == 0) {
						moves.push_back(create_move(col, row, j, i));
					} else if (board[i][j] < 0) {
						moves.push_back(create_move(col, row, j, i));
						break;
					} else if (board[i][j] > 0) {
						break;
					}
				}
			}
			for (int i = row + 1, j = col - 1; i < 10 && j > 1; i++, j--) {
				if ((i + j) == (row + col)) {
					if (board[i][j] == 0) {
						moves.push_back(create_move(col, row, j, i));
					} else if (board[i][j] < 0) {
						moves.push_back(create_move(col, row, j, i));
						break;
					} else if (board[i][j] > 0) {
						break;
					}
				}
			}

			// rook
			for (int i = col + 1; i < 10; i++) {
				if (board[row][i] == 0) {
					moves.push_back(create_move(col, row, i, row));
				} else if (board[row][i] < 0) {
					moves.push_back(create_move(col, row, i, row));
					break;
				} else if (board[row][i] > 0) {
					break;
				}
			}
			for (int i = col - 1; i > 1; i--) {
				if (board[row][i] == 0) {
					moves.push_back(create_move(col, row, i, row));
				} else if (board[row][i] < 0) {
					moves.push_back(create_move(col, row, i, row));
					break;
				} else if (board[row][i] > 0) {
					break;
				}
			}
			
			for (int i = row + 1; i < 10; i++) {
				if (board[i][col] == 0) {
					moves.push_back(create_move(col, row, col, i));
				} else if (board[i][col] < 0) {
					moves.push_back(create_move(col, row, col, i));
					break;
				} else if (board[i][col] > 0) {
					break;
				}
			}
			for (int i = row - 1; i > 1; i--) {
				if (board[i][col] == 0) {
					moves.push_back(create_move(col, row, col, i));
				} else if (board[i][col] < 0) {
					moves.push_back(create_move(col, row, col, i));
					break;
				} else if (board[i][col] > 0) {
					break;
				}
			}
			
		break;

		case knight:
			add_simple_move(moves, board, row, col, 1, -2);
			add_simple_move(moves, board, row, col, 1, 2);
			add_simple_move(moves, board, row, col, 2, -1);
			add_simple_move(moves, board, row, col, 2, 1);
			add_simple_move(moves, board, row, col, -1, -2);
			add_simple_move(moves, board, row, col, -1, 2);
			add_simple_move(moves, board, row, col, -2, -1);
			add_simple_move(moves, board, row, col, -2, 1);
			
		break;

		case king:
			add_king_move(moves, board, row, col, 1, 1);
			add_king_move(moves, board, row, col, 1, 0);
			add_king_move(moves, board, row, col, 1, -1);
			add_king_move(moves, board, row, col, 0, 1);
			add_king_move(moves, board, row, col, 0, -1);
			add_king_move(moves, board, row, col, -1, 1);
			add_king_move(moves, board, row, col, -1, 0);
			add_king_move(moves, board, row, col, -1, -1);
			
		break;

		default:
			moves.push_back("");
			break;
	}
}}}

	vector<string> filtered_moves;
	for (string next_move : moves) {
		int taken_piece = move(next_move, board);
		if (!is_check(board)) {
			filtered_moves.push_back(next_move);
		} else {
			cout << "# AR FI SAH LA " << (true == true ? convert_move(next_move) : next_move) << endl;
		}
		undo_move(next_move, board, taken_piece);
	}
	return filtered_moves;
}

// updates the internal board representation
int move(string m, int board[12][12]) {
	int col = coord_to_col(m.at(0));
	int row = coord_to_row(m.at(1));
	int new_col = coord_to_col(m.at(2));
	int new_row = coord_to_row(m.at(3));
	int piece = board[row][col];
	int taken_piece = board[new_row][new_col];
	board[row][col] = 0;
	board[new_row][new_col] = piece;
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

void print_board_d(int board[12][12]) {
	cout << "# " << endl;
	for (int i = 2; i < 10; i++) {
		cout << "# ";
		for(int j = 2; j < 10; j++) {
			if (board[i][j] >= 0)
				cout << board[i][j] << " | ";
			else
				cout << board[i][j] << "| ";
		}
		cout << "# " << endl << "# -------------------------------" << endl;
	}
}


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