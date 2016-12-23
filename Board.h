/*
 * Board.h
 *
 *  Created on: Sep 18, 2016
 *      Author: kempe
 */

// The following is a suggestion for how to proceed.
// Feel free to make any changes you like.

#ifndef BOARD_H_
#define BOARD_H_
#include "Tile.h"
#include "Square.h"
#include "Dictionary.h"


#include <string>
#include <stdlib.h> 
#include <vector>

using namespace std;

class Board {
public:
	Board (std::string board_file_name);

	int Move(string direction, string rw, string column, string toBeWord, std::set<Tile*> set);

	void printBoard();

	int returnXsize();
	int returnYsize();
	string returnLetter(int i, int j);

	// What else will this need?

private:
	int _x, _y;
	int _startx, _starty;
	string board[100][100];
	Square *squares[100][100];
	string dir, word;
	int col, row;
	vector<Tile*> currTiles;
	// What else will this need?

};


#endif /* BOARD_H_ */
