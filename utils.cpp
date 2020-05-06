#include <string>

#include "utils.h"

int coord_to_col(char c) {
	return c - 'a' + 2;
}

int coord_to_row(char c) {
	return 10 - c + '0';
}

char col_to_coord(char c) {
	return c + 'a' - 2;
}

char row_to_coord(char c) {
	return 10 - c + '0';
}

std::string convert_move(std::string m) {
	if (is_promotion(m)) {
		std::string new_p = "qqqqq";
		new_p.at(0) = 'h' + 'a' - m.at(0);
		new_p.at(1) = '9' - m.at(1) + '0';
		new_p.at(2) = 'h' + 'a' - m.at(2);
		new_p.at(3) = '9' - m.at(3) + '0';
		new_p.at(4) = m.at(4);
		return new_p;
	}
	std::string new_m = "qqqq";
	new_m.at(0) = 'h' + 'a' - m.at(0);
	new_m.at(1) = '9' - m.at(1) + '0';
	new_m.at(2) = 'h' + 'a' - m.at(2);
	new_m.at(3) = '9' - m.at(3) + '0';
	return new_m;
}


std::string create_move(int col, int row, int new_col, int new_row) {
	std::string move = "qqqq";
	move.at(0) = col_to_coord(col);
	move.at(1) = row_to_coord(row);
	move.at(2) = col_to_coord(new_col);
	move.at(3) = row_to_coord(new_row);
	return move;
}

bool is_move(std::string input) {
	if (input.length() != 4) {
		return false;
	}
	char c1 = input.at(0);
	char c2 = input.at(1);
	char c3 = input.at(2);
	char c4 = input.at(3);
	return (islower(c1) && islower(c3) && isdigit(c2) && isdigit(c4));
}

bool is_promotion(std::string input) {
	if (input.length() != 5) {
		return false;
	}
	char c1 = input.at(0);
	char c2 = input.at(1);
	char c3 = input.at(2);
	char c4 = input.at(3);
	char c5 = input.at(4);
	return (islower(c1) && islower(c3) && isdigit(c2) && isdigit(c4) && islower(c5));
}

bool my_turn(bool white_turn, bool black_turn, bool play_white, bool play_black) {
	return ((white_turn && play_white) || (black_turn && play_black));
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