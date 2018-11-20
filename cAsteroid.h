/*
=================
cAsteroid.h
- Header file for class definition - SPECIFICATION
- Header file for the Rocket class which is a child of cSprite class
=================
*/
#ifndef _CASTEROID_H
#define _CASTEROID_H
#include "cSprite.h"

class cAsteroid : public cSprite
{
private:
	int asteroidVelocity;

public:
	cAsteroid();
	void update(double deltaTime);		// Rocket update method
	void setAsteroidVelocity(int asteroidVel);   // Sets the velocity for the asteroid
	int getAsteroidVelocity();				 // Gets the asteroid velocity
	int direction = 1;		 // when 1 asteroids go right, when -1 the asteroids go left
};
#endif