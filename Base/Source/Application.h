#ifndef APPLICATION_H
#define APPLICATION_H
#include "timer.h"
#include "SceneTerrain.h"
#include "SceneSkyPlane.h"

#include <vector>
using std::vector;

class Application
{
public:
	static Application& GetInstance()
	{
		static Application app;
		return app;
	}
	void Init();
	void Run();
	void Exit();
	static bool IsKeyPressed(unsigned short key);
	bool getKeyboardUpdate();

	//mouse
	static double mouse_last_x, mouse_last_y, mouse_current_x, mouse_current_y, mouse_diff_x, mouse_diff_y;

	static bool GetMouseUpdate();
	static double getCameraYaw();
	static double getCameraPitch();

	static double getYawAngle();
	static double  getPitchAngle();
	static void setYawAngle(float y);
	static void setPitchAngle(float p);
	static float yaw;
	static float pitch;

	//change state
	void changeState(double& time, double& timer, double dt);

	/* getter setter */
	static float getWidth(){return  m_window_width;}
	static float getHeight(){return  m_window_height;}
	static bool* getMyKeys(){return myKeys;}
private:
	Application();
	~Application();

	//Declare a window object
	StopWatch m_timer;
	double m_dElapsedTime;
	double m_dAccumulatedTime_thread1;
	double m_dAccumulatedTime_thread2;

	//mouse
	static double camera_yaw, camera_pitch;
	const static int m_window_deadzone = 100;
	const static int m_window_width = 800;
	const static int m_window_height = 600;
	static double yawAngle, pitchAngle;

	//state
	SCENE_STATES sceneState;
	bool stateSwitched;	//use bool to check if state can be switched again to avoid if(timer >= time), consumes cpu
	//vector of states
	vector<Scene*> allStates;
	double changeTime;
	double changeTimer;

	//keys
	static bool myKeys[255];
};

#endif