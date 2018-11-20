/*
=================
cAsteroid.cpp
- Header file for class definition - IMPLEMENTATION
=================
*/
#include "cAsteroid.h"
#include <windows.h>

/*
=================================================================
Defualt Constructor
=================================================================
*/
cAsteroid::cAsteroid() : cSprite()
{
	this->asteroidVelocity = 0;
}
/*
=================================================================
Update the sprite position
=================================================================
*/

void cAsteroid::update(double deltaTime)
{
	this->setSpriteRotAngle((float)(this->getSpriteRotAngle() + (5.0f * deltaTime)));
	if (this->getSpriteRotAngle() > 360)
	{
		this->setSpriteRotAngle(this->getSpriteRotAngle() - 360.0f);
	}

	SDL_Rect currentSpritePos = this->getSpritePos();


	// @@@ ASTEROIDS BOUNCE FROM EACH SIDE
	if (currentSpritePos.x < 0)
	{
		this->direction = 1;
	}

	if (currentSpritePos.x > 1280 - getSpriteDimensions().w)
	{
		this->direction = -1;
	}

	currentSpritePos.x += (int)(this->getSpriteTranslation().x * deltaTime * direction);
	currentSpritePos.y -= (int)(this->getSpriteTranslation().y * deltaTime * direction);

	this->setSpritePos({ currentSpritePos.x, currentSpritePos.y });
	this->setBoundingRect(this->getSpritePos());
}
/*
=================================================================
  Sets the velocity for the Asteroid
=================================================================
*/
void cAsteroid::setAsteroidVelocity(int AsteroidVel)
{
	asteroidVelocity = AsteroidVel;
}
/*
=================================================================
  Gets the Asteroid velocity
=================================================================
*/
int cAsteroid::getAsteroidVelocity()
{
	return asteroidVelocity;
}
