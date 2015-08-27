#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif


#ifndef CONTROLLER_H
#define CONTROLLER_H
#include "Model.h"
#include "View.h"
#include "View.h"
#include "timer.h"

class Controller
{
public:
	/********************** enums **********************/
	enum MODE
	{
		TWO_D,
		THREE_D,
	};

	/******************** core functions **********************/
	void Init();
	void Run();
	void Exit();

	/********************** controller functions **********************/
	static bool IsKeyPressed(unsigned short key);
	static bool KeyPressed(KEYS key);
	static bool getKeyboardUpdate();

	/********************** mouse **********************/
	static double mouse_last_x, mouse_last_y, mouse_current_x, mouse_current_y, mouse_diff_x, mouse_diff_y;

	bool GetMouseUpdate();
	static double getCameraYaw();
	static double getCameraPitch();
	void UpdateMouseClick();	//any functions that are activated only when player clicks mouse

	/********************** getter setter **********************/
	bool* getKeyPressed();
	static double getYawAngle();
	static double  getPitchAngle();
	static void incrementPitchAngle(float i);
	static void incrementYawAngle(float i);
	static void setYawAngle(float y);
	static void setPitchAngle(float p);
	static void setScrollX(double p);
	static void setScrollY(double p);
	
	Vector3 GetMousePos();
	float getFPS();

	/********************* constructor / destructor *********************/
	Controller();
	Controller(View* view);
	~Controller();
private:
	/********************** Input **********************/
	MODE type;	//mode
	static char inputChar[KEY_6 + 1];	//char inputs from keyboard only until key 6, the rest are mouse inputs
	static bool myKeys[TOTAL_KEY];

	/********************** model and view ptr **********************/
	Model* model; 
	View* view;

	/********************** Declare a window object **********************/
	StopWatch m_timer;
	double m_dElapsedTime;
	double m_dAccumulatedTime_thread1;
	double m_dAccumulatedTime_thread2;

	/********************** mouse **********************/
	static double camera_yaw, camera_pitch;
	const static int m_window_deadzone = 100;
	static double yawAngle, pitchAngle;
	static int mouseRightButton;
	static int mouseLeftButton;

	//scroll
	static double scrollxPos;
	static double scrollyPos;

	float fps;
};

#endif