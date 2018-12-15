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

public:
	cAsteroid();
	void update(double deltaTime);		// Rocket update method
	int health = 0;		// brick's health points (used to destroy when damaged to 0 health)
	string colour = ""; // brick's colour (used to change texture when damaged)
};
#endif