#include "Tile.h"

class Square{
	public:
		Square(int x, int y, int z);
		bool isOccupied();
		Tile occupyingTile();
		int getLetterBonus();
		int getWordBonus();
		void insertTile(char ltr, int scr);
		int getPoints();
		char getLetter();
		void Occupied();


	private:
		int letterMultiplier;
		int wordMultiplier;
		char ch;
		int letterScore;
		bool occupied;

};
