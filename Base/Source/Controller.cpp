#include "Controller.h"

//Include the standard C++ headers
#include <stdio.h>
#include <stdlib.h>

const unsigned char FPS = 60; // FPS of this game (ONLY RELEASE MODE CAN RUN SMOOTHLY AT 170FPS MAX)
const unsigned int frameTime = 1000 / FPS; // time for each frame

//mouse
char Controller::inputChar[KEY_6 + 1] = {0};
bool Controller::myKeys[TOTAL_KEY] = {false};
double Controller::mouse_last_x = 0.0, Controller::mouse_last_y = 0.0,
Controller::mouse_current_x = 0.0, Controller::mouse_current_y = 0.0,
Controller::mouse_diff_x = 0.0, Controller::mouse_diff_y = 0.0;
int Controller::mouseRightButton = 0, Controller::mouseLeftButton = 0;
double Controller::camera_yaw = 0.0, Controller::camera_pitch = 0.0;
double Controller::scrollxPos = 0.0;
double Controller::scrollyPos = 0.0;

//yaw and pitch angle
double Controller::yawAngle = 0.0;
double Controller::pitchAngle = 0.0;

void scroll(GLFWwindow* window,double x,double y)
{
	Controller::setScrollX(x);
	Controller::setScrollY(y);
}

/********************* constructor / destructor *********************/
Controller::Controller() : model(NULL), view(NULL)
{
}

Controller::Controller(View* view) : view(view)
{
}

Controller::~Controller()
{
	model = NULL;
	view = NULL;
}

/******************** core functions **********************/
void Controller::Init()
{
	//set both timers to 0
	m_dElapsedTime = 0.0;
	m_dAccumulatedTime_thread1 = 0.0;
	m_dAccumulatedTime_thread2 = 0.0;

	//keys
	for(int i = 0; i <= KEY_6; ++i) //all 256 chars
		myKeys[i] = false;

	inputChar[KEY_W] = 'W';
	inputChar[KEY_A] = 'A';
	inputChar[KEY_S] = 'S';
	inputChar[KEY_D] = 'D';
	inputChar[KEY_C] = 'C';
	inputChar[KEY_K] = 'K';
	inputChar[KEY_L] = 'L';
	inputChar[KEY_T] = 'T';
	inputChar[KEY_I] = 'I';
	inputChar[KEY_O] = 'O';
	inputChar[KEY_B] = 'B';
	inputChar[KEY_V] = 'V';
	inputChar[KEY_E] = 'E';
	inputChar[KEY_SPACE] = ' ';
	inputChar[KEY_1] = '1';
	inputChar[KEY_2] = '2';
	inputChar[KEY_3] = '3';
	inputChar[KEY_4] = '4';
	inputChar[KEY_5] = '5';
	inputChar[KEY_6] = '6';

	//mouse button
	mouseLeftButton = glfwGetMouseButton(glfwGetCurrentContext(), GLFW_MOUSE_BUTTON_LEFT);
	mouseRightButton = glfwGetMouseButton(glfwGetCurrentContext(), GLFW_MOUSE_BUTTON_RIGHT);
	scrollyPos = scrollxPos = 0.0;

	//init view
	view->Init();

	//set model
	model = view->getModel();

	//hide the cursor
	glfwSetInputMode(view->getWindow(), GLFW_CURSOR, GLFW_CURSOR_HIDDEN);


	model->Init();
}

void Controller::Run()
{
	//Main Loop
	m_timer.startTimer();    // Start timer to calculate how long it takes to render this frame

	while (!glfwWindowShouldClose(view->getWindow()) && !IsKeyPressed(VK_ESCAPE))
	{
		//get the elapsed time
		m_dElapsedTime = m_timer.getElapsedTime();
		m_dAccumulatedTime_thread1 += m_dElapsedTime;
		m_dAccumulatedTime_thread2 += m_dElapsedTime;

		/* fps */
		fps = (float)(1.f / m_dElapsedTime);

		/* twin threaded approach */
		if(m_dAccumulatedTime_thread1 > 0.01)	//update: update fps is _dAccumulatedTime_thread1 > fps
		{
			/** controller update **/
			GetMouseUpdate();
			getKeyboardUpdate();

			/** model update **/
			model->Update(m_dElapsedTime, myKeys, GetMousePos());

			m_dAccumulatedTime_thread1 = 0.0;
		}
		if(m_dAccumulatedTime_thread2 > 0.003)	//render: render fps is _dAccumulatedTime_thread1 > fps
		{
			/** View update(rendering) **/
			view->Render(fps);	//or switch to pause screen

			m_dAccumulatedTime_thread2 = 0.0;
		}

		if(myKeys[KEY_LMOUSE] || myKeys[KEY_RMOUSE])
			UpdateMouseClick();
		
		//Swap buffers
		glfwSwapBuffers(view->getWindow());
		//Get and organize events, like keyboard and mouse input, window resizing, etc...
		glfwPollEvents();
		m_timer.waitUntil(frameTime);       // Frame rate limiter. Limits each frame to a specified time in ms.   

	} //Check if the ESC key had been pressed or if the window had been closed
	
	view->Exit();
	model->Exit();
}

void Controller::Exit()
{
}

/********************** controller functions **********************/
bool Controller::IsKeyPressed(unsigned short key)
{
	return ((GetAsyncKeyState(key) & 0x8001) != 0);
}

bool Controller::getKeyboardUpdate()
{
	/** Set all keys to false **/
	for(unsigned i = 0; i < TOTAL_KEY; ++i)
		myKeys[i] = false;

	/**** See which keys are pressed ****/
	/** Keyboard **/
	for(int i = 0; i <= KEY_6; ++i) //all 256 chars
	{
		if(IsKeyPressed(inputChar[i]))
			myKeys[i] = true;
	}

	/** non-keyboard(mouse) **/
	mouseLeftButton = glfwGetMouseButton(glfwGetCurrentContext(), GLFW_MOUSE_BUTTON_LEFT);
	mouseRightButton = glfwGetMouseButton(glfwGetCurrentContext(), GLFW_MOUSE_BUTTON_RIGHT);

	if(mouseLeftButton == GLFW_PRESS)
		myKeys[KEY_LMOUSE] = true;
	if(mouseRightButton == GLFW_PRESS)
		myKeys[KEY_RMOUSE] = true;

	/** Scrolling **/
	GLFWwindow* glfwGetCurrentContext(void);
	glfwSetScrollCallback(glfwGetCurrentContext(), scroll);

	if(scrollyPos > 0.0)
	{
		myKeys[SCROLL_TOP] = true;
	}
	else if(scrollyPos < 0.0)
	{
		myKeys[SCROLL_BOTTOM] = true;
	}

	if(scrollyPos != 0.0)
	{
		scrollyPos = 0.0;
	}

	/* mouse */
	return true;
}

void Controller::setScrollX(double p)
{
	Controller::scrollxPos = p;
}

void Controller::setScrollY(double p)
{
	Controller::scrollyPos = p;
}

bool Controller::KeyPressed(KEYS key)
{
	return myKeys[key];
}

void Controller::UpdateMouseClick()
{
}

/********************** mouse **********************/
bool Controller::GetMouseUpdate()
{
	glfwGetCursorPos(view->getWindow(), &mouse_current_x, &mouse_current_y);

	// Calculate the difference in positions
	mouse_diff_x = mouse_current_x - mouse_last_x;
	mouse_diff_y = mouse_current_y - mouse_last_y;
/*	//Calculate the yaw and pitch
	camera_yaw = (float) mouse_diff_x * 0.0174555555555556f;// * 3.142f / 180.0f;
	camera_pitch = (float) mouse_diff_y * 0.0174555555555556f;// 3.142f / 180.0f );
	
	// Do a wraparound if the mouse cursor has gone out of the deadzone
	if ((mouse_current_x < m_window_deadzone) || (mouse_current_x > view->getConsoleWidth() - m_window_deadzone))
	{
		mouse_current_x = view->getConsoleWidth() >> 1;
		glfwSetCursorPos(view->getWindow(), mouse_current_x, mouse_current_y);
	}
	if ((mouse_current_y < m_window_deadzone) || (mouse_current_y > view->getConsoleHeight() - m_window_deadzone))
	{
		mouse_current_y = view->getConsoleHeight() >> 1;
		glfwSetCursorPos(view->getWindow(), mouse_current_x, mouse_current_y);
	} */
	// Store the current position as the last position
	mouse_last_x = mouse_current_x;
	mouse_last_y = mouse_current_y;
	 return false;
}

double Controller::getCameraYaw()
{
	return camera_yaw;
}

double Controller::getCameraPitch()
{
	return camera_pitch;
}

Vector3 Controller::GetMousePos()
{
	Vector3 r(model->get2DViewWidth() * (mouse_current_x * (1.f / View::getConsoleWidth())), 
		 (model->get2DViewHeight() *  ((View::getConsoleHeight() - mouse_current_y) * (1.f / View::getConsoleHeight()))),
		0);
	return r;
}

/********************** getter setter **********************/
bool* Controller::getKeyPressed(){return myKeys;}
double Controller::getYawAngle(){return yawAngle;}
double  Controller::getPitchAngle(){return pitchAngle;}
void Controller::incrementPitchAngle(float i){pitchAngle += i;}
void Controller::incrementYawAngle(float i){yawAngle += i;}
void Controller::setYawAngle(float y){yawAngle = y;}
void Controller::setPitchAngle(float p){pitchAngle = p;}
float Controller::getFPS(){return fps;}
