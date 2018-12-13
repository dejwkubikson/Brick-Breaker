#include "cController.h"

cController::cController()
{

}

cController::~cController()
{

}

// Left thumbstick x-axis value. The value is between -32768 and 32767. < 0 - analog moved left / > 0 - analog moved right
bool cController::movingRight()
{
	ZeroMemory(&state, sizeof(XINPUT_STATE));

	dwXIndex0 = XInputGetState(0, &state);

	//cout << state.Gamepad.sThumbLX << endl;

	if (state.Gamepad.sThumbLX > 15000)
		return true;
	else
		return false;

}

// Left thumbstick x-axis value. The value is between -32768 and 32767. < 0 - analog moved left / > 0 - analog moved right
bool cController::movingLeft()
{
	ZeroMemory(&state, sizeof(XINPUT_STATE));

	dwXIndex0 = XInputGetState(0, &state);

	//cout << state.Gamepad.sThumbLX << endl;

	if (state.Gamepad.sThumbLX < -15000)
		return true;
	else
		return false;
}

bool cController::checkIfShooting()
{
	ZeroMemory(&state, sizeof(XINPUT_STATE));

	dwXIndex0 = XInputGetState(0, &state);

	if (state.Gamepad.wButtons && XINPUT_GAMEPAD_A)
		return true;
	else
		return false;
}

bool cController::checkConnection()
{
	ZeroMemory(&state, sizeof(XINPUT_STATE));

	dwXIndex0 = XInputGetState(0, &state);

	// If there was no error trying to get the first controller, then it is plugged in
	if (dwXIndex0 == ERROR_SUCCESS)
	{
		return true;
	}
	else
	{
		return false;
		//cout << "Xbox controller not detected" << endl;
	}

}