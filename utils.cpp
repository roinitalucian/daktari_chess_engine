#include <string>

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
	return !((white_turn && play_black) || (black_turn && play_white));
}