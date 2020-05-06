#include <iostream>
#include <vector>

#include "minimax.h"
#include "engine.h"
#include "utils.h"

using namespace std;

int evaluate_move(int board[12][12]) {
	int sum = 0;

	if (is_check(board)) {
		return -100;
	}

	flip_board(board);
	if (is_check(board)) {
		flip_board(board);
		return 100;
	}
	flip_board(board);

	for (int i = 2; i < 10; i++) {
		for (int j = 2; j < 10; j++) {
			sum += board[i][j];
		}
	}
	
	return sum;
}

bool game_over(int board[12][12]) {
	return false;
}

int alpha_beta_negamax(int alpha, int beta, int depth, int board[12][12], string* m) {
	if (depth == 0 || game_over(board)) {
		return evaluate_move(board);
	} 
	string best_move = "kkkk";
	int max = -9999999;
	int different_scores = 0;

	vector<string> possible_moves = search_legal_move(board);

	for (string next_move : possible_moves) {
		int taken_piece = move(next_move, board);

		flip_board(board);
		int score = -alpha_beta_negamax(-beta, -alpha, depth - 1, board, m);
		flip_board(board);
		cout << "# " << next_move << " scor: " << score << " | max = " << max << endl;
		if (score > max) {
			max = score;
			cout << "#  SCORE > MAX \n";
			best_move = next_move;
			different_scores++;
		}

		if (max > alpha) {
			alpha = max;
			cout << "# MAX > ALPHA \n";
		}

		if (alpha >= beta) {
			undo_move(next_move, board, taken_piece);
			break;
		}

		undo_move(next_move, board, taken_piece);
	}
	cout << "# " << different_scores <<endl;
	if (different_scores == 1) {
		best_move = possible_moves[rand() % (possible_moves.size())];
	}
	*m = best_move;
	cout << "# THE BEST MOVE IS " << best_move << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n";
	return alpha;
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
