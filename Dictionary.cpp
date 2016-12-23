/*
 * Dictionary.cpp
 *
 *  Created on: Sep 18, 2016
 *      Author: kempe
 */

// The following is a suggestion for how to proceed.
// Feel free to make any changes you like.

#include <string>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include "Dictionary.h"

using namespace std;

Dictionary::Dictionary (std::string dictionary_file_name)
{
	ifstream dictFile (dictionary_file_name.c_str());
	std::string word;

	if (dictFile.is_open())
	{
		while (getline (dictFile, word))
		{
			word.erase(word.length()-1); // remove end-of-line character
			// What do you want to do with the word?
			wordSet.insert(word);

		
		}
		dictFile.close ();
	}
	else throw invalid_argument("Cannot open file: " + dictionary_file_name);
}

bool Dictionary::searchDict(string str){
	for(int i = 0; i < (int)str.size(); i++){
		str[i] = (char)tolower(str[i]);
	}
	std::set<string>::iterator it = wordSet.find(str);
	if(it == wordSet.end()){
		return false;
	}
	return true;
}