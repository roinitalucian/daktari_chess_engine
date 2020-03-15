#include <iostream>
#include <fstream>

#include "engine.h"

using namespace std;

int main() {

	string input;
	// start_game();
    while (1) {

    	getline(cin, input);

    	if (input == "protover 2") {
    		cout << "feature sigint=0\n";
    		cout << "feature done=1\n";
    	} else if (input == "hard") {
    		start_game();
    	} 
    }

    return 0;
}