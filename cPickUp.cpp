/*
=================
cAsteroid.cpp
- Header file for class definition - IMPLEMENTATION
=================
*/
#include "cPickUp.h"
#include <windows.h>

/*
=================================================================
Defualt Constructor
=================================================================
*/
cPickUp::cPickUp() : cSprite()
{

}
/*
=================================================================
Update the sprite position
=================================================================
*/

void cPickUp::update(double deltaTime)
{
	SDL_Rect currentSpritePos = this->getSpritePos();

	setBoundingRect(this->getSpritePos());
}

string cPickUp::getEffect()
{
	return this->effect;
}
