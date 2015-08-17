
#include "Application.h"
//Include GLEW
#include <GL/glew.h>

//Include GLFW
#include <GLFW/glfw3.h>

//Include the standard C++ headers
#include <stdio.h>
#include <stdlib.h>

GLFWwindow* m_window;
const unsigned char FPS = 60; // FPS of this game
const unsigned int frameTime = 1000 / FPS; // time for each frame

//mouse
double Application::mouse_last_x = 0.0, Application::mouse_last_y = 0.0,
 Application::mouse_current_x = 0.0, Application::mouse_current_y = 0.0,
 Application::mouse_diff_x = 0.0, Application::mouse_diff_y = 0.0;
double Application::camera_yaw = 0.0, Application::camera_pitch = 0.0;

//yaw and pitch angle
double Application::yawAngle = 0.0;
double Application::pitchAngle = 0.0;
float Application::yaw = 90.0;
float Application::pitch = 0.0;


//Define an error callback
static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
	_fgetchar();
}

//Define the key input callback
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

void resize_callback(GLFWwindow* window, int w, int h)
{
	glViewport(0, 0, w, h);
}

//myKeys
bool Application::myKeys[255];

bool Application::IsKeyPressed(unsigned short key)
{
    return ((GetAsyncKeyState(key) & 0x8001) != 0);
}

Application::Application()
{
}

Application::~Application()
{
}

void Application::Init()
{
	//Set the error callback
	glfwSetErrorCallback(error_callback);

	//Initialize GLFW
	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}

	//Set the GLFW window creation hints - these are optional
	glfwWindowHint(GLFW_SAMPLES, 4); //Request 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //Request a specific OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //Request a specific OpenGL version
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL 


	//Create a window and create its OpenGL context
	m_window = glfwCreateWindow(m_window_width, m_window_height, "DM2231_Framework", NULL, NULL);

	//If the window couldn't be created
	if (!m_window)
	{
		fprintf( stderr, "Failed to open GLFW window.\n" );
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	//This function makes the context of the specified window current on the calling thread. 
	glfwMakeContextCurrent(m_window);

	//Sets the key callback
	//glfwSetKeyCallback(m_window, key_callback);
	glfwSetWindowSizeCallback(m_window, resize_callback);

	glewExperimental = true; // Needed for core profile
	//Initialize GLEW
	GLenum err = glewInit();

	//If GLEW hasn't initialized
	if (err != GLEW_OK) 
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		//return -1;
	}

	//hide the cursor
	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//set both timers to 0
	m_dElapsedTime = 0.0;
	m_dAccumulatedTime_thread1 = 0.0;
	m_dAccumulatedTime_thread2 = 0.0;

	//scene state
	sceneState = SCENE_GAME;
	stateSwitched = false;

	//change state timer
	changeTime = 0.2;
	changeTimer = changeTime;
	
	//assign states (ALL scenes MUST USE THE SAME SHADERS?)
	Scene* assign_scene = NULL;

	for(int i = 0; i <TOTAL_SCENE; ++i)
	{
		switch (i)
		{
			case SCENE_GAME:
				assign_scene = new SceneTerrain();
				break;
			case SCENE_PAUSE:
				assign_scene = new SceneSkyPlane();
				break;
		};
		allStates.push_back(assign_scene);
	}

	//keys
	for(int i = 0; i < 255; ++i) //all 256 chars
		myKeys[i] = true;
}

void Application::Run()
{
	//Main Loop
	m_timer.startTimer();    // Start timer to calculate how long it takes to render this frame

	while (!glfwWindowShouldClose(m_window) && !IsKeyPressed(VK_ESCAPE))
	{
		//get the elapsed time
		m_dElapsedTime = m_timer.getElapsedTime();
		m_dAccumulatedTime_thread1 += m_dElapsedTime;
		m_dAccumulatedTime_thread2 += m_dElapsedTime;

		changeState(changeTime, changeTimer, m_dElapsedTime);

		/* WORK but lag SINCE UPDATES EVERY 1 SECOND WHILE RENDER EVERY FRAME 
		if(m_dAccumulatedTime > 1.0)
		{
			allStates[sceneState]->Update(m_dElapsedTime);
			m_dAccumulatedTime = 0.0;
			//show pause menU
		}*/

		/* twin threaded approach */
		if(m_dAccumulatedTime_thread1 > 0.01)	//update: update fps is _dAccumulatedTime_thread1 > fps
		{
			GetMouseUpdate();
			getKeyboardUpdate();
			allStates[sceneState]->Update(m_dElapsedTime);
			m_dAccumulatedTime_thread1 = 0.0;
		}
		if(m_dAccumulatedTime_thread2 > 0.003)	//render: render fps is _dAccumulatedTime_thread1 > fps
		{
			allStates[sceneState]->Render();	//or switch to pause screen
			m_dAccumulatedTime_thread2 = 0.0;
		}
		
		//Swap buffers
		glfwSwapBuffers(m_window);
		//Get and organize events, like keyboard and mouse input, window resizing, etc...
		glfwPollEvents();
		//std::cout << counter << std::endl;
		
		m_timer.waitUntil(frameTime);       // Frame rate limiter. Limits each frame to a specified time in ms.   

	} //Check if the ESC key had been pressed or if the window had been closed
	allStates[sceneState]->Exit();
	delete allStates[sceneState];
}

void Application::Exit()
{
	//Close OpenGL window and terminate GLFW
	glfwDestroyWindow(m_window);
	//Finalize and clean up GLFW
	glfwTerminate();
}

//mouse
bool Application::GetMouseUpdate()
{
	glfwGetCursorPos(m_window, &mouse_current_x, &mouse_current_y);
	// Calculate the difference in positions
	mouse_diff_x = mouse_current_x - mouse_last_x;
	mouse_diff_y = mouse_current_y - mouse_last_y;
	//Calculate the yaw and pitch
	camera_yaw = (float) mouse_diff_x * 0.0174555555555556f;// * 3.142f / 180.0f;
	camera_pitch = mouse_diff_y * 0.0174555555555556f;// 3.142f / 180.0f );

	// Do a wraparound if the mouse cursor has gone out of the deadzone
	if ((mouse_current_x < m_window_deadzone) || (mouse_current_x > m_window_width - m_window_deadzone))
	{
		mouse_current_x = m_window_width >> 1;
		glfwSetCursorPos(m_window, mouse_current_x, mouse_current_y);
	}
	if ((mouse_current_y < m_window_deadzone) || (mouse_current_y > m_window_height - m_window_deadzone))
	{
		mouse_current_y = m_window_height >> 1;
		glfwSetCursorPos(m_window, mouse_current_x, mouse_current_y);
	}
	// Store the current position as the last position
	mouse_last_x = mouse_current_x;
	mouse_last_y = mouse_current_y;
	 return false;
}

double Application::getCameraYaw()
{
	return camera_yaw;
}

double Application::getYawAngle()
{
	return yawAngle;
}

double  Application::getPitchAngle()
{
	return pitchAngle;
}

void Application::setYawAngle(float y)
{
	yawAngle = y;
}

void Application::setPitchAngle(float p)
{
	pitchAngle = p;
}

double Application::getCameraPitch()
{
	return camera_pitch;
}

bool Application::getKeyboardUpdate()
{
	//any way to make this whole stuff to 2 lines?
	//doneee
	for(int i = 0; i < 255; ++i)	//loop thru all chars
	{
		if(IsKeyPressed(i))
			myKeys[i] = true;
		else
			myKeys[i] = false;
	}

	/* mouse */
	return true;
}

//pause
void Application::changeState(double& time, double& timer, double dt)
{
	/* timers */
	//update switch scene timer
	if(timer < time)
		timer += dt;
	else
		stateSwitched = false;

	///* change of state */
	//if(IsKeyPressed('P') && sceneState != SCENE_PAUSE && stateSwitched == false)	//if press P, pause/unpause if timer reaches limit
	//{
	//	stateSwitched = true;
	//	sceneState = SCENE_PAUSE;
	//	/* pause changes */
	//	//glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	//	timer = 0.0;
	//}
	//if(IsKeyPressed('P') && sceneState == SCENE_PAUSE && stateSwitched == false)
	//{
	//	stateSwitched = true;
	//	sceneState = SCENE_GAME;
	//	//glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	//	timer = 0.0;
	//}

	/* only scenes are called then they are initialized, etc. if pause scene not called at all, it does not even initialize */
	if(allStates[sceneState]->getFirstCall() == true)
	{
		allStates[sceneState]->Init();
		allStates[sceneState]->setFirstCall(false);
	}
}