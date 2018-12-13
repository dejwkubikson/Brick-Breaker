#pragma once
#include <iostream>
#include <SDL.h>
#include <Windows.h>
#include <Xinput.h>
#pragma comment(lib, "XInput.lib")

class cController
{
private:
	DWORD dwXIndex0;
	XINPUT_STATE state;

public:
	cController();
	~cController();

	bool movingRight();
	bool movingLeft();
	bool checkIfShooting();
	bool checkConnection();
};