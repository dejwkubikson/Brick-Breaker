
/*
==========================================================================
cSoundMgr.h
==========================================================================
*/

#include "GameConstants.h"

using namespace std;

class cSceneMgr
{
private:
	
protected:
	cSceneMgr();
	~cSceneMgr();

public:
	enum class gameState { menu, playing, end, quit };

	void playGame();
	void mainMenu();
	void endMenu();
	void quit();

	gameState theGameState;

};
#pragma once
