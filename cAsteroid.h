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
	int health = 0;		// brick's health points (used to destroy when damaged to 0 health)
	string colour = ""; // brick's colour (used to change texture when damaged)
	int direction = 1;		 // when 1 asteroids go right, when -1 the asteroids go left
};
#endif