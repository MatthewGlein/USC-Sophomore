#include "Player.h"

Player::Player(string nm, int scr, set<Tile*> letters){
	name = nm;
	currScore = scr;
	tiles = letters;
}

void Player::giveTiles(set<Tile*> letters){
	tiles = letters;
}

string Player::getName(){
	return name;
}
	
int Player::getCurrScore(){
	return currScore;
}
string Player::getScoreAsString(){
	string scoreString = patch::to_string(currScore);
	return scoreString;
}
void Player::addScore(int num){
	currScore += num;
}
void Player::subtractScore(int num){
	currScore = currScore - num;
}

set<Tile*> Player::getTiles(){
	return tiles;
}
bool Player::hasAllTiles(string str){
	vector<Tile*> temp(tiles.begin(), tiles.end());
	vector<Tile*>::iterator it;
	int scoreOfTiles;

	for(int i = 0; i < (int)str.size(); i++){
		bool isFound = false;
		for(it = temp.begin(); it != temp.end();){
			if(str.at(i) == '?' && ((*it)->isBlank())){
				//how do you do points for blanks?
				it = temp.erase(it);
				i++;
				isFound = true;
				break;
			}
			else if(str.at(i) == (*it)->getLetter()){
				scoreOfTiles += ((*it)->getPoints());
				it = temp.erase(it);
				isFound = true;
				break;
			}
			++it;
		}
		if(!isFound){
			return false;
		}
	}

	return true;
}


int Player::tileScores(){
	vector<int> letterScores;
	int totalScore = 0;
	vector<Tile*> temp(tiles.begin(), tiles.end());
	int pnts;
	
	for(int j = 0; j < (int)temp.size(); j++){
		if(temp[j]->isBlank()){
			continue;
		}
		else{
			pnts = (int) temp[j]->getPoints();
			letterScores.push_back(pnts);
		}
	}
	for(int i = 0; i < (int)letterScores.size(); i++){
		totalScore += letterScores[i];
	}

	return totalScore;
}

void Player::exchangeTiles(string str, Bag &bag){
	vector<Tile*> temp;
	std::set<Tile*>::iterator itt;
	for(itt = tiles.begin(); itt != tiles.end(); ++itt){
		temp.push_back(*itt);
	}
	vector<int> whereFound;
	vector<char> letterFound;
	vector<Tile*> leftOver;
	set<Tile*>::iterator it;
	for(int i = 0; i < (int)str.size(); i++){
			bool alreadyFound = false;
		for(int j = 0; j < (int)temp.size(); j++){
				for(int k = 0; k < (int)whereFound.size(); k++){
					if((j == whereFound[k]) && (str.at(i) == letterFound[k])){
						alreadyFound = true;
					}
				}
			if((str.at(i) == '?' && (temp[j]->isBlank())) || (str.at(i) == (temp[j])->getLetter() && !alreadyFound)){
					if(!alreadyFound){
					bag.addTile(temp[j]);
					whereFound.push_back(j+1);
					letterFound.push_back(str.at(i));
				}
			}
		}
	}
	for(int i = 0; i < (int)temp.size(); i++){
		bool shouldDelete = false;
		for(int j = 0; j < (int)whereFound.size(); j++){
			if(i == whereFound[j]){
				shouldDelete = true;
			}
		}
		if(!shouldDelete){
			leftOver.push_back(temp[i]);
		}
	}
	set<Tile*> newSet = bag.drawTiles(str.size());
	for(int i = 0; i < (int)leftOver.size(); i++){
		if(leftOver[i]->getPoints() != 0){
			newSet.insert(leftOver[i]);	
		}
	}
	/*it = newSet.end();
	--it;
	for(int i = 0; i < (int)leftOver.size(); i++){
		newSet.insert(it, leftOver[i]);
		++it;
	}*/
	giveTiles(newSet);
	//tiles = newSet;
}
void Player::exchangeTilesWhenPlacing(string str, Bag &bag){
	vector<Tile*> temp;
	std::set<Tile*>::iterator itt;
	for(itt = tiles.begin(); itt != tiles.end(); ++itt){
		temp.push_back(*itt);
	}
	vector<int> whereFound;
	vector<char> letterFound;
	vector<Tile*> leftOver;
	set<Tile*>::iterator it;
	for(int i = 0; i < (int)str.size(); i++){
			bool alreadyFound = false;
		for(int j = 0; j < (int)temp.size(); j++){
				for(int k = 0; k < (int)whereFound.size(); k++){
					if((j == whereFound[k]) && (str.at(i) == letterFound[k])){
						alreadyFound = true;
					}
				}
			if((str.at(i) == '?' && (temp[j]->isBlank())) || (str.at(i) == (temp[j])->getLetter() && !alreadyFound)){
					if(!alreadyFound){
					whereFound.push_back(j+1);
					letterFound.push_back(str.at(i));
				}
			}
		}
	}
	for(int i = 0; i < (int)temp.size(); i++){
		bool shouldDelete = false;
		for(int j = 0; j < (int)whereFound.size(); j++){
			if(i == whereFound[j]){
				shouldDelete = true;
			}
		}
		if(!shouldDelete){
			leftOver.push_back(temp[i]);
		}
	}
	set<Tile*> newSet = bag.drawTiles(str.size());
	for(int i = 0; i < (int)leftOver.size(); i++){
		if(leftOver[i]->getPoints() != 0){
			newSet.insert(leftOver[i]);	
		}
	}
	/*it = newSet.end();
	--it;
	for(int i = 0; i < (int)leftOver.size(); i++){
		newSet.insert(it, leftOver[i]);
		++it;
	}*/
	giveTiles(newSet);
	//tiles = newSet;
}

string Player::printTiles(){
	string toPrint = "";
	set<Tile*>::iterator it;
	for(it = tiles.begin(); it != tiles.end(); ++it){
		char ch = ((*it)->getLetter());
		int score = ((*it)->getPoints());
		std::string scr = patch::to_string(score);
		toPrint += "(";
		toPrint += ch;
		toPrint += ",";
		toPrint += scr;
		toPrint += ")";
		toPrint += " ";
	}
	//cout << endl;
	return toPrint;
}