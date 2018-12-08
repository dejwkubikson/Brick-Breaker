#pragma once
#include <string>
#include <vector>

using namespace std;

class cHighScoreTable
{
private:
	int highestScore = 0;
public:
	cHighScoreTable();

	int loadFromFile(string highScoreFile);
	bool saveToFile(string score, string highScoreFile);
	void compareScoreAndSave(string score, string highScoreFile);

	virtual ~cHighScoreTable();
};

