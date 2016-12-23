#include "Square.h"

Square::Square(int x, int y, int z){
	letterMultiplier = x;
	wordMultiplier = y;
	letterScore = z;
	occupied = false;
}

bool Square::isOccupied(){
	return occupied;
}
void Square::Occupied(){
	occupied = true;
}

int Square::getLetterBonus(){
	return letterMultiplier;
}

int Square::getWordBonus(){
	return wordMultiplier;
}

void Square::insertTile(char ltr, int scr){
	ch = ltr;
	letterScore = scr;
}

int Square::getPoints(){
	return letterScore;
}

char Square::getLetter(){
	return ch;
}