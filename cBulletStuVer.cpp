/*
=================
cBullet.cpp
- Header file for class definition - IMPLEMENTATION
=================
*/
#include "cBullet.h"
#include "cGame.h"

/*
=================================================================
Defualt Constructor
=================================================================
*/
cBullet::cBullet() : cSprite()
{
	this->bulletVelocity = 0;
}
/*
=================================================================
Update the sprite position
=================================================================
*/

void cBullet::update(double deltaTime)
{
	auto rads = PI / 180.0f * this->getSpriteRotAngle() - 90.0f * PI / 180;

	FPoint direction = { 0.0f, 0.0f };
	direction.X = (float)(cos(rads)) * bulletDirectionX;
	direction.Y = (float)(sin(rads)) * bulletDirectionY;

	SDL_Rect currentSpritePos = this->getSpritePos();
	currentSpritePos.x += (int)(direction.X * this->bulletVelocity * this->getSpriteTranslation().x * deltaTime);
	currentSpritePos.y += (int)(direction.Y * this->bulletVelocity * this->getSpriteTranslation().y * deltaTime);

	//cout << wallHeight << endl;
	//cout << wallWidth << endl;

	//cout << currentSpritePos.y << endl;


	// GOING UP dividing the ball's height gave the best and most accurate bounce effect (it bounced right of the wall when it touched it and not from the centre or any other point)
	if (currentSpritePos.y < 0 + wallHeight + (this->getSpriteDimensions().h / 2))
	{
		bulletDirectionY = -1;
	}

	// GOING LEFT
	if (currentSpritePos.x < 0 + wallWidth + (this->getSpriteDimensions().w / 2))
	{
		bulletDirectionX = 1;
	}

	// GOING RIGHT
	if (currentSpritePos.x > 1280 - wallWidth - (this->getSpriteDimensions().w))
	{
		bulletDirectionX = -1;
	}

	this->setSpritePos({ currentSpritePos.x, currentSpritePos.y });

	this->setBoundingRect(this->getSpritePos());
}
/*
=================================================================
  Sets the velocity for the Bullet
=================================================================
*/
void cBullet::setBulletVelocity(int bulletVel)
{
	this->bulletVelocity = bulletVel;
}
/*
=================================================================
  Gets the Bullet velocity
=================================================================
*/
int cBullet::getBulletVelocity()
{
	return this->bulletVelocity;
}
/*
=================================================================
  Sets the space width in which ball can move in
=================================================================
*/
void cBullet::setMovingPoints(int wallSizeW, int wallSizeH)
{
	wallWidth = wallSizeW;
	wallHeight = wallSizeH;
}
/*
=================================================================
  Gets the space width in which ball can move in
=================================================================
*/