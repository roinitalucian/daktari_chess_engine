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