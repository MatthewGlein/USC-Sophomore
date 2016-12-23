/*
 * Board.cpp
 *
 *  Created on: Sep 18, 2016
 *      Author: kempe
 */

// The following is a suggestion for how to proceed.
// Feel free to make any changes you like.

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include "Board.h"

using namespace std;

Board::Board (string board_file_name)
{
	ifstream boardFile (board_file_name.c_str());
	string row;

	_x = _y = _startx = _starty = 0; // to appease compiler
	if (!boardFile.is_open())
		throw invalid_argument("Cannot open file: " + board_file_name);
	getline (boardFile, row);
	stringstream s1 (row);
	s1 >> _x >> _y;
	getline (boardFile, row);
	stringstream s2 (row);
	s2 >> _startx >> _starty;
	_startx --; _starty --;  // coordinates from 0 in array

	// Anything else you need to initialize?
	for (int i = 0 ; i < _y; ++ i)
	{
		getline (boardFile, row);
		for (int j = 0; j < _x; ++ j)
		{
			// Fill in the following based on how you store the board.
			if (i == _starty && j == _startx) { }
			else switch (row[j]) {
			case '.' :	squares[i][j] = new Square(1,1,0);
						board[i][j] = ".";
						break;
			case '2' :	squares[i][j] = new Square(2,1,0);
						board[i][j] = "2";
						break;
			case '3' :	squares[i][j] = new Square(3,1,0);
						board[i][j] = "3";
						break;
			case 'd' :	squares[i][j] = new Square(1,2,0);
						board[i][j] = "d";
						break;
			case 't' :	squares[i][j] = new Square(1,3,0);
						board[i][j] = "t";
						break;
			default:
				string error = "Improper character in Board file: ";
				throw invalid_argument(error + row[j]);
			}
		}
	}
	squares[7][7] = new Square(1,1,0);
	board[7][7] = ".";
	boardFile.close ();
}

int Board::Move(string direction, string rw, string column, string toBeWord, std::set<Tile*> set){
	Dictionary dict("dictionary.txt");
	dir = direction;
	row = atoi(rw.c_str());
	col = atoi(column.c_str());
	row--;
	col--;
	word = toBeWord;
	int placeCounter = 0;
	char ch;
	vector<Tile*> playerTiles;
	std::set<Tile*>::iterator it;
	for(it = set.begin(); it != set.end(); ++it){
		playerTiles.push_back(*it);
	}
	int points;
	int letterScores = 0;
	int wordBonus = 1;
	int origWordSize = word.size();
	bool connecting = false;
	vector<int> scorePerLetter;
	//compare word they want w/ the map to see what the full word is
	//Need to put tiles into squares still dk how
	if(squares[row][col]->isOccupied()){
		return -6;
	}

	//SIDE 2 SIDE
	if(dir == "|"){
		//Makes sure it is placed in bounds
		for(int i = col; i <= origWordSize + col; i++){
			if(row > _y || i > _x){
				return -5;
			}
		}
		//Makes sure first move has a letter in the middle
		if(!squares[_starty][_startx]->isOccupied()){
			bool valid = false;
			for(int i = col; i < (int)word.size() + col; i++){
				if(_starty == row && _startx == i ){
					valid = true;
				}
			}
			if(!valid){
				return -4;
			}
		}
		//Checks if there is letter before your word
		int b = 1;
		while(squares[row][col-b]->isOccupied()){
				ch = squares[row][col-b]->getLetter();
				word.insert(0, &ch);
				placeCounter++;
				connecting = true;
				b++;
		}
		//Checks if there are occupied squares where you are placing
		//your word, if so, put the letter in your word
		//for loop checks 1 square before & one after
		int r = 0;
		for(int j = col; j < (int)word.size() + col; j++){ 
			if(squares[row][j]->isOccupied()){
				ch = squares[row][j]->getLetter();
				word.insert(r, &ch);
				connecting = true;
			}
			r++;
			placeCounter++;
		}
		//checks if there is a letter after your word ends
		b = 0;
		while(squares[row][col + (int)word.size() + b]->isOccupied()){ 
			ch = squares[row][col + (int)word.size()]->getLetter();
			word.insert((int)word.size(), &ch);
			placeCounter++;
			connecting = true;
			b++;
		}
		if(!connecting && squares[_starty][_startx]->isOccupied()){
			return -3;
		}
		//Check if is dictionary and valid
		if(!dict.searchDict(word)){
			return -2;
		}
		//Inserts tiles into board and squarea
		int f = 0;
		for(int j = col; j < (int)toBeWord.size() + col; j++){
			wordBonus = wordBonus * squares[row][j]->getWordBonus();
			if((j == col) && squares[row][col-1]->isOccupied()){
				letterScores += squares[row][j]->getPoints();
				//f++;
			}
			if(squares[row][j]->isOccupied()){
				letterScores += squares[j][col]->getPoints();
				//if(j == ((int)toBeWord.size() + col) -1){
					board[row][j+1] = toBeWord.at(f);
					squares[row][j+1]->Occupied();
					f++;
				//}

			}
			else{
				if(toBeWord.at(f) == '?'){
					board[row][j] = word.at(f+1);
					f++;
				}
				else{
				board[row][j] = toBeWord.at(f);
				squares[row][j]->Occupied();
				}
				f++;				
			}
		}
		int tempCol = col;
		for(int i = 0; i < (int)toBeWord.size(); i++){
			for(int k = 0; k < (int)playerTiles.size(); k++){
				//cout << playerTiles[k]->getPoints();
				if(toBeWord.at(i) == '?'){
					//Lol you dont have a tile for toBeWord[i+1]
					if(toBeWord[i+1] == (playerTiles[k])->getLetter()){
						squares[row][tempCol]->insertTile(playerTiles[k]->getLetter(), playerTiles[k]->getPoints());//IDK HOW WHAT
						board[row][tempCol] = toBeWord[i+1];
						i++;
					}
				}
				else if(toBeWord.at(i) == (playerTiles[k])->getLetter()){
					letterScores += (playerTiles[k]->getPoints() * squares[row][tempCol]->getLetterBonus());
					squares[row][tempCol]->insertTile(playerTiles[k]->getLetter(), playerTiles[k]->getPoints());//IDK HOW WHAT
					break;
				}
			}
			tempCol++;
		}
		points = letterScores * wordBonus;
		return points;
	}




	//GOIN DOWN
	else if(dir == "-"){
		//Makes sure it is placed in bounds
		for(int j = row; j <= origWordSize + row; j++){
			if(j > _y || col > _x){
				return -5;
			}
		}
		//Makes sure first move has a letter in the middle
		if(!squares[_starty][_startx]->isOccupied()){
			bool valid = false;
			for(int j = row; j < (int)word.size() + row; j++){
				if(_starty == j && _startx == col ){
					valid = true;
				}
			}
			if(!valid){
				return -4;
			}
		}
		//Checks if theres something side to side
			//Checks if theres something above or below
		//Checks if there is letter before your word
		int c = 1;
		while(squares[row-c][col]->isOccupied()){ 
			ch = squares[row-c][col]->getLetter();
			word.insert(0, &ch);
			placeCounter++;
			connecting = true;
			c++;
		}
		//Checks if there are occupied squares where you are placing
		//your word, if so, put the letter in your word
		//for loop checks 1 square before & one after
		c = 0;
		for(int j = row; j < (int)word.size() + row; j++){ 
			if(squares[j][col]->isOccupied()){
				ch = squares[j][col]->getLetter();
				word.insert(c, &ch);
				connecting = true;
			}
			c++;
			placeCounter++;
		}
		//checks if there is a letter after your word ends
		c=0;
		while(squares[row + (int)word.size() + c][col]->isOccupied()){ 
			ch = squares[row + (int)word.size()+c][col]->getLetter();
			word.insert((int)word.size(), &ch);
			placeCounter++;
			connecting = true;
			c++;
		}
		if(!connecting && squares[_starty][_startx]->isOccupied()){
			return -3;
		}
		//Check if is dictionary and valid
		if(!dict.searchDict(word)){
			return -2;
		}
		//Inserts tiles into board and squarea
		//cout << word;
		int f = 0;
		for(int j = row; j < (int)toBeWord.size() + row; j++){
			if((j == row) && squares[row-1][col]->isOccupied()){
				letterScores += squares[row-1][col]->getPoints();
			}
			if(squares[j][col]->isOccupied()){
				letterScores += squares[j][col]->getPoints();
				if(j == ((int)toBeWord.size() + row) -1){
					board[j+1][col] = toBeWord.at(f);
					squares[j+1][col]->Occupied();
				}
				f++;
			}
			else{
				wordBonus = wordBonus * squares[row][j]->getWordBonus();
				if(toBeWord.at(f) == '?'){
					board[j][col] = toBeWord.at(f+1);
					f++;
				}
				else{
				board[j][col] = toBeWord.at(f);
				squares[j][col]->Occupied();
				}
				f++;				
			}
		}	

				//Checks which tile corresponds with the letter
		int tempRow = row;
		for(int i = 0; i < (int)toBeWord.size(); i++){
			for(int k = 0; k < (int)playerTiles.size(); k++){
				//cout << playerTiles[k]->getPoints();
				//if(squares[tempRow][col]->isOccupied()){
				//	tempRow++;
				//}
				if(toBeWord.at(i) == '?'){
					//Lol you dont have a tile for word[i+1]
					if(toBeWord[i+1] == (playerTiles[k])->getLetter()){
						squares[tempRow][col]->insertTile(playerTiles[k]->getLetter(), playerTiles[k]->getPoints());//IDK HOW WHAT
						board[tempRow][col] = word[i+1];
						i++;
					}
				}
				else if(toBeWord.at(i) == (playerTiles[k])->getLetter()){
					squares[tempRow][col]->getLetterBonus();
					letterScores += (playerTiles[k]->getPoints() * squares[tempRow][col]->getLetterBonus());
					squares[tempRow][col]->insertTile(playerTiles[k]->getLetter(), playerTiles[k]->getPoints());//IDK HOW WHAT
					break;
				}
			}
			tempRow++;
		}
		points = letterScores * wordBonus;
		return points;
	}
	return -1;
}

void Board::printBoard(){
	string delimiter = " ";
	for(int i = 0; i < _y; i++){
		for(int j = 0; j < _x; j++){
			cout << board[i][j] << delimiter;
		}
		cout << endl;
	}
}

int Board::returnXsize(){
	return _x;
}

int Board::returnYsize(){
	return _y;
}

string Board::returnLetter(int i, int j){
	return board[i][j];
}