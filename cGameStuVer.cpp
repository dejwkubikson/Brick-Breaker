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
	uniform_int_distribution<> AsteroidTextDis{ 4, 15 };

	theTextureMgr->setRenderer(theRenderer);
	theFontMgr->initFontLib();
	theSoundMgr->initMixer();
	theScore = 0;

	// Store the textures
	
	textureName = { "background", "paddle_s", "paddle_m", "paddle_l", // background and paddles
					"brick_black", "brick_white", "brick_green", "brick_green_dmg1", "brick_orange", "brick_orange_dmg1", "brick_green_dmg2", "brick_red", "brick_red_dmg1", "brick_red_dmg2", "brick_red_dmg3", // bricks
					"pick_up_effect", "ball", "bonus_balls", "bonus_bigger", "bonus_score", "bonus_smaller", "bonus_speed", "bonus_time" // pick ups
					"life", "score", "time", "wall_left", "wall_right", "wall_up" }; // additional
	texturesToUse = { "Images\\Bkg\\Background1280x1280.jpg", "Images\\Sprites\\paddle_s.png", "Images\\Sprites\\paddle_m.png", "Images\\Sprites\\paddle_l.png", // background and paddles
					  "Images\\Sprites\\brick_black.png", "Images\\Sprites\\brick_white.png", "Images\\Sprites\\brick_green.png", "Images\\Sprites\\brick_green_dmg1.png", "Images\\Sprites\\brick_orange.png", "Images\\Sprites\\brick_orange_dmg1.png", "Images\\Sprites\\brick_orange_dmg2.png", "Images\\Sprites\\brick_red.png", "Images\\Sprites\\brick_red_dmg1.png", "Images\\Sprites\\brick_red_dmg2.png", "Images\\Sprites\\brick_red_dmg3.png", // bricks
					  "Images\\Sprites\\pick_up_effect.png", "Images\\Sprites\\ball.png", "Images\\Sprites\\bonus_balls.png", "Images\\Sprites\\bonus_bigger.png", "Images\\Sprites\\bonus_score.png", "Images\\Sprites\\bonus_smaller.png", "Images\\Sprites\\bonus_speed.png", "Images\\Sprites\\bonus_time.png", // pick ups
					  "Images\\Sprites\\life.png", "Images\\Sprites\\score.png", "Images\\Sprites\\time.png", "Images\\Sprites\\wall_left.png", "Images\\Sprites\\wall_right.png", "Images\\Sprites\\wall_up.png" }; // additional

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
	gameTextList = { "Lifes: ", "Time: 10", "Score: "};
	strScore = gameTextList[2];
	strScore += to_string(theScore).c_str();

	theTextureMgr->addTexture("Lifes", theFontMgr->getFont("Main")->createTextTexture(theRenderer, gameTextList[0], textType::solid, { 255, 255, 255, 255 }, { 0, 0, 0, 0 }));
	theTextureMgr->addTexture("Time", theFontMgr->getFont("Main")->createTextTexture(theRenderer, gameTextList[1], textType::solid, { 255, 255, 255, 255 }, { 0, 0, 0, 0 }));
	theTextureMgr->addTexture("theScore", theFontMgr->getFont("Main")->createTextTexture(theRenderer, strScore.c_str(), textType::solid, { 255, 255, 255, 255 }, { 0, 0, 0, 0 }));
	
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
	thePaddle.setSpritePos({ 640, (int)(1280 - thePaddle.getSpriteDimensions().h * 1.5) });
	thePaddle.setRocketVelocity(200);
	thePaddle.setRocketMaxSpeed(700);
	thePaddle.setSpriteTranslation({ 50,50 });

	// Create vector array of textures

	for (int astro = 0; astro < 10; astro++)
	{
		theAsteroids.push_back(new cAsteroid);
		theAsteroids[astro]->setSpritePos({ 150 * AsteroidDis(gen), 50 * AsteroidDis(gen) });
		theAsteroids[astro]->setSpriteTranslation({ 100, -50 });
		int randAsteroid = AsteroidTextDis(gen);
		theAsteroids[astro]->setTexture(theTextureMgr->getTexture(textureName[randAsteroid]));
		theAsteroids[astro]->setSpriteDimensions(theTextureMgr->getTexture(textureName[randAsteroid])->getTWidth(), theTextureMgr->getTexture(textureName[randAsteroid])->getTHeight());
		theAsteroids[astro]->setAsteroidVelocity(200);
		theAsteroids[astro]->setActive(true);
	}

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
	spriteBkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteScale());
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
	// Render the Title
	cTexture* tempTextTexture = theTextureMgr->getTexture("Lifes");
	SDL_Rect pos = { 0, 10, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
	FPoint scale = { 1, 1 };
	tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
	// Render updated score value
	
	// Lab 7 code goes here
	cTexture* tempTextTexture2 = theTextureMgr->getTexture("Time");
	SDL_Rect pos3 = { (1280 / 2) - (tempTextTexture2->getTextureRect().w / 2), 10, tempTextTexture2->getTextureRect().w, tempTextTexture2->getTextureRect().h };
	FPoint scale3 = { 1, 1 };
	tempTextTexture2->renderTexture(theRenderer, tempTextTexture2->getTexture(), &tempTextTexture2->getTextureRect(), &pos3, scale3);
	theTextureMgr->addTexture("Time", theFontMgr->getFont("Main")->createTextTexture(theRenderer, gameTextList[2], textType::solid, { 255, 255, 255, 255 }, { 0, 0, 0, 0 }));

	cTexture* tempTextTexture3 = theTextureMgr->getTexture("theScore");
	SDL_Rect pos2 = { 1280 - tempTextTexture3->getTextureRect().w, 10, tempTextTexture3->getTextureRect().w, tempTextTexture3->getTextureRect().h };
	FPoint scale2 = { 1, 1 };
	tempTextTexture3->renderTexture(theRenderer, tempTextTexture3->getTexture(), &tempTextTexture3->getTextureRect(), &pos2, scale2);
	theTextureMgr->addTexture("theScore", theFontMgr->getFont("Main")->createTextTexture(theRenderer, strScore.c_str(), textType::solid, { 255, 255, 255, 255 }, { 0, 0, 0, 0 }));

	// render the rocket
	thePaddle.render(theRenderer, &thePaddle.getSpriteDimensions(), &thePaddle.getSpritePos(), thePaddle.getSpriteRotAngle(), &thePaddle.getSpriteCentre(), thePaddle.getSpriteScale());
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
		//(*bulletIterartor)->update(deltaTime);
		for (vector<cAsteroid*>::iterator asteroidIterator = theAsteroids.begin(); asteroidIterator != theAsteroids.end(); ++asteroidIterator)
		{
			if ((*asteroidIterator)->collidedWith(&(*asteroidIterator)->getBoundingRect(), &(*bulletIterartor)->getBoundingRect()))
			{
				// if a collision set the bullet and asteroid to false
				(*asteroidIterator)->setActive(false);
				(*bulletIterartor)->setActive(false);
				theExplosions.push_back(new cSprite);
				int index = theExplosions.size()-1;
				theExplosions[index]->setSpriteTranslation({ 0, 0 });
				theExplosions[index]->setActive(true);
				theExplosions[index]->setNoFrames(6);
				theExplosions[index]->setTexture(theTextureMgr->getTexture("pick_up_effect"));
				theExplosions[index]->setSpriteDimensions(theTextureMgr->getTexture("pick_up_effect")->getTWidth()/ theExplosions[index]->getNoFrames(), theTextureMgr->getTexture("pick_up_effect")->getTHeight());
				theExplosions[index]->setSpritePos({ (*asteroidIterator)->getSpritePos().x + (int)((*asteroidIterator)->getSpritePos().w/2), (*asteroidIterator)->getSpritePos().y + (int)((*asteroidIterator)->getSpritePos().h / 2) });

				theSoundMgr->getSnd("pick_up_sound")->play(0);
				
				// Lab 7 code goes here
				theTextureMgr->deleteTexture("theScore");
				theScore += 400;
				strScore = "Score: ";
				strScore += to_string(theScore).c_str();
				theTextureMgr->addTexture("theScore", theFontMgr->getFont("Main")->createTextTexture(theRend, strScore.c_str(), textType::solid, { 255, 255, 255, 255 }, { 0, 0, 0, 0 }));		
			}
		}
	}

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
					theBullets[numBullets]->setBulletVelocity(50);
					theBullets[numBullets]->setSpriteRotAngle(thePaddle.getSpriteRotAngle());
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

