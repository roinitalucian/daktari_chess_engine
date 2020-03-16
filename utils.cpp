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