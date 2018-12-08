#include "cHighScoreTable.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>


cHighScoreTable::cHighScoreTable()
{

}

int cHighScoreTable::loadFromFile(string highScoreFile)
{
	ifstream file;
	string strHighScore;

	file.open(highScoreFile);
	if (!file.is_open())
	{ 
		cout << "Error opening file " << highScoreFile << endl;
		// file not open, default score set to 0
		highestScore = 0;
		return false;
	}
	// if file is not empty
	else if(!(file.eof()))
	{
		getline(file, strHighScore);
		// Converting the string to int
		highestScore = stoi(strHighScore);
	}
	else
	{
		// saving 0 as the highestScore
		saveToFile("0", highScoreFile);
		highestScore = 0;
	}
	
	file.close();
	return highestScore;
}

bool cHighScoreTable::saveToFile(string score, string highScoreFile)
{
	ofstream file;
	// Overwriting the existing file
	file.open(highScoreFile, ios_base::app);
	if (!file.is_open())
	{ 
		cout << "Error opening file " << highScoreFile << " for saving to ..." << endl;
		return false;
	}
	else
	{
		highestScore = stoi(score);
		file << highestScore;
	}

	file.close();
	return true;
}

// comparing the score and saving the heigher one
void  cHighScoreTable::compareScoreAndSave(string score, string highScoreFile)
{
	int toCompare = loadFromFile(highScoreFile);
	//cout << "SCORE: " << stoi(score) << "TO COMPARE: " << toCompare;

	// if the score is higher than in the file, save it
	if (stoi(score) > toCompare)
	{
		saveToFile(score, highScoreFile);
	}
}

cHighScoreTable::~cHighScoreTable()
{
}
