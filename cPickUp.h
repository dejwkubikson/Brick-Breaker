/*
=================
cPickUp.h
- Header file for class definition - SPECIFICATION
- Header file for the Rocket class which is a child of cSprite class
=================
*/
#ifndef _CPICKUP_H
#define _CPICKUP_H
#include "cSprite.h"

class cPickUp : public cSprite
{
private:

public:
	cPickUp();
	void update(double deltaTime);		// pickup update method
	string getEffect();
	string effect = "";
};
#endif#pragma once
