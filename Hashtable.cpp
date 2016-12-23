#include <string>
#include <iostream>
#include "math.h"
#include "Hashtable.h"
/**
* A Hashtable to store <std::string, int> as the entries. The string represents
* the element in the Hashtable, and the int represents the number of instances of
* that string in the Hashtable. 
*/

	/**
	* Creates a new Hashtable instance. Two optional parameters are provided, and by
	* default are turned off. If the debug parameter is set to true, then the Hashtable
	* should use the pre-prescribed random numbers in the assignment description. The 
	* probing parameter will always be 0, 1, or 2, indicating the kind of collision
	* avoidance strategy. 
	* 0: Linear probing
	* 1: Quadratic probing
	* 2: Double hashing
	*/
	Hashtable::Hashtable(bool debug, unsigned int probing){
		probeCommand = probing;
		//hTable = new vector<pair<string, int> >();
		hTableSizeIndex = 0;
		randomIntegers = new vector<int>();
		loadFactor = 0;

		for(int i = 0; i < resizeIndices[hTableSizeIndex]; i++){
			hTable.push_back(pair<string, int>("",0));
		}

		if(debug){
			randomIntegers->push_back(983132572);
			randomIntegers->push_back(1468777056);
			randomIntegers->push_back(552714139);
			randomIntegers->push_back(984953261);
			randomIntegers->push_back(261934300);
		}
		else{
			srand (time(NULL));
			for(int i = 0; i < 5; i++){
				randomIntegers->push_back((long long)rand() % 1685759167 + 1);
			}
		}

	}

	/**
	* Destructor.
	*/
	Hashtable::~Hashtable(){
		delete randomIntegers;
	}

	/**
	* If the parameter k already exists as an element in the Hashtable, increment the 
	* total number of entries. Otherwise create a new entry with a count of 1.
	*/
	void Hashtable::add(const std::string& k){
		int hashed = hash(k);
		if(count(k) > 0){
			(hTable[hashed]).second++;
			resize();
			return;
		}
		else if(probeCommand == 0){
			while(hTable[hashed].first != "" && hTable[hashed].first != k){
				hashed++;
				hashed = hashed % hTable.size();
			} 
			if(hTable[hashed].first == ""){
				(hTable)[hashed].first = k;
				loadFactor++;
			}
			if(hTable[hashed].first == k){
				hTable[hashed].second++;
			}
		}
		else if(probeCommand == 1){
			tableProbe = 1;
			while(hTable[hashed].first != "" && hTable[hashed].first != k){
				hashed += pow(tableProbe,2)-pow(tableProbe-1,2);
				hashed=hashed % hTable.size();
				tableProbe++;
			}
			if(hTable[hashed].first == ""){
				(hTable)[hashed].first = k;
				loadFactor++;
			}
			if(hTable[hashed].first == k){
				hTable[hashed].second++;
			}

		}
		else if(probeCommand == 2){
			long long finalize = 0;
			string storageIndex = k;
			int a = 0;
			int i;
			//doubleHash function
			while(i = storageIndex[storageIndex.size() - 1]){
				storageIndex = storageIndex.substr(0, storageIndex.size()-1);
				finalize += pow(26, a)*(long long)(i-97);
				a++;
				if(a > 5){
					a = 0;
				}
			}
			tableProbe = primes[hashed] - (finalize % primes[hashed]); 

			while(hTable[hashed].first != "" && hTable[hashed].first != k){
				hashed += tableProbe;
				hashed=hashed % hTable.size();
			}
			if(hTable[hashed].first == ""){
				(hTable)[hashed].first = k;
				loadFactor++;
			}
			if(hTable[hashed].first == k){
				hTable[hashed].second++;
			}
		}
		resize();
	}

	/**
	* Returns the total number of entries for the given string, or 0 if it does not
	* exist in the Hashtable.
	*/
	int Hashtable::count(const std::string& k) const{
		//finds total number of enteries in the hasTable for the string
		int i = hash(k);
		while(hTable[i].first != ""){
			if(hTable[i].first == k){
				return hTable[i].second;
			}
			//searches for k linearly, must be after where it hashes to if 
			//the original hash was already filled with a different string
			i++;
		}
		//if not found in the list, return 0
		return 0;
	}

	/**
	* Outputs to the provided stream every key-value pair of elements in the Hashtable
	* in the order they appear. The output should follow the format that each
	* key-value pair is printed on a separate line, and the key is separated from the 
	* value by a space .
	*/
	void Hashtable::reportAll(std::ostream& out) const{
		int i = 0;
		//should output every pair in the hash table
		while(i < (signed)hTable.size()){
			out << (hTable)[i].first << " " << (hTable)[i].second << endl;
			i++;
		}

	}

	void Hashtable::resize(){
		//if more than half of the elements in the table are filled, the table resizes
			if(loadFactor > .5*hTable.size()){
				vector<pair<string, int> > tempHTable = hTable;
				hTableSizeIndex++;
				int newSizeTracker = 0; 
				//making the vector the new resized size filled with empty pairs
				while(newSizeTracker < resizeIndices[hTableSizeIndex]){
					hTable.push_back(pair<string, int>("",0));
					newSizeTracker++;
				}
				//put the original hashtable back into the now larger hashtable
				//also reshashes the strings already in the hashtable
				for(int i = 0; i < (signed)tempHTable.size(); i++){
					for(int j = 0; j < (tempHTable)[i].second; j++){
						if(tempHTable[i].first != ""){
							add((tempHTable)[i].first);
						}
					}
				}
			}
	}

	/**
	* The actual hash function that transforms the string input into the index where
	* the element should be stored. See the assignment for more details on how to 
	* implement this function according to our specific guidelines. 
	*/
	int Hashtable::hash(const std::string& k) const{
		int list[5] = {};
		long long finalize = 0;
		string str = k;
		int a,w, lowerCase, currRandVal;
		a = 0;
		w = 4;
		currRandVal = 0;

		while(lowerCase = str[str.size() - 1]){
			//int lowerCase = (int)str[str.size() - 1];
			str = str.substr(0, str.size() - 1);
			list[w] = pow(26, a)*(long long)(lowerCase-97);
			a++;
			if(a > 5){
				w--;
				a = 0;
			}
			else{
				break;
			}
		}
		while(currRandVal < 5){
			int randInt = (*randomIntegers)[currRandVal];
			finalize += list[currRandVal] * randInt;
			currRandVal++;
		}
		return finalize % hTable.size();
	}