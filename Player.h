#include <vector>
#include <set>
#include "Tile.h"
#include "Bag.h"
#include <string>
#include <sstream>

namespace patch
{
    template < typename T > std::string to_string( const T& n )
    {
        std::ostringstream stm ;
        stm << n ;
        return stm.str() ;
    }
}

#include <iostream>
using namespace std;


class Player{

public:
	Player(string nm, int scr, set<Tile*> letters);
	string getName();
	void addScore(int num);
	void subtractScore(int num);
	int getCurrScore();
	string getScoreAsString();
	set<Tile*> getTiles();
	bool hasAllTiles(string str);
	void exchangeTiles(string str, Bag &bag);
	void exchangeTilesWhenPlacing(string str, Bag &bag);
	string printTiles();
	int tileScores();
	void giveTiles(set<Tile*> letters);

private:
	string name;
	int currScore;
	set<Tile*> tiles;


};