#include "cSceneMgr.h"

/*
=================
- Constructor
=================
*/
cSceneMgr::cSceneMgr()
{

}

/*
=================
- Destructor.
=================
*/
cSceneMgr::~cSceneMgr()
{

}

void cSceneMgr::mainMenu()
{
	theGameState = gameState::menu;
}

void cSceneMgr::playGame()
{
	theGameState = gameState::playing;
}

void cSceneMgr::endMenu()
{
	theGameState = gameState::end;
}

void cSceneMgr::quit()
{
	theGameState = gameState::quit;
}