#include <iostream>

#include "debugging.h"
#include "engine.h"

using namespace std;

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