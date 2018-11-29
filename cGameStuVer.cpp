/*
==================================================================================
cGame.cpp
==================================================================================
*/
#include "cGame.h"

cGame* cGame::pInstance = NULL;
static cTextureMgr* theTextureMgr = cTextureMgr::getInstance();
static cFontMgr* theFontMgr = cFontMgr::getInstance();
static cSoundMgr* theSoundMgr = cSoundMgr::getInstance();

string totScore = "";
double timePassed = 0;
int timeLeft = 10;
int lifesLeft = 3;
string stringTime = "";

SDL_Renderer* theRend;


/*
=================================================================================
Constructor
=================================================================================
*/
cGame::cGame()
{

}
/*
=================================================================================
Singleton Design Pattern
=================================================================================
*/
cGame* cGame::getInstance()
{
	if (pInstance == NULL)
	{
		pInstance = new cGame();
	}
	return cGame::pInstance;
}

void cGame::initialise(SDL_Window* theSDLWND, SDL_Renderer* theRenderer)
{
	// Get width and height of render context
	theRend = theRenderer;
	SDL_GetRendererOutputSize(theRenderer, &renderWidth, &renderHeight);
	this->m_lastTime = high_resolution_clock::now();

	// Clear the buffer with a black background
	SDL_SetRenderDrawColor(theRenderer, 0, 0, 0, 255);
	SDL_RenderPresent(theRenderer);
	
	/* Let the computer pick a random number */
	random_device rd;    // non-deterministic engine 
	mt19937 gen{ rd() }; // deterministic engine. For most common uses, std::mersenne_twister_engine, fast and high-quality.
	uniform_int_distribution<> AsteroidDis{ 1, 10 };
	uniform_int_distribution<> AsteroidTextDis{ 4, 14 };

	theTextureMgr->setRenderer(theRenderer);
	theFontMgr->initFontLib();
	theSoundMgr->initMixer();
	theScore = 0;

	// Store the textures
	textureName = { "background", "paddle_s", "paddle_m", "paddle_l", // background and paddles - 4
					"brick_black", "brick_white", "brick_green", "brick_green_dmg1", "brick_orange", "brick_orange_dmg1", "brick_orange_dmg2", "brick_red", "brick_red_dmg1", "brick_red_dmg2", "brick_red_dmg3", // bricks - 11
					"pick_up_effect", "ball", "bonus_balls", "bonus_bigger", "bonus_score", "bonus_smaller", "bonus_speed", "bonus_time", // pick ups - 8
					"life", "score", "time", "wall_left", "wall_right", "wall_up" }; // additional - 6
	texturesToUse = { "Images\\Bkg\\Background1280x1280.jpg", "Images\\Sprites\\paddle_s.png", "Images\\Sprites\\paddle_m.png", "Images\\Sprites\\paddle_l.png", // background and paddles - 4
					  "Images\\Sprites\\brick_black.png", "Images\\Sprites\\brick_white.png", "Images\\Sprites\\brick_green.png", "Images\\Sprites\\brick_green_dmg1.png", "Images\\Sprites\\brick_orange.png", "Images\\Sprites\\brick_orange_dmg1.png", "Images\\Sprites\\brick_orange_dmg2.png", "Images\\Sprites\\brick_red.png", "Images\\Sprites\\brick_red_dmg1.png", "Images\\Sprites\\brick_red_dmg2.png", "Images\\Sprites\\brick_red_dmg3.png", // bricks - 11
					  "Images\\Sprites\\pick_up_effect.png", "Images\\Sprites\\ball.png", "Images\\Sprites\\bonus_balls.png", "Images\\Sprites\\bonus_bigger.png", "Images\\Sprites\\bonus_score.png", "Images\\Sprites\\bonus_smaller.png", "Images\\Sprites\\bonus_speed.png", "Images\\Sprites\\bonus_time.png", // pick ups - 8
					  "Images\\Sprites\\life.png", "Images\\Sprites\\score.png", "Images\\Sprites\\time.png", "Images\\Sprites\\wall_left.png", "Images\\Sprites\\wall_right.png", "Images\\Sprites\\wall_up.png" }; // additional - 6

	for (int tCount = 0; tCount < (int)textureName.size(); tCount++)
	{	
		theTextureMgr->addTexture(textureName[tCount], texturesToUse[tCount]);
	}
	// Create textures for Game Dialogue (text)
	fontList = { "Main", "Secondary" };
	fontsToUse = { "Fonts/Freshman.ttf", "Fonts/Berlin_Sans.ttf" };
	for (int fonts = 0; fonts < (int)fontList.size(); fonts++)
	{
		theFontMgr->addFont(fontList[fonts], fontsToUse[fonts], 36);
	}
	gameTextList = {"Time: ", "Score: ", "Brick Breaker", "Destroy all the bricks before you ran out of time!", "Use arrow keys or A / D to move the paddle.", "Leaderboard", "Player: score"};
	strScore = gameTextList[1];
	stringTime = gameTextList[0];
	strScore += to_string(theScore).c_str();

	theTextureMgr->addTexture("Time", theFontMgr->getFont("Main")->createTextTexture(theRenderer, stringTime.c_str(), textType::solid, { 255, 255, 255, 255 }, { 0, 0, 0, 0 }));
	theTextureMgr->addTexture("theScore", theFontMgr->getFont("Main")->createTextTexture(theRenderer, strScore.c_str(), textType::solid, { 255, 255, 255, 255 }, { 0, 0, 0, 0 }));
	theTextureMgr->addTexture("Title", theFontMgr->getFont("Main")->createTextTexture(theRenderer, gameTextList[2], textType::solid, { 255, 255, 255, 255 }, { 0, 0, 0, 0 }));
	theTextureMgr->addTexture("Objective", theFontMgr->getFont("Secondary")->createTextTexture(theRenderer, gameTextList[3], textType::solid, { 255, 255, 255, 255 }, { 0, 0, 0, 0 }));
	theTextureMgr->addTexture("Instructions", theFontMgr->getFont("Secondary")->createTextTexture(theRenderer, gameTextList[4], textType::solid, { 255, 255, 255, 255 }, { 0, 0, 0, 0 }));
	theTextureMgr->addTexture("Leaderboard", theFontMgr->getFont("Main")->createTextTexture(theRenderer, gameTextList[5], textType::solid, { 255, 255, 255, 255 }, { 0, 0, 0, 0 }));
	theTextureMgr->addTexture("PlayerScore", theFontMgr->getFont("Secondary")->createTextTexture(theRenderer, gameTextList[6], textType::solid, { 255, 255, 255, 255 }, { 0, 0, 0, 0 }));

	// Load game sounds
	soundList = { "theme1", "theme2", "theme3", "theme4", "pick_up_appeared", "pick_up_sound", "ball_hit", "life_lost", "countdown", "button_click" };
	soundTypes = { soundType::music, soundType::music, soundType::music, soundType::music, soundType::sfx, soundType::sfx, soundType::sfx, soundType::sfx, soundType::sfx, soundType::sfx };
	soundsToUse = { "Audio/music_1.wav", "Audio/music_2.wav", "Audio/music_3.wav", "Audio/music_4.wav", "Audio/pick_up_appeared.wav", "Audio/pick_up_sound.wav", "Audio/ball_hit.wav", "Audio/life_lost.wav", "Audio/countdown.wav", "Audio/button_click.wav" };
	for (int sounds = 0; sounds < (int)soundList.size(); sounds++)
	{
		theSoundMgr->add(soundList[sounds], soundsToUse[sounds], soundTypes[sounds]);
	}

	theSoundMgr->getSnd("theme1")->play(-1);

	spriteBkgd.setSpritePos({ 0, 0 });
	spriteBkgd.setTexture(theTextureMgr->getTexture("background"));
	spriteBkgd.setSpriteDimensions(theTextureMgr->getTexture("background")->getTWidth(), theTextureMgr->getTexture("background")->getTHeight());

	thePaddle.setTexture(theTextureMgr->getTexture("paddle_s"));
	thePaddle.setSpriteDimensions(theTextureMgr->getTexture("paddle_s")->getTWidth(), theTextureMgr->getTexture("paddle_s")->getTHeight());
	// placing the paddle sprite in the middle bottom of the screen
	thePaddle.setSpritePos({ 500, (int)(1280 - thePaddle.getSpriteDimensions().h * 1.5) });
	thePaddle.setRocketVelocity(200);
	thePaddle.setRocketMaxSpeed(700);
	thePaddle.setSpriteTranslation({ 50,50 });

	// adding up and side wall textures
	wallTextureLeft.setTexture(theTextureMgr->getTexture("wall_left"));
	wallTextureLeft.setSpriteDimensions(theTextureMgr->getTexture("wall_left")->getTWidth(), theTextureMgr->getTexture("wall_left")->getTHeight());
	wallTextureRight.setTexture(theTextureMgr->getTexture("wall_right"));
	wallTextureRight.setSpriteDimensions(theTextureMgr->getTexture("wall_right")->getTWidth(), theTextureMgr->getTexture("wall_right")->getTHeight());
	wallTextureTop.setTexture(theTextureMgr->getTexture("wall_up"));
	wallTextureTop.setSpriteDimensions(theTextureMgr->getTexture("wall_up")->getTWidth(), theTextureMgr->getTexture("wall_up")->getTHeight());
	
	lifeTexture.setTexture(theTextureMgr->getTexture("life"));
	lifeTexture.setSpriteDimensions(theTextureMgr->getTexture("life")->getTWidth(), theTextureMgr->getTexture("life")->getTHeight());
	// some kind of weird stuff ???
	//lifeTexture.setSpritePos({ 0 , theTextureMgr->getTexture("life")->getTHeight() });

	// adding life textures
	/*for (int i = 0; i < lifesLeft; i++)
	{
		theLifes[i]->setTexture(theTextureMgr->getTexture("life"));
		theLifes[i]->setSpriteDimensions(theTextureMgr->getTexture("life")->getTWidth(), theTextureMgr->getTexture("life")->getTHeight());
		theLifes[i]->setSpritePos({theTextureMgr->getTexture("life")->getTWidth(), theTextureMgr->getTexture("life")->getTHeight() + 4});
	}*/
	
	// Create vector array of textures
	/*
	for (int astro = 0; astro < 10; astro++)
	{
		theAsteroids.push_back(new cAsteroid);
		theAsteroids[astro]->setSpritePos({ 150 * AsteroidDis(gen), wallTextureTop.getSpriteDimensions().h + 50 * AsteroidDis(gen) });
		theAsteroids[astro]->setSpriteTranslation({ 100, -50 });
		int randAsteroid = AsteroidTextDis(gen);
		theAsteroids[astro]->setTexture(theTextureMgr->getTexture(textureName[randAsteroid]));
		theAsteroids[astro]->setSpriteDimensions(theTextureMgr->getTexture(textureName[randAsteroid])->getTWidth(), theTextureMgr->getTexture(textureName[randAsteroid])->getTHeight());
		theAsteroids[astro]->setAsteroidVelocity(200);
		theAsteroids[astro]->setActive(true);
	}*/

	// FIRST LEVEL DESIGN //
	int brickCount = 0;
	
	// The layout of the bricks will be 16 columns (X), 8 rows (Y).
	for (int x = 0; x < 16; x++)
	{
		for (int y = 0; y < 8; y++)
		{
			theAsteroids.push_back(new cAsteroid);

			// all bricks have same dimensions
			int tempDimensionW = theTextureMgr->getTexture("brick_white")->getTWidth();
			int tempDimensionH = theTextureMgr->getTexture("brick_white")->getTHeight();

			// first two rows are red
			if (y < 2)
			{
				theAsteroids[brickCount]->setTexture(theTextureMgr->getTexture("brick_red"));
				theAsteroids[brickCount]->health = 4;
				theAsteroids[brickCount]->colour = "red";
			}

			// next two rows are orange
			if (y < 4 && y >= 2)
			{
				theAsteroids[brickCount]->setTexture(theTextureMgr->getTexture("brick_orange"));
				theAsteroids[brickCount]->health = 3;
				theAsteroids[brickCount]->colour = "orange";
			}

			// next two rows are green
			if (y < 6 && y >= 4)
			{
				theAsteroids[brickCount]->setTexture(theTextureMgr->getTexture("brick_green"));
				theAsteroids[brickCount]->health = 2;
				theAsteroids[brickCount]->colour = "green";
			}

			// and last two rows (closest to player) are white
			if (y < 8 && y >= 6)
			{
				theAsteroids[brickCount]->setTexture(theTextureMgr->getTexture("brick_white"));
				theAsteroids[brickCount]->health = 1;
				theAsteroids[brickCount]->colour = "white";
			}

			theAsteroids[brickCount]->setSpriteDimensions(tempDimensionW, tempDimensionH);
			theAsteroids[brickCount]->setSpritePos({ wallTextureLeft.getSpriteDimensions().w + (int)(tempDimensionW * x)  , wallTextureTop.getSpriteDimensions().h + (int)(tempDimensionH * y) });
			theAsteroids[brickCount]->setActive(true);
			brickCount++;
		}
	}
	
	/*
	// SECOND LEVEL DESIGN
	for (int x = 0; x < 16; x++)
	{
		for (int y = 0; y < 10; y++)
		{
			theAsteroids.push_back(new cAsteroid);

			// all bricks have same dimensions
			int tempDimensionW = theTextureMgr->getTexture("brick_white")->getTWidth();
			int tempDimensionH = theTextureMgr->getTexture("brick_white")->getTHeight();
			
			// first brick border is white (all bricks that touch the wall are white)
			//if (y == 0 || y == 9 || x == 0 || x == 15)
			if (y == 0 || y == 9)
			{
				theAsteroids[brickCount]->setTexture(theTextureMgr->getTexture("brick_white"));
				theAsteroids[brickCount]->health = 1;
				theAsteroids[brickCount]->colour = "white";
			}

			// inner bricks are green
			//if (y == 1 && x > 0 && x < 15 || y == 8 && x > 0 && x < 15)
			if (y == 1 || y == 8)
			{
				theAsteroids[brickCount]->setTexture(theTextureMgr->getTexture("brick_green"));
				theAsteroids[brickCount]->health = 2;
				theAsteroids[brickCount]->colour = "green";
			}

			// another inner row is orange
			//if (y == 2 && x > 1 && x < 14 || y == 7 && x > 1 && x < 14)
			if (y == 2 || y == 7)
			{
				theAsteroids[brickCount]->setTexture(theTextureMgr->getTexture("brick_orange"));
				theAsteroids[brickCount]->health = 3;
				theAsteroids[brickCount]->colour = "orange";
			}

			// another inner row is orange
			//if (y == 3 && x > 2 && x < 13 || y == 6 && x > 2 && x < 13)
			if (y == 3 || y == 6)
			{
				theAsteroids[brickCount]->setTexture(theTextureMgr->getTexture("brick_red"));
				theAsteroids[brickCount]->health = 4;
				theAsteroids[brickCount]->colour = "red";
			}

			// another inner row is orange
			//if (y == 3 && x > 2 && x < 13 || y == 6 && x > 2 && x < 13)
			if (y == 4 || y == 5)
			{
				theAsteroids[brickCount]->setTexture(theTextureMgr->getTexture("brick_black"));
				theAsteroids[brickCount]->health = 0;
				theAsteroids[brickCount]->colour = "black";
			}

			theAsteroids[brickCount]->setSpriteDimensions(tempDimensionW, tempDimensionH);
			theAsteroids[brickCount]->setSpritePos({ wallTextureLeft.getSpriteDimensions().w + (int)(tempDimensionW * x)  , wallTextureTop.getSpriteDimensions().h + (int)(tempDimensionH * y) });
			theAsteroids[brickCount]->setActive(true);
			brickCount++;
		}
	}
	*/
	/*
	// THIRD LEVEL DESIGN
	for (int x = 0; x < 14; x++)
	{
		// empty space (2 rows) between top wall
		for (int y = 3; y < 12; y++)
		{
			theAsteroids.push_back(new cAsteroid);
			
			// all bricks have same dimensions
			int tempDimensionW = theTextureMgr->getTexture("brick_white")->getTWidth();
			int tempDimensionH = theTextureMgr->getTexture("brick_white")->getTHeight();
			
			// two red rows
			if (y < 5 && y >= 3)
			{
				theAsteroids[brickCount]->setTexture(theTextureMgr->getTexture("brick_red"));
				theAsteroids[brickCount]->health = 4;
				theAsteroids[brickCount]->colour = "red";
			}

			// two orange rows
			if (y < 7 && y >= 5)
			{
				theAsteroids[brickCount]->setTexture(theTextureMgr->getTexture("brick_orange"));
				theAsteroids[brickCount]->health = 3;
				theAsteroids[brickCount]->colour = "orange";
			}

			// two green rows
			if (y < 9 && y >= 7)
			{
				theAsteroids[brickCount]->setTexture(theTextureMgr->getTexture("brick_green"));
				theAsteroids[brickCount]->health = 2;
				theAsteroids[brickCount]->colour = "green";
			}

			// two white rows
			if (y < 11 && y >= 9)
			{
				theAsteroids[brickCount]->setTexture(theTextureMgr->getTexture("brick_white"));
				theAsteroids[brickCount]->health = 1;
				theAsteroids[brickCount]->colour = "white";
			}

			// last row is black (closest to player)
			if(y >= 11)
			{
				theAsteroids[brickCount]->setTexture(theTextureMgr->getTexture("brick_black"));
				theAsteroids[brickCount]->health = 0;
				theAsteroids[brickCount]->colour = "black";
			}

			if (x >= 13)
			{
				theAsteroids[brickCount]->setTexture(theTextureMgr->getTexture("brick_black"));
				theAsteroids[brickCount]->health = 0;
				theAsteroids[brickCount]->colour = "black";
			}

			theAsteroids[brickCount]->setSpriteDimensions(tempDimensionW, tempDimensionH);
			theAsteroids[brickCount]->setSpritePos({ wallTextureLeft.getSpriteDimensions().w + (int)(tempDimensionW * x)  , wallTextureTop.getSpriteDimensions().h + (int)(tempDimensionH * y) });
			theAsteroids[brickCount]->setActive(true);
			brickCount++;
		}
	}*/

	// calculating how many wall textures fit in screen
	wallNumberTop = (int)(1280 / wallTextureTop.getSpriteDimensions().w) + 1;
	wallNumberSide = (int)(1280 / wallTextureLeft.getSpriteDimensions().h);

	// passing wall's width to cRocket
	thePaddle.setMovingWidth(wallTextureLeft.getSpriteDimensions().w);
}

void cGame::run(SDL_Window* theSDLWND, SDL_Renderer* theRenderer)
{
	bool loop = true;

	while (loop)
	{
		//We get the time that passed since the last frame
		double elapsedTime = this->getElapsedSeconds();

		loop = this->getInput(loop);
		this->update(elapsedTime);
		this->render(theSDLWND, theRenderer);
	}
}

void cGame::render(SDL_Window* theSDLWND, SDL_Renderer* theRenderer)
{
	SDL_RenderClear(theRenderer);
	
	switch (theGameState)
	{
	case gameState::menu:
	{
		// Background for menu
		spriteBkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteScale());

		// Title
		cTexture* tempTextTexture = theTextureMgr->getTexture("Title");
		SDL_Rect pos = { (1280 / 2) - (tempTextTexture->getTextureRect().w), 100, tempTextTexture->getTextureRect().w * 2, tempTextTexture->getTextureRect().h * 2 };
		FPoint scale = { 1, 1 };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);

		// Objective
		tempTextTexture = theTextureMgr->getTexture("Objective");
		pos = { (1280 / 2) - (tempTextTexture->getTextureRect().w / 2), 200, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		scale = { 1 , 1 };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		
		// Instructions
		tempTextTexture = theTextureMgr->getTexture("Instructions");
		pos = { (1280 / 2) - (tempTextTexture->getTextureRect().w / 2), 250, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);

		/* BUTTONS TO ADD
		theButtonMgr->getBtn("play_btn")->render(theRenderer, &theButtonMgr->getBtn("play_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("play_btn")->getSpritePos(), theButtonMgr->getBtn("play_btn")->getSpriteScale());
		theButtonMgr->getBtn("exit_btn")->setSpritePos({ 400, 375 });
		theButtonMgr->getBtn("exit_btn")->render(theRenderer, &theButtonMgr->getBtn("exit_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("exit_btn")->getSpritePos(), theButtonMgr->getBtn("exit_btn")->getSpriteScale());
		*/
	}break;
	case gameState::playing:
	{
		// Ingame background
		spriteBkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteScale());

		// Create side walls
		for (int i = 0; i < wallNumberSide; i++)
		{
			// starting point left side and just under the upper wall
			wallTextureLeft.setSpritePos({ 0 , wallTextureTop.getSpriteDimensions().h + i*wallTextureLeft.getSpriteDimensions().h });
			wallTextureLeft.render(theRenderer, &wallTextureLeft.getSpriteDimensions(), &wallTextureLeft.getSpritePos(), wallTextureLeft.getSpriteRotAngle(), &wallTextureLeft.getSpriteCentre(), wallTextureLeft.getSpriteScale());

			// starting point right side (screen width - texture width) and just under the upper wall
			wallTextureRight.setSpritePos({ 1280 - wallTextureRight.getSpriteDimensions().w, wallTextureTop.getSpriteDimensions().h + i*wallTextureRight.getSpriteDimensions().h });
			wallTextureRight.render(theRenderer, &wallTextureRight.getSpriteDimensions(), &wallTextureRight.getSpritePos(), wallTextureRight.getSpriteRotAngle(), &wallTextureRight.getSpriteCentre(), wallTextureRight.getSpriteScale());
		}

		// some kind of weird stuff ????
		for (int i = 0; i < lifesLeft; i++)
		{
			lifeTexture.setSpritePos({ lifeTexture.getSpriteDimensions().w + i * lifeTexture.getSpriteDimensions().w, lifeTexture.getSpriteDimensions().h });
			lifeTexture.render(theRenderer, &lifeTexture.getSpriteDimensions(), &lifeTexture.getSpritePos(), lifeTexture.getSpriteRotAngle(), &lifeTexture.getSpriteCentre(), lifeTexture.getSpriteScale());
		}
	
		// Create top wall
		for (int i = 0; i < wallNumberTop; i++)
		{
			wallTextureTop.setSpritePos({ i*wallTextureTop.getSpriteDimensions().w, 0 });
			wallTextureTop.render(theRenderer, &wallTextureTop.getSpriteDimensions(), &wallTextureTop.getSpritePos(), wallTextureTop.getSpriteRotAngle(), &wallTextureTop.getSpriteCentre(), wallTextureTop.getSpriteScale());
		}

		// Render each asteroid in the vector array
		for (int draw = 0; draw < (int)theAsteroids.size(); draw++)
		{
			theAsteroids[draw]->render(theRenderer, &theAsteroids[draw]->getSpriteDimensions(), &theAsteroids[draw]->getSpritePos(), theAsteroids[draw]->getSpriteRotAngle(), &theAsteroids[draw]->getSpriteCentre(), theAsteroids[draw]->getSpriteScale());
		}

		// Render each bullet in the vector array
		for (int draw = 0; draw < (int)theBullets.size(); draw++)
		{
			theBullets[draw]->render(theRenderer, &theBullets[draw]->getSpriteDimensions(), &theBullets[draw]->getSpritePos(), theBullets[draw]->getSpriteRotAngle(), &theBullets[draw]->getSpriteCentre(), theBullets[draw]->getSpriteScale());
		}

		// Render each explosion in the vector array
		for (int draw = 0; draw < (int)theExplosions.size(); draw++)
		{
			theExplosions[draw]->render(theRenderer, &theExplosions[draw]->getSourceRect(), &theExplosions[draw]->getSpritePos(), theExplosions[draw]->getSpriteScale());
		}
	
		// Lab 7 code goes here
		cTexture* tempTextTexture2 = theTextureMgr->getTexture("Time");
		SDL_Rect pos3 = { (1280 / 2) - (tempTextTexture2->getTextureRect().w / 2), 4, tempTextTexture2->getTextureRect().w, tempTextTexture2->getTextureRect().h };
		FPoint scale3 = { 1, 1 };
		tempTextTexture2->renderTexture(theRenderer, tempTextTexture2->getTexture(), &tempTextTexture2->getTextureRect(), &pos3, scale3);
		theTextureMgr->addTexture("Time", theFontMgr->getFont("Main")->createTextTexture(theRenderer, gameTextList[1], textType::solid, { 255, 255, 255, 255 }, { 0, 0, 0, 0 }));

		cTexture* tempTextTexture3 = theTextureMgr->getTexture("theScore");
		SDL_Rect pos2 = { 1280 - tempTextTexture3->getTextureRect().w, 4, tempTextTexture3->getTextureRect().w, tempTextTexture3->getTextureRect().h };
		FPoint scale2 = { 1, 1 };
		tempTextTexture3->renderTexture(theRenderer, tempTextTexture3->getTexture(), &tempTextTexture3->getTextureRect(), &pos2, scale2);
		theTextureMgr->addTexture("theScore", theFontMgr->getFont("Main")->createTextTexture(theRenderer, strScore.c_str(), textType::solid, { 255, 255, 255, 255 }, { 0, 0, 0, 0 }));

		// Render the paddle
		thePaddle.render(theRenderer, &thePaddle.getSpriteDimensions(), &thePaddle.getSpritePos(), thePaddle.getSpriteRotAngle(), &thePaddle.getSpriteCentre(), thePaddle.getSpriteScale());
		SDL_RenderPresent(theRenderer);
	}break;
	case gameState::end:
	{
		// End menu background
		spriteBkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteScale());
		
		// Title
		cTexture* tempTextTexture = theTextureMgr->getTexture("Title");
		SDL_Rect pos = { (1280 / 2) - (tempTextTexture->getTextureRect().w / 2), 4, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h * 2 };
		FPoint scale = { 1, 1 };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);

		// Objective
		tempTextTexture = theTextureMgr->getTexture("Leaderboard");
		pos = { (1280 / 2) - (tempTextTexture->getTextureRect().w / 2), 4, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h * 4 };
		scale = { 1 , 1 };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);

		// Instructions
		tempTextTexture = theTextureMgr->getTexture("PlayerScore");
		pos = { (1280 / 2) - (tempTextTexture->getTextureRect().w / 2), 4, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h * 6 };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);

		/* BUTTONS TO ADD
		theButtonMgr->getBtn("playAgain_btn")->setSpritePos({ 400, 375 });
		theButtonMgr->getBtn("playAgain_btn")->render(theRenderer, &theButtonMgr->getBtn("play_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("play_btn")->getSpritePos(), theButtonMgr->getBtn("play_btn")->getSpriteScale());
		theButtonMgr->getBtn("menu_btn")->setSpritePos({ 400, 375 });
		theButtonMgr->getBtn("menu_btn")->render(theRenderer, &theButtonMgr->getBtn("play_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("play_btn")->getSpritePos(), theButtonMgr->getBtn("play_btn")->getSpriteScale());
		theButtonMgr->getBtn("exit_btn")->setSpritePos({ 400, 375 });
		theButtonMgr->getBtn("exit_btn")->render(theRenderer, &theButtonMgr->getBtn("exit_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("exit_btn")->getSpritePos(), theButtonMgr->getBtn("exit_btn")->getSpriteScale());
		*/
	}break;
	case gameState::quit:
	{
		loop = false;
	}break;
	default:
		break;
	}
	SDL_RenderPresent(theRenderer);
}

void cGame::render(SDL_Window* theSDLWND, SDL_Renderer* theRenderer, double rotAngle, SDL_Point* spriteCentre)
{
	SDL_RenderPresent(theRenderer);
}

void cGame::update()
{

}

void cGame::update(double deltaTime)
{
	// Update the visibility and position of each asteriod
	vector<cAsteroid*>::iterator asteroidIterator = theAsteroids.begin();
	while (asteroidIterator != theAsteroids.end())
	{
		if ((*asteroidIterator)->isActive() == false)
		{
			asteroidIterator = theAsteroids.erase(asteroidIterator);
		}
		else
		{
			(*asteroidIterator)->update(deltaTime);
			++asteroidIterator;
		}
	}

	// Update the visibility and position of each bullet
	vector<cBullet*>::iterator bulletIterartor = theBullets.begin();
	while (bulletIterartor != theBullets.end())
	{
		if ((*bulletIterartor)->isActive() == false)
		{
			bulletIterartor = theBullets.erase(bulletIterartor);
		}
		else
		{
			(*bulletIterartor)->update(deltaTime);
			++bulletIterartor;
		}
	}

	// Update the visibility and position of each explosion
	vector<cSprite*>::iterator expIterartor = theExplosions.begin();
	while (expIterartor != theExplosions.end())
	{
		if ((*expIterartor)->isActive() == false)
		{
			expIterartor = theExplosions.erase(expIterartor);
		}
		else
		{
			(*expIterartor)->animate(deltaTime);
			++expIterartor;
		}
	}

	/*
	==============================================================
	| Check for collisions
	==============================================================
	*/
	for (vector<cBullet*>::iterator bulletIterartor = theBullets.begin(); bulletIterartor != theBullets.end(); ++bulletIterartor)
	{
		(*bulletIterartor)->update(deltaTime);

		for (vector<cAsteroid*>::iterator asteroidIterator = theAsteroids.begin(); asteroidIterator != theAsteroids.end(); ++asteroidIterator)
		{
			if ((*asteroidIterator)->collidedWith(&(*asteroidIterator)->getBoundingRect(), &(*bulletIterartor)->getBoundingRect()))
			{
				(*asteroidIterator)->health--;
				(*bulletIterartor)->bulletDirectionY *= -1;

				// if brick is red
				if ((*asteroidIterator)->colour == "red")
				{
					if ((*asteroidIterator)->health == 3)
					{
						(*asteroidIterator)->setTexture(theTextureMgr->getTexture("brick_red_dmg1"));
					} else if ((*asteroidIterator)->health == 2)
					{
						(*asteroidIterator)->setTexture(theTextureMgr->getTexture("brick_red_dmg2"));
					} else if ((*asteroidIterator)->health == 1)
					{
						(*asteroidIterator)->setTexture(theTextureMgr->getTexture("brick_red_dmg3"));
					} else if ((*asteroidIterator)->health == 0)
						theScore += 100;
				}

				// if brick is orange
				if ((*asteroidIterator)->colour == "orange")
				{
					if ((*asteroidIterator)->health == 2)
					{
						(*asteroidIterator)->setTexture(theTextureMgr->getTexture("brick_orange_dmg1"));
					}
					else if ((*asteroidIterator)->health == 1)
					{
						(*asteroidIterator)->setTexture(theTextureMgr->getTexture("brick_orange_dmg2"));
					} else if ((*asteroidIterator)->health == 0)
						theScore += 50;
				}

				// if brick is green
				if ((*asteroidIterator)->colour == "green")
				{
					if ((*asteroidIterator)->health == 1)
					{
						(*asteroidIterator)->setTexture(theTextureMgr->getTexture("brick_green_dmg1"));
					} else if ((*asteroidIterator)->health == 0)
						theScore += 25;
				}

				// if brick is white
				if ((*asteroidIterator)->colour == "white")
				{
					if ((*asteroidIterator)->health == 0)
						theScore += 10;
				}

				if ((*asteroidIterator)->health == 0)
				{
					(*asteroidIterator)->setActive(false);
					theTextureMgr->deleteTexture("theScore");
					strScore = "Score: ";
					strScore += to_string(theScore).c_str();
					theTextureMgr->addTexture("theScore", theFontMgr->getFont("Main")->createTextTexture(theRend, strScore.c_str(), textType::solid, { 255, 255, 255, 255 }, { 0, 0, 0, 0 }));
				}

				// console view of health
				cout << (*asteroidIterator)->health << endl;

				theExplosions.push_back(new cSprite);
				int index = theExplosions.size()-1;
				theExplosions[index]->setSpriteTranslation({ 0, 0 });
				theExplosions[index]->setActive(true);
				theExplosions[index]->setNoFrames(6);
				theExplosions[index]->setTexture(theTextureMgr->getTexture("pick_up_effect"));
				theExplosions[index]->setSpriteDimensions(theTextureMgr->getTexture("pick_up_effect")->getTWidth()/ theExplosions[index]->getNoFrames(), theTextureMgr->getTexture("pick_up_effect")->getTHeight());
				theExplosions[index]->setSpritePos({ (*asteroidIterator)->getSpritePos().x + (int)((*asteroidIterator)->getSpritePos().w/2), (*asteroidIterator)->getSpritePos().y + (int)((*asteroidIterator)->getSpritePos().h / 2) });

				theSoundMgr->getSnd("pick_up_sound")->play(0);
			}
		}

		// when bullet collides with the platform
		if ((*bulletIterartor)->collidedWith(&(thePaddle).getBoundingRect(), &(*bulletIterartor)->getBoundingRect()))
		{
			(*bulletIterartor)->bulletDirectionY = 1;
		}
	}
	
	timePassed += deltaTime;
	//cout << (int)timePassed << endl;

	/*
	timeLeft -= timePassed;
	cout << timeLeft << endl;
	*/

	// Update timer
	/*theTextureMgr->deleteTexture("Time");
	stringTime = "Time: ";
	stringTime += to_string((int)timePassed).c_str();
	theTextureMgr->addTexture("Time", theFontMgr->getFont("Main")->createTextTexture(theRend, stringTime.c_str(), textType::solid, { 255, 255, 255, 255 }, { 0, 0, 0, 0 }));
	*/
	// Update the paddle's position
	thePaddle.update(deltaTime);
}

bool cGame::getInput(bool theLoop)
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
		{
			theLoop = false;
		}

		switch (event.type)
		{
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					theLoop = false;
					break;
				case SDLK_SPACE:
				{
					theBullets.push_back(new cBullet);
					int numBullets = theBullets.size() - 1;
					theBullets[numBullets]->setSpritePos({ thePaddle.getBoundingRect().x + thePaddle.getSpriteCentre().x, thePaddle.getBoundingRect().y + thePaddle.getSpriteCentre().y });
					theBullets[numBullets]->setSpriteTranslation({ 50, 50 });
					theBullets[numBullets]->setTexture(theTextureMgr->getTexture("ball"));
					theBullets[numBullets]->setSpriteDimensions(theTextureMgr->getTexture("ball")->getTWidth(), theTextureMgr->getTexture("ball")->getTHeight());
					theBullets[numBullets]->setBulletVelocity(5);
					// passing walls' dimensions to cBulletStuVer
					theBullets[numBullets]->setMovingPoints(wallTextureLeft.getSpriteDimensions().w, wallTextureTop.getSpriteDimensions().h);
					theBullets[numBullets]->setSpriteRotAngle(-50);
					theBullets[numBullets]->setActive(true);
					cout << "Bullet added to Vector at position - x: " << thePaddle.getBoundingRect().x << " y: " << thePaddle.getBoundingRect().y << endl;
				}
				theSoundMgr->getSnd("ball_hit")->play(0);
				break;
				default:
					break;
				}
		}
		
	}
	return theLoop;
}

double cGame::getElapsedSeconds()
{
	this->m_CurrentTime = high_resolution_clock::now();
	this->deltaTime = (this->m_CurrentTime - this->m_lastTime);
	this->m_lastTime = this->m_CurrentTime;
	return deltaTime.count();
}

void cGame::cleanUp(SDL_Window* theSDLWND)
{
	// Delete our OpengL context
	SDL_GL_DeleteContext(theSDLWND);

	// Destroy the window
	SDL_DestroyWindow(theSDLWND);

	//Quit FONT system
	TTF_Quit();

	// Quit IMG system
	IMG_Quit();

	// Shutdown SDL 2
	SDL_Quit();
}

