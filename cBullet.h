/*
=================
cBullet.h
- Header file for class definition - SPECIFICATION
- Header file for the Bullet class which is a child of cSprite class
=================
*/
#ifndef _CBULLET_H
#define _CBULLET_H
#include "cSprite.h"


class cBullet : public cSprite
{
private:
	int bulletVelocity;
	int wallWidth = 0;
	int wallHeight = 0;

public:
	cBullet();
	void update(double deltaTime);		// Bullet update method
	void setBulletVelocity(int bulletVel);   // Sets the velocity for the Bullet
	int getBulletVelocity();				 // Gets the Bullet velocity
	void setMovingPoints(int wallSizeW, int wallSizeH); // Sets the walls' dimensions to cBullet's variables
	int bulletDirectionX = 1; // used for bounce of the ball
	int bulletDirectionY = 1; // used for bounce of the ball
};
#endif