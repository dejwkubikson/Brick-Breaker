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
static cButtonMgr* theButtonMgr = cButtonMgr::getInstance();

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

void cGame::applyPickUpEffect(string effectToApply)
{
	if (effectToApply == "bonus_balls")
	{
		enabledSpace = true;
	}

	if (effectToApply == "bonus_bigger")
	{
		if (currentPaddleSize == "small")
		{
			//theTextureMgr->deleteTexture("paddle_s");
			thePaddle.setTexture(theTextureMgr->getTexture("paddle_m"));
			thePaddle.setSpriteDimensions(theTextureMgr->getTexture("paddle_m")->getTWidth(), theTextureMgr->getTexture("paddle_m")->getTHeight());
			currentPaddleSize = "medium";
		}

		if (currentPaddleSize == "medium")
		{
			//theTextureMgr->deleteTexture("paddle_m");
			thePaddle.setTexture(theTextureMgr->getTexture("paddle_l"));
			thePaddle.setSpriteDimensions(theTextureMgr->getTexture("paddle_l")->getTWidth(), theTextureMgr->getTexture("paddle_l")->getTHeight());
			currentPaddleSize = "large";
		}

		if (currentPaddleSize == "large")
			currentPaddleSize = "large";
	}

	if (effectToApply == "bonus_score")
	{
		if (scoreBoost == 0)
			scoreBoost = 1;
		else 
			scoreBoost *= 2;
	}

	if (effectToApply == "bonus_smaller")
	{
		if (currentPaddleSize == "large")
		{
			//theTextureMgr->deleteTexture("paddle_l");
			thePaddle.setTexture(theTextureMgr->getTexture("paddle_m"));
			thePaddle.setSpriteDimensions(theTextureMgr->getTexture("paddle_m")->getTWidth(), theTextureMgr->getTexture("paddle_m")->getTHeight());
			currentPaddleSize = "medium";
		}

		if (currentPaddleSize == "medium")
		{
			//theTextureMgr->deleteTexture("paddle_m");
			thePaddle.setTexture(theTextureMgr->getTexture("paddle_s"));
			thePaddle.setSpriteDimensions(theTextureMgr->getTexture("paddle_s")->getTWidth(), theTextureMgr->getTexture("paddle_s")->getTHeight());
			currentPaddleSize = "small";
		}

		if (currentPaddleSize == "small")
			currentPaddleSize = "small";
	}

	if (effectToApply == "bonus_speed")
	{
		// applying effect on every bullet
		for (int i = 0; i < theBullets.size(); i++)
		{
			theBullets[i]->setBulletVelocity(theBullets[i]->getBulletVelocity() * 2);
		}
	}

	if (effectToApply == "bonus_time")
	{
		if (timePassed < 30)
			timePassed = 0;
		else
			timePassed -= 30;
	}
}


// FIRST LEVEL DESIGN //
// The layout of the bricks will be 16 columns (X), 8 rows (Y).
void cGame::createLevel1()
{
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
	currentLevel = 1;
}

// Second LEVEL DESIGN
void cGame::createLevel2()
{
	for (int x = 0; x < 14; x++)
	{
		// empty space (2 rows) between top wall
		for (int y = 2; y < 12; y++)
		{
			theAsteroids.push_back(new cAsteroid);

			// all bricks have same dimensions
			int tempDimensionW = theTextureMgr->getTexture("brick_white")->getTWidth();
			int tempDimensionH = theTextureMgr->getTexture("brick_white")->getTHeight();

			// two red rows
			if (y < 4 && y >= 2)
			{
				theAsteroids[brickCount]->setTexture(theTextureMgr->getTexture("brick_red"));
				theAsteroids[brickCount]->health = 4;
				theAsteroids[brickCount]->colour = "red";
			}

			// two orange rows
			if (y < 6 && y >= 4)
			{
				theAsteroids[brickCount]->setTexture(theTextureMgr->getTexture("brick_orange"));
				theAsteroids[brickCount]->health = 3;
				theAsteroids[brickCount]->colour = "orange";
			}

			// two green rows
			if (y < 8 && y >= 6)
			{
				theAsteroids[brickCount]->setTexture(theTextureMgr->getTexture("brick_green"));
				theAsteroids[brickCount]->health = 2;
				theAsteroids[brickCount]->colour = "green";
			}

			// two white rows
			if (y < 10 && y >= 8)
			{
				theAsteroids[brickCount]->setTexture(theTextureMgr->getTexture("brick_white"));
				theAsteroids[brickCount]->health = 1;
				theAsteroids[brickCount]->colour = "white";
			}

			// last two rows are red (nearest player)
			if (y >= 10)
			{
				theAsteroids[brickCount]->setTexture(theTextureMgr->getTexture("brick_red"));
				theAsteroids[brickCount]->health = 4;
				theAsteroids[brickCount]->colour = "red";
			}

			// the right column is black
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
	}

	currentLevel = 2;
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

	theTextureMgr->setRenderer(theRenderer);
	theFontMgr->initFontLib();
	theSoundMgr->initMixer();
	theScore = 0;

	// Store the textures
	textureName = { "background", "paddle_s", "paddle_m", "paddle_l", // background and paddles - 4
					"brick_black", "brick_white", "brick_green", "brick_green_dmg1", "brick_orange", "brick_orange_dmg1", "brick_orange_dmg2", "brick_red", "brick_red_dmg1", "brick_red_dmg2", "brick_red_dmg3", // bricks - 11
					"pick_up_effect", "ball", "bonus_balls", "bonus_bigger", "bonus_score", "bonus_smaller", "bonus_speed", "bonus_time", // pick ups - 8
					"life", "wall_left", "wall_right", "wall_up", "leaderboard_background", "menu_background", "hit_effect" }; // additional - 7
	texturesToUse = { "Images\\Bkg\\Background1280x1280.jpg", "Images\\Sprites\\paddle_s.png", "Images\\Sprites\\paddle_m.png", "Images\\Sprites\\paddle_l.png", // background and paddles - 4
					  "Images\\Sprites\\brick_black.png", "Images\\Sprites\\brick_white.png", "Images\\Sprites\\brick_green.png", "Images\\Sprites\\brick_green_dmg1.png", "Images\\Sprites\\brick_orange.png", "Images\\Sprites\\brick_orange_dmg1.png", "Images\\Sprites\\brick_orange_dmg2.png", "Images\\Sprites\\brick_red.png", "Images\\Sprites\\brick_red_dmg1.png", "Images\\Sprites\\brick_red_dmg2.png", "Images\\Sprites\\brick_red_dmg3.png", // bricks - 11
					  "Images\\Sprites\\pick_up_effect.png", "Images\\Sprites\\ball.png", "Images\\Sprites\\bonus_balls.png", "Images\\Sprites\\bonus_bigger.png", "Images\\Sprites\\bonus_score.png", "Images\\Sprites\\bonus_smaller.png", "Images\\Sprites\\bonus_speed.png", "Images\\Sprites\\bonus_time.png", // pick ups - 8
					  "Images\\Sprites\\life.png", "Images\\Sprites\\wall_left.png", "Images\\Sprites\\wall_right.png", "Images\\Sprites\\wall_up.png", "Images\\Bkg\\Leaderboard.jpg", "Images\\Bkg\\Menu_background.jpg", "Images\\Sprites\\hit_effect.png" }; // additional - 9

	for (int tCount = 0; tCount < (int)textureName.size(); tCount++)
	{	
		theTextureMgr->addTexture(textureName[tCount], texturesToUse[tCount]);
	}

	// Store the textures
	btnNameList = { "exit_btn","menu_btn", "play_btn" };
	btnTexturesToUse = { "Images/Buttons/button_exit.png", "Images/Buttons/button_menu.png", "Images/Buttons/button_play.png" };
	btnPos = { { 660, 600 },{ 400, 600 },{ 460, 600 } };
	
	for (unsigned int bCount = 0; bCount < btnNameList.size(); bCount++)
	{
		theTextureMgr->addTexture(btnNameList[bCount], btnTexturesToUse[bCount]);
	}
	
	for (unsigned int bCount = 0; bCount < btnNameList.size(); bCount++)
	{
		cButton * newBtn = new cButton();
		newBtn->setTexture(theTextureMgr->getTexture(btnNameList[bCount]));
		newBtn->setSpritePos(btnPos[bCount]);
		newBtn->setSpriteDimensions(theTextureMgr->getTexture(btnNameList[bCount])->getTWidth(), theTextureMgr->getTexture(btnNameList[bCount])->getTHeight());
		theButtonMgr->add(btnNameList[bCount], newBtn);
	}

	theGameState = gameState::menu;
	theBtnType = btnTypes::exit;

	// Create textures for Game Dialogue (text)
	fontList = { "Main", "Secondary" };
	fontsToUse = { "Fonts/Freshman.ttf", "Fonts/Berlin_Sans.ttf" };
	for (int fonts = 0; fonts < (int)fontList.size(); fonts++)
	{
		theFontMgr->addFont(fontList[fonts], fontsToUse[fonts], 36);
	}
	gameTextList = {"Time: ", "Score: ", "Brick Breaker", "Destroy all the bricks before you ran out of time!", "Use arrow keys, A / D or Left Analog (Xbox) to move the paddle.", "Leaderboard", "Press 'SPACE' to shoot out the ball.", "Game Over", "Highscore: "};
	strScore = gameTextList[1];
	stringTime = gameTextList[0];
	strScore += to_string(theScore).c_str();

	theTextureMgr->addTexture("Time", theFontMgr->getFont("Main")->createTextTexture(theRenderer, stringTime.c_str(), textType::solid, { 255, 255, 255, 255 }, { 0, 0, 0, 0 }));
	theTextureMgr->addTexture("theScore", theFontMgr->getFont("Main")->createTextTexture(theRenderer, strScore.c_str(), textType::solid, { 255, 255, 255, 255 }, { 0, 0, 0, 0 }));
	theTextureMgr->addTexture("Title", theFontMgr->getFont("Main")->createTextTexture(theRenderer, gameTextList[2], textType::solid, { 255, 255, 255, 255 }, { 0, 0, 0, 0 }));
	theTextureMgr->addTexture("Objective", theFontMgr->getFont("Secondary")->createTextTexture(theRenderer, gameTextList[3], textType::solid, { 255, 255, 255, 255 }, { 0, 0, 0, 0 }));
	theTextureMgr->addTexture("Instructions", theFontMgr->getFont("Secondary")->createTextTexture(theRenderer, gameTextList[4], textType::solid, { 255, 255, 255, 255 }, { 0, 0, 0, 0 }));
	theTextureMgr->addTexture("Leaderboard", theFontMgr->getFont("Main")->createTextTexture(theRenderer, gameTextList[5], textType::solid, { 255, 255, 255, 255 }, { 0, 0, 0, 0 }));
	theTextureMgr->addTexture("PressSpace", theFontMgr->getFont("Secondary")->createTextTexture(theRenderer, gameTextList[6], textType::solid, { 255, 255, 255, 255 }, { 0, 0, 0, 0 }));
	theTextureMgr->addTexture("GameOver", theFontMgr->getFont("Secondary")->createTextTexture(theRenderer, gameTextList[7], textType::solid, { 255, 255, 255, 255 }, { 0, 0, 0, 0 }));
	theTextureMgr->addTexture("HighScore", theFontMgr->getFont("Secondary")->createTextTexture(theRenderer, gameTextList[8], textType::solid, { 255, 255, 255, 255 }, { 0, 0, 0, 0 }));

	// Load game sounds
	soundList = { "theme1", "pick_up_appeared", "brick_destroy_sound", "ball_hit", "life_lost", "button_click", "pick_up_sound" };
	soundTypes = { soundType::music, soundType::sfx, soundType::sfx, soundType::sfx, soundType::sfx, soundType::sfx, soundType::sfx };
	soundsToUse = { "Audio/music.wav", "Audio/pick_up_appeared.wav", "Audio/brick_destroy_sound.wav", "Audio/ball_hit.wav", "Audio/life_lost.wav", "Audio/button_click.wav", "Audio/pick_up_sound.wav" };
	for (int sounds = 0; sounds < (int)soundList.size(); sounds++)
	{
		theSoundMgr->add(soundList[sounds], soundsToUse[sounds], soundTypes[sounds]);
	}

	theSoundMgr->getSnd("theme1")->play(1);
	
	// Preparing the background
	spriteBkgd.setSpritePos({ 0, 0 });
	spriteBkgd.setTexture(theTextureMgr->getTexture("background"));
	spriteBkgd.setSpriteDimensions(theTextureMgr->getTexture("background")->getTWidth(), theTextureMgr->getTexture("background")->getTHeight());

	thePaddle.setTexture(theTextureMgr->getTexture("paddle_s"));
	thePaddle.setSpriteDimensions(theTextureMgr->getTexture("paddle_s")->getTWidth(), theTextureMgr->getTexture("paddle_s")->getTHeight());
	// Placing the paddle sprite in the middle bottom of the screen
	thePaddle.setSpritePos({ 500, (int)(1280 - thePaddle.getSpriteDimensions().h * 1.5) });
	thePaddle.setRocketVelocity(200);
	thePaddle.setRocketMaxSpeed(700);
	thePaddle.setSpriteTranslation({ 50,50 });

	currentPaddleSize = "small";

	// adding up and side wall textures
	wallTextureLeft.setTexture(theTextureMgr->getTexture("wall_left"));
	wallTextureLeft.setSpriteDimensions(theTextureMgr->getTexture("wall_left")->getTWidth(), theTextureMgr->getTexture("wall_left")->getTHeight());
	wallTextureRight.setTexture(theTextureMgr->getTexture("wall_right"));
	wallTextureRight.setSpriteDimensions(theTextureMgr->getTexture("wall_right")->getTWidth(), theTextureMgr->getTexture("wall_right")->getTHeight());
	wallTextureTop.setTexture(theTextureMgr->getTexture("wall_up"));
	wallTextureTop.setSpriteDimensions(theTextureMgr->getTexture("wall_up")->getTWidth(), theTextureMgr->getTexture("wall_up")->getTHeight());
	
	lifeTexture.setTexture(theTextureMgr->getTexture("life"));
	lifeTexture.setSpriteDimensions(theTextureMgr->getTexture("life")->getTWidth(), theTextureMgr->getTexture("life")->getTHeight());

	createLevel1();

	// calculating how many wall textures fit in screen
	wallNumberTop = (int)(1280 / wallTextureTop.getSpriteDimensions().w) + 1;
	wallNumberSide = (int)(1280 / wallTextureLeft.getSpriteDimensions().h);

	// passing wall's width to cRocket
	thePaddle.setMovingWidth(wallTextureLeft.getSpriteDimensions().w);
}

void cGame::run(SDL_Window* theSDLWND, SDL_Renderer* theRenderer)
{
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
		spriteBkgd.setSpritePos({ 0, 0 });
		spriteBkgd.setTexture(theTextureMgr->getTexture("menu_background"));
		spriteBkgd.setSpriteDimensions(theTextureMgr->getTexture("menu_background")->getTWidth(), theTextureMgr->getTexture("menu_background")->getTHeight());
		
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

		// Rendered buttons
		theButtonMgr->getBtn("play_btn")->setSpritePos({ 460, 480 });
		theButtonMgr->getBtn("play_btn")->render(theRenderer, &theButtonMgr->getBtn("play_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("play_btn")->getSpritePos(), theButtonMgr->getBtn("play_btn")->getSpriteScale());
		theButtonMgr->getBtn("exit_btn")->setSpritePos({ 640, 480 });
		theButtonMgr->getBtn("exit_btn")->render(theRenderer, &theButtonMgr->getBtn("exit_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("exit_btn")->getSpritePos(), theButtonMgr->getBtn("exit_btn")->getSpriteScale());
		
	}break;
	case gameState::playing:
	{
		// Ingame background
		spriteBkgd.setSpritePos({ 0, 0 });
		spriteBkgd.setTexture(theTextureMgr->getTexture("background"));
		spriteBkgd.setSpriteDimensions(theTextureMgr->getTexture("background")->getTWidth(), theTextureMgr->getTexture("background")->getTHeight());
		
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

		// Create top wall
		for (int i = 0; i < wallNumberTop; i++)
		{
			wallTextureTop.setSpritePos({ i*wallTextureTop.getSpriteDimensions().w, 0 });
			wallTextureTop.render(theRenderer, &wallTextureTop.getSpriteDimensions(), &wallTextureTop.getSpritePos(), wallTextureTop.getSpriteRotAngle(), &wallTextureTop.getSpriteCentre(), wallTextureTop.getSpriteScale());
		}

		// Rendering the hearts that represent player's lifes
		for (int i = 0; i < lifesLeft; i++)
		{
			lifeTexture.setSpritePos({ i * lifeTexture.getSpriteDimensions().w, 0 });
			lifeTexture.render(theRenderer, &lifeTexture.getSpriteDimensions(), &lifeTexture.getSpritePos(), lifeTexture.getSpriteRotAngle(), &lifeTexture.getSpriteCentre(), lifeTexture.getSpriteScale());
		}

		// Render each brick in the vector array
		for (int draw = 0; draw < (int)theAsteroids.size(); draw++)
		{
			theAsteroids[draw]->render(theRenderer, &theAsteroids[draw]->getSpriteDimensions(), &theAsteroids[draw]->getSpritePos(), theAsteroids[draw]->getSpriteRotAngle(), &theAsteroids[draw]->getSpriteCentre(), theAsteroids[draw]->getSpriteScale());
		}

		// Rendering pick ups
		for (int draw = 0; draw < (int)thePickUps.size(); draw++)
		{
			thePickUps[draw]->render(theRenderer, &thePickUps[draw]->getSpriteDimensions(), &thePickUps[draw]->getSpritePos(), thePickUps[draw]->getSpriteRotAngle(), &thePickUps[draw]->getSpriteCentre(), thePickUps[draw]->getSpriteScale());
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

		if (enabledSpace == true)
		{
			cTexture* tempTextTexture4 = theTextureMgr->getTexture("PressSpace");
			SDL_Rect pos4 = { (1280 / 2) - (tempTextTexture4->getTextureRect().w / 2), (1280 / 2), tempTextTexture4->getTextureRect().w, tempTextTexture4->getTextureRect().h };
			FPoint scale4 = { 1, 1 };
			tempTextTexture4->renderTexture(theRenderer, tempTextTexture4->getTexture(), &tempTextTexture4->getTextureRect(), &pos4, scale4);
			
			// Showing to press 'A' instead of spacebar if controller is connected 
			if (theController.checkConnection() == true)
			{
				string pressButton = "Press 'A' to shoot out the ball!";
				theTextureMgr->deleteTexture("PressSpace");
				theTextureMgr->addTexture("PressSpace", theFontMgr->getFont("Secondary")->createTextTexture(theRenderer, pressButton.c_str(), textType::solid, { 255, 255, 255, 255 }, { 0, 0, 0, 0 }));
			}
		}
		// Render the paddle
		thePaddle.render(theRenderer, &thePaddle.getSpriteDimensions(), &thePaddle.getSpritePos(), thePaddle.getSpriteRotAngle(), &thePaddle.getSpriteCentre(), thePaddle.getSpriteScale());
		SDL_RenderPresent(theRenderer);
		break;
	}
	case gameState::end:
	{
		// End menu background
		spriteBkgd.setSpritePos({ 0, 0 });
		spriteBkgd.setTexture(theTextureMgr->getTexture("leaderboard_background"));
		spriteBkgd.setSpriteDimensions(theTextureMgr->getTexture("leaderboard_background")->getTWidth(), theTextureMgr->getTexture("leaderboard_background")->getTHeight());

		spriteBkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteScale());
		
		// Title
		cTexture* tempTextTexture = theTextureMgr->getTexture("Title");
		SDL_Rect pos = { (1280 / 2) - (tempTextTexture->getTextureRect().w / 2), 4, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		FPoint scale = { 1, 1 };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);

		// Objective
		tempTextTexture = theTextureMgr->getTexture("Leaderboard");
		pos = { (1280 / 2) - (tempTextTexture->getTextureRect().w / 2), 60, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);

		// 'Game over' text
		tempTextTexture = theTextureMgr->getTexture("GameOver");
		pos = { (1280 / 2) - (tempTextTexture->getTextureRect().w), 600, tempTextTexture->getTextureRect().w *2, tempTextTexture->getTextureRect().h *2 };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		
		// Highscore - setting values and deleting the texture
		tempTextTexture = theTextureMgr->getTexture("HighScore");
		pos = { (1280 / 2) - (tempTextTexture->getTextureRect().w / 2), 150, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		theTextureMgr->deleteTexture("HighScore");

		// displaying the highscore
		string highScoreText = "Highscore: " + to_string(theHSTable.loadFromFile("Data\\HighestScore.dat"));
		theTextureMgr->addTexture("HighScore", theFontMgr->getFont("Secondary")->createTextTexture(theRenderer, highScoreText.c_str(), textType::solid, { 255, 255, 255, 255 }, { 0, 0, 0, 0 }));

		// Rendered buttons
		theButtonMgr->getBtn("play_btn")->setSpritePos({ 500, 700 });
		theButtonMgr->getBtn("play_btn")->render(theRenderer, &theButtonMgr->getBtn("play_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("play_btn")->getSpritePos(), theButtonMgr->getBtn("play_btn")->getSpriteScale());
		theButtonMgr->getBtn("menu_btn")->setSpritePos({ 700, 700 });
		theButtonMgr->getBtn("menu_btn")->render(theRenderer, &theButtonMgr->getBtn("menu_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("menu_btn")->getSpritePos(), theButtonMgr->getBtn("menu_btn")->getSpriteScale());
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
	// Checking button in order to know what action to take
	if (theGameState == gameState::menu || theGameState == gameState::end)
	{
		theGameState = theButtonMgr->getBtn("exit_btn")->update(theGameState, gameState::quit, theAreaClicked);
	}
	theGameState = theButtonMgr->getBtn("play_btn")->update(theGameState, gameState::playing, theAreaClicked);
	theGameState = theButtonMgr->getBtn("menu_btn")->update(theGameState, gameState::menu, theAreaClicked);
	
	if (theGameState == gameState::playing)
	{
		// restores default values if player will play again 
		if (gameEnded == true)
		{
			theScore = 0;
			timePassed = 0;
			lifesLeft = 3;
			brickCount = 0;
			theScore = 0;

			theTextureMgr->deleteTexture("theScore");
			strScore = "Score: 0";
			theTextureMgr->addTexture("theScore", theFontMgr->getFont("Main")->createTextTexture(theRend, strScore.c_str(), textType::solid, { 255, 255, 255, 255 }, { 0, 0, 0, 0 }));

			enabledSpace = true;
			scoreBoost = 0;
			bulletsFallen = 0;
			numberOfPickUps = 0;

			theAsteroids.clear();
			thePickUps.clear();

			createLevel1();
			gameEnded = false;
		}

		// create second level
		if (brickCount == 0 && currentLevel == 1)
		{
			scoreBoost = 0;
			bulletsFallen = 0;
			bulletAmount = 0;
			numberOfPickUps = 0;

			// clearing the vector holding bricks and pickups
			theAsteroids.clear();
			thePickUps.clear();

			// destroying all the balls
			for (vector<cBullet*>::iterator bulletIterartor = theBullets.begin(); bulletIterartor != theBullets.end(); ++bulletIterartor)
			{
				(*bulletIterartor)->setActive(false);
			}

			// desigining second level
			createLevel2();

			// letting the player shoot the ball out again
			enabledSpace = true;
		}

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

		if (enabledSpace == false)
			timeToPickUp += deltaTime;
		else
			timeToPickUp = 0;

		// spawn a pick up
		if (timeToPickUp > 20)
		{
			numberOfPickUps++;

			thePickUps.push_back(new cPickUp);

			random_device rd; // obtain a random number from hardware
			mt19937 eng(rd()); // seed the generator
			uniform_int_distribution<> distr(0, 5); // define the range

			currentPickUp = distr(eng);

			theSoundMgr->getSnd("pick_up_appeared")->play(0);

			// ball
			if (currentPickUp == 0)
			{
				thePickUps[numberOfPickUps - 1]->setTexture(theTextureMgr->getTexture("bonus_balls"));
				thePickUps[numberOfPickUps - 1]->effect = "bonus_balls";
			}
			// bigger
			if (currentPickUp == 1)
			{
				thePickUps[numberOfPickUps - 1]->setTexture(theTextureMgr->getTexture("bonus_bigger"));
				thePickUps[numberOfPickUps - 1]->effect = "bonus_bigger";
			}
			// score
			if (currentPickUp == 2)
			{
				thePickUps[numberOfPickUps - 1]->setTexture(theTextureMgr->getTexture("bonus_score"));
				thePickUps[numberOfPickUps - 1]->effect = "bonus_score";
			}
			// smaller
			if (currentPickUp == 3)
			{
				thePickUps[numberOfPickUps - 1]->setTexture(theTextureMgr->getTexture("bonus_smaller"));
				thePickUps[numberOfPickUps - 1]->effect = "bonus_smaller";
			}
			// speed
			if (currentPickUp == 4)
			{
				thePickUps[numberOfPickUps - 1]->setTexture(theTextureMgr->getTexture("bonus_speed"));
				thePickUps[numberOfPickUps - 1]->effect = "bonus_speed";
			}
			// time
			if (currentPickUp == 5)
			{
				thePickUps[numberOfPickUps - 1]->setTexture(theTextureMgr->getTexture("bonus_time"));
				thePickUps[numberOfPickUps - 1]->effect = "bonus_time";
			}

			random_device rd2; // obtain a random number from hardware
			mt19937 eng2(rd2()); // seed the generator
			uniform_int_distribution<> distr2(wallTextureLeft.getSpriteDimensions().w, 1280 - wallTextureLeft.getSpriteDimensions().w - theTextureMgr->getTexture("bonus_balls")->getTWidth()); // define the range

			int xPos = distr2(eng2);

			// all pick ups have same dimensions
			thePickUps[numberOfPickUps - 1]->setSpriteDimensions(theTextureMgr->getTexture("bonus_balls")->getTWidth(), theTextureMgr->getTexture("bonus_balls")->getTHeight());
			thePickUps[numberOfPickUps - 1]->setSpritePos({ xPos, 550 });

			// resetting the clock
			timeToPickUp = 0;
		}

		/*
		==============================================================
		| Check for collisions
		==============================================================
		*/
		for (vector<cBullet*>::iterator bulletIterartor = theBullets.begin(); bulletIterartor != theBullets.end(); ++bulletIterartor)
		{
			(*bulletIterartor)->update(deltaTime);

			// Playing sound when hitting the top wall
			if ((*bulletIterartor)->getSpritePos().y < 0 + wallTextureTop.getSpriteDimensions().h + 1)
			{
				theSoundMgr->getSnd("ball_hit")->play(0);
			}

			// Playing sound when hitting the left wall
			if ((*bulletIterartor)->getSpritePos().x < 0 + wallTextureRight.getSpriteDimensions().w + 1)
			{
				theSoundMgr->getSnd("ball_hit")->play(0);
			}

			// Playing sound when hitting the right wall
			if ((*bulletIterartor)->getSpritePos().x > 1280 - wallTextureRight.getSpriteDimensions().w - ((*bulletIterartor)->getSpriteDimensions().w))
			{
				theSoundMgr->getSnd("ball_hit")->play(0);
			}

			// checking if hit a pick up
			for (vector<cPickUp*>::iterator pickUpIterator = thePickUps.begin(); pickUpIterator != thePickUps.end();)
			{
				(*pickUpIterator)->update(deltaTime);
				pickUpHit = false;

  				if ((*pickUpIterator)->collidedWith(&(*pickUpIterator)->getBoundingRect(), &(*bulletIterartor)->getBoundingRect()))
				{
					theSoundMgr->getSnd("pick_up_sound")->play(0);
				
					checkWhichToDestroy++;
					applyPickUpEffect((*pickUpIterator)->getEffect());
					theExplosions.push_back(new cSprite);
					int index = theExplosions.size() - 1;
					theExplosions[index]->setSpriteTranslation({ 0, 0 });
					theExplosions[index]->setActive(true);
					theExplosions[index]->setNoFrames(6);
					theExplosions[index]->setTexture(theTextureMgr->getTexture("pick_up_effect"));
					theExplosions[index]->setSpriteDimensions(theTextureMgr->getTexture("pick_up_effect")->getTWidth() / theExplosions[index]->getNoFrames(), theTextureMgr->getTexture("pick_up_effect")->getTHeight());
					theExplosions[index]->setSpritePos({ (*pickUpIterator)->getSpritePos().x + (int)((*pickUpIterator)->getSpritePos().w / 2), (*pickUpIterator)->getSpritePos().y + (int)((*pickUpIterator)->getSpritePos().h / 2) });
					numberOfPickUps--;
					pickUpHit = true;
					thePickUps.erase(pickUpIterator);
					break;
				}else

				if(pickUpHit == false)
					++pickUpIterator;
			}

			if (pickUpHit)
				pickUpHit = false;

			for (vector<cAsteroid*>::iterator asteroidIterator = theAsteroids.begin(); asteroidIterator != theAsteroids.end(); ++asteroidIterator)
			{
				if ((*asteroidIterator)->collidedWith(&(*asteroidIterator)->getBoundingRect(), &(*bulletIterartor)->getBoundingRect()))
				{
					(*asteroidIterator)->health--;

					cout << "BALLS X POS " << (*bulletIterartor)->getSpriteCentre().x + (*bulletIterartor)->getSpritePos().x << endl;
					cout << "BRICK X POS " << (*asteroidIterator)->getSpritePos().x << endl;
					cout << "BRICK X + WIDTH " << (*asteroidIterator)->getSpritePos().x + (*asteroidIterator)->getSpriteDimensions().w <<endl;

					// ball's centre Y pos is higher than brick's Y pos + height - hit the bottom of the brick
					if ((*bulletIterartor)->getSpriteCentre().y + (*bulletIterartor)->getSpritePos().y > (*asteroidIterator)->getSpritePos().y + (*asteroidIterator)->getSpriteDimensions().h)
					{
						cout << "HIT BOTTOM !!!" << endl;
						(*bulletIterartor)->bulletDirectionY = -1;
					}
					// ball's Y pos + texture height is lower than brick's Y pos - hit the top of the brick
					else if ((*bulletIterartor)->getSpritePos().y + (*bulletIterartor)->getSpriteDimensions().h < (*asteroidIterator)->getSpritePos().y)
					{
						cout << "HIT TOP !!!" << endl;
						(*bulletIterartor)->bulletDirectionY = 1;
					}
					// ball's X pos + texture width is lower than brick's X pos - hit the left side of the brick
					else if ((*bulletIterartor)->getSpritePos().x + (*bulletIterartor)->getSpriteDimensions().w  < (*asteroidIterator)->getSpritePos().x)
					{	
						cout << " HIT LEFT !!!" << endl;
						(*bulletIterartor)->bulletDirectionX *= -1;
					}
					// ball's centre X pos is higher than brick's X pos - hit the right side of the brick
					else if ((*bulletIterartor)->getSpriteCentre().x + (*bulletIterartor)->getSpritePos().x > (*asteroidIterator)->getSpritePos().x + (*asteroidIterator)->getSpriteDimensions().w)
					{
						cout << " HIT RIGHT !!!" << endl;
						(*bulletIterartor)->bulletDirectionX *= -1;
					}
					// if none of the above was true it means that the ball hit the corner of the brick
					else
					{
						cout << " HIT THE CORNER !!!" << endl;
						(*bulletIterartor)->bulletDirectionX *= -1;
					}

					// plays sound when ball hits a brick
					theSoundMgr->getSnd("ball_hit")->play(0);

					// if brick is red
					if ((*asteroidIterator)->colour == "red")
					{
						if ((*asteroidIterator)->health == 3)
						{
							(*asteroidIterator)->setTexture(theTextureMgr->getTexture("brick_red_dmg1"));
						}
						else if ((*asteroidIterator)->health == 2)
						{
							(*asteroidIterator)->setTexture(theTextureMgr->getTexture("brick_red_dmg2"));
						}
						else if ((*asteroidIterator)->health == 1)
						{
							(*asteroidIterator)->setTexture(theTextureMgr->getTexture("brick_red_dmg3"));
						}
						else if ((*asteroidIterator)->health == 0)
							theScore += 100 + (100 * scoreBoost);
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
						}
						else if ((*asteroidIterator)->health == 0)
							theScore += 50 + (50 * scoreBoost);
					}

					// if brick is green
					if ((*asteroidIterator)->colour == "green")
					{
						if ((*asteroidIterator)->health == 1)
						{
							(*asteroidIterator)->setTexture(theTextureMgr->getTexture("brick_green_dmg1"));
						}
						else if ((*asteroidIterator)->health == 0)
							theScore += 25 + (25 * scoreBoost);
					}

					// if brick is white
					if ((*asteroidIterator)->colour == "white")
					{
						if ((*asteroidIterator)->health == 0)
							theScore += 10 + (10 * scoreBoost);
					}

					if ((*asteroidIterator)->health == 0)
					{
						(*asteroidIterator)->setActive(false);
						theTextureMgr->deleteTexture("theScore");
						strScore = "Score: ";
						strScore += to_string(theScore).c_str();
						theTextureMgr->addTexture("theScore", theFontMgr->getFont("Main")->createTextTexture(theRend, strScore.c_str(), textType::solid, { 255, 255, 255, 255 }, { 0, 0, 0, 0 }));
						theSoundMgr->getSnd("brick_destroy_sound")->play(0); 
						brickCount--;
					}

					theExplosions.push_back(new cSprite);
					int index = theExplosions.size() - 1;
					theExplosions[index]->setSpriteTranslation({ 0, 0 });
					theExplosions[index]->setActive(true);
					theExplosions[index]->setNoFrames(6);
					theExplosions[index]->setTexture(theTextureMgr->getTexture("hit_effect"));
					theExplosions[index]->setSpriteDimensions(theTextureMgr->getTexture("hit_effect")->getTWidth() / theExplosions[index]->getNoFrames(), theTextureMgr->getTexture("hit_effect")->getTHeight());
					theExplosions[index]->setSpritePos({ (*asteroidIterator)->getSpritePos().x + (int)((*asteroidIterator)->getSpritePos().w / 2), (*asteroidIterator)->getSpritePos().y + (int)((*asteroidIterator)->getSpritePos().h / 2) });
				}
			}

			// when bullet collides with the platform
			if ((*bulletIterartor)->collidedWith(&(thePaddle).getBoundingRect(), &(*bulletIterartor)->getBoundingRect()))
			{
				(*bulletIterartor)->bulletDirectionY = 1;
				theSoundMgr->getSnd("ball_hit")->play(0);

				// adding a different angle to the ball depending of how the far from the centre of the paddle the ball hit
				int bulletCentreX = (*bulletIterartor)->getSpriteCentre().x + (*bulletIterartor)->getSpritePos().x;
				int paddleCentreX = thePaddle.getSpriteCentre().x + thePaddle.getSpritePos().x;

				int difference = bulletCentreX - paddleCentreX;

				/*cout << "BULLET X " << bulletCentreX << endl;
				cout << "PADDLE X " << paddleCentreX << endl;
				cout << "DIFFERENCE " << difference << endl;*/

				// ball hit paddle's left side from centre
				if (difference < 0)
					difference *= -1;

				// maximum angle is 80, depending on the paddle size, each pixel will mean a different angle
				double anglePerPixel = (thePaddle.getSpriteDimensions().w / 2) / 70;

				int ballAngle = int (difference / anglePerPixel);

				//cout << "ANGLE " << ballAngle << endl;

				// shouldn't exeed 70 because the ball would travel nearly in a straight, horizontal line
				if (ballAngle > 70)
					ballAngle = 70;

				// shouldn't be lower than 15 because the ball would travel nearly in a straight, vertical line
				if (ballAngle < 15)
					ballAngle = 15;

				// the first bounce with current angle 0, checking if it hit the left side of the paddle to bounce it from the paddle to the left side
				if ((*bulletIterartor)->getSpriteRotAngle() == 0)
				{
					if (bulletCentreX - paddleCentreX < 0)
					{
						(*bulletIterartor)->setSpriteRotAngle(-ballAngle);
					}
					else
						(*bulletIterartor)->setSpriteRotAngle(ballAngle);
				}
				else
					(*bulletIterartor)->setSpriteRotAngle(ballAngle);
			}

			// when bullet falls under the platform
			if ((*bulletIterartor)->getSpritePos().y > thePaddle.getSpritePos().y + 30)
			{
				// deleting the bullet
				(*bulletIterartor)->setActive(false);
				bulletsFallen++;

				// only if all the bullets are under the platform
				if (bulletsFallen == bulletAmount)
				{
					enabledSpace = true;
					bulletsFallen = 0;
					bulletAmount = 0;
					
					// deactivate all bonus effects
					thePaddle.setTexture(theTextureMgr->getTexture("paddle_s"));
					thePaddle.setSpriteDimensions(theTextureMgr->getTexture("paddle_s")->getTWidth(), theTextureMgr->getTexture("paddle_s")->getTHeight());
					currentPaddleSize = "small";
					scoreBoost = 0;
					theBullets[0]->setBulletVelocity(5);

					// delete all the pick ups
					numberOfPickUps = 0;
					for (int i = 0; i < thePickUps.size(); i++)
						thePickUps[i]->setActive(false);
					thePickUps.clear();

					// remove one life (removes the heart texture as well because they are rendered every frame)
					lifesLeft--;
					theSoundMgr->getSnd("life_lost")->play(0);
				}
			}
		}

		timePassed += deltaTime;

		// Update timer
		theTextureMgr->deleteTexture("Time");
		stringTime = "Time: ";
		stringTime += to_string((int)timePassed).c_str();
		theTextureMgr->addTexture("Time", theFontMgr->getFont("Main")->createTextTexture(theRend, stringTime.c_str(), textType::solid, { 255, 255, 255, 255 }, { 0, 0, 0, 0 }));

		// Update the paddle's position
		thePaddle.update(deltaTime);

		// if all lifes are lost the player loses the game or if the player completed the second level he goes to the end screen
		if (lifesLeft == 0 || (currentLevel == 2 && brickCount == 0))
		{
			// destroying all balls
			for (vector<cBullet*>::iterator bulletIterartor = theBullets.begin(); bulletIterartor != theBullets.end(); ++bulletIterartor)
			{
				(*bulletIterartor)->setActive(false);
			}

			// additional score for the left time
			int timeBonusScore = 0;

			// 10 points for each second but no points taken if exeeded time limit
			if ((60 - timePassed) < 0)
				timeBonusScore = 0;
			else timeBonusScore = (360 - int(timePassed)) * 10;
			
			theScore += lifesLeft * 150 + timeBonusScore;

			theHSTable.compareScoreAndSave(to_string(theScore), "Data\\HighestScore.dat");

			gameEnded = true;
			theGameState = gameState::end;
		}

		// simulating spacebar press when using a controller
		if (theController.checkConnection() == true && theController.checkIfShooting() == true)
		{
			SDL_Event event;

			SDL_PollEvent(&event);
			event.type = SDL_KEYDOWN;
			event.key.keysym.sym = SDLK_SPACE;
			SDL_PushEvent(&event);

			//cout << "Simulated spacebar press" << endl;
		}
	}
}

bool cGame::getInput(bool theLoop)
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		theAreaClicked = { 0,0 };
		if (event.type == SDL_QUIT)
		{
			theLoop = false;
		}

		switch (event.type)
		{
			case SDL_MOUSEBUTTONDOWN:
				switch (event.button.button)
				{
				case SDL_BUTTON_LEFT:
				{
					theSoundMgr->getSnd("button_click")->play(0);
					theAreaClicked = { event.motion.x, event.motion.y };
				}break;
				case SDL_BUTTON_RIGHT:
					break;
				default:
					break;
				}

			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					theLoop = false;
					break;
				case SDLK_SPACE:
				{
					// the player can shoot a ball from platform when he loses his current one
					if (enabledSpace == true && theGameState == gameState::playing)
					{
						enabledSpace = false;
						theBullets.push_back(new cBullet);
						int numBullets = theBullets.size() - 1;
						theBullets[numBullets]->setSpritePos({ thePaddle.getBoundingRect().x + thePaddle.getSpriteCentre().x, thePaddle.getBoundingRect().y - thePaddle.getSpriteCentre().y});
						theBullets[numBullets]->setSpriteTranslation({ 50, 50 });
						theBullets[numBullets]->setTexture(theTextureMgr->getTexture("ball"));
						theBullets[numBullets]->setSpriteDimensions(theTextureMgr->getTexture("ball")->getTWidth(), theTextureMgr->getTexture("ball")->getTHeight());
						theBullets[numBullets]->setBulletVelocity(5);
						// passing walls' dimensions to cBulletStuVer
						theBullets[numBullets]->setMovingPoints(wallTextureLeft.getSpriteDimensions().w, wallTextureTop.getSpriteDimensions().h);
						theBullets[numBullets]->setSpriteRotAngle(0);
						theBullets[numBullets]->setActive(true);
						cout << "Bullet added to Vector at position - x: " << thePaddle.getBoundingRect().x << " y: " << thePaddle.getBoundingRect().y << endl;
						theSoundMgr->getSnd("ball_hit")->play(0);
						bulletAmount++;
					}
				}
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

