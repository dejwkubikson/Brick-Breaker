#ifndef _GAME_H
#define _GAME_H

/*
==================================================================================
cGame.h
==================================================================================
*/

#include <SDL.h>
#include <random>
// Game specific includes
#include "asteroidsGame.h"

using namespace std;

class cGame
{
public:
	cGame();

	void initialise(SDL_Window* theSDLWND, SDL_Renderer* theRenderer);
	void run(SDL_Window* theSDLWND, SDL_Renderer* theRenderer);
	void cleanUp(SDL_Window* theSDLWND);
	void render(SDL_Window* theSDLWND, SDL_Renderer* theRenderer);
	void render(SDL_Window* theSDLWND, SDL_Renderer* theRenderer, double rotAngle, SDL_Point* spriteCentre);
	void update();
	void update(double deltaTime);
	bool getInput(bool theLoop);
	double getElapsedSeconds();

	void createLevel1();

	static cGame* getInstance();

private:

	static cGame* pInstance;
	// for framerates
	time_point< high_resolution_clock > m_lastTime;
	time_point< high_resolution_clock > m_CurrentTime;
	duration< double > deltaTime;
	bool loop = false;

	// Sprites for displaying background and rocket textures
	cSprite spriteBkgd;
	cSprite wallTextureTop;
	cSprite wallTextureLeft;
	cSprite wallTextureRight;
	cSprite lifeTexture;
	cRocket thePaddle;
	cAsteroid theAsteroid;
	cBullet theBullet;
	// game related variables
	vector<LPCSTR> textureName;
	vector<LPCSTR> textName;
	vector<LPCSTR> texturesToUse;
	vector<cAsteroid*> theAsteroids;
	vector<cBullet*> theBullets;
	vector<cSprite*> theExplosions;
	vector<cSprite> vecLifesLeft;
	// Fonts to use
	vector<LPCSTR> fontList;
	vector<LPCSTR> fontsToUse;
	// Text for Game
	vector<LPCSTR> gameTextList;
	// Game Sounds
	vector<LPCSTR> soundList;
	vector<soundType> soundTypes;
	vector<LPCSTR> soundsToUse;
	// Create vector array of button textures
	vector<LPCSTR> btnNameList;
	vector<LPCSTR> btnTexturesToUse;
	vector<SDL_Point> btnPos;
	vector <cButton> theButtons;
	btnTypes theBtnType;
	// Game objects
	// Define the elements and there position in/on the array/map
	int renderWidth, renderHeight;
	int theScore;
	string strScore;
	int wallNumberTop;
	int wallNumberSide;
	gameState theGameState;
	SDL_Point theAreaClicked;
	cHighScoreTable theHSTable;
	int numTableItems;
	string theHeighScoreTable;
	int theHSTableSize;
	vector<LPCSTR> highScoreTextures;
};

#endif
