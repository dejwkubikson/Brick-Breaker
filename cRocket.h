/*
=================
cRocket.h
- Header file for class definition - SPECIFICATION
- Header file for the Rocket class which is a child of cSprite class
=================
*/
#ifndef _CROCKET_H
#define _CROCKET_H
#include "cSprite.h"
#include "cController.h"

class cRocket : public cSprite
{
private:
	int rocketVelocity;
	int rocketMaxSpeed = 700;
	int rocketAcceleration = 5;
	int rocketDecceleration = 20;
	int wallWidth = 0;
	int move = 0;
	bool stopRocket = false; // true when the movement keys are released - stops the platform

public:
	cRocket();
	void update(double deltaTime); // Rocket update method, added the int in order to prevent the platform moving 'on' the walls on the side
	void setRocketVelocity(int rocketVel);   // Sets the velocity for the rocket
	int getRocketVelocity();				 // Gets the rocket velocity
	void setRocketMove(int rocketMove);   // Sets the rocket move Value
	int getRocketMove();				 // Gets the rocket move value
	void setRocketMaxSpeed(int rocketMax); // Sets the rocket's max speed
	int getRocketMaxSpeed(); // Gets the rocket's max speed
	void setMovingWidth(int wallSizeW); // sets the moving width - platform cannot move onto the walls on sides
	cController theController;
	int paddleSize = 0;
	void setPaddleSize(int toSet);
	int getPaddleSize();
}; 
#endif