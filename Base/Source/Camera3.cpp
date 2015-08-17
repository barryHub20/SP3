#include "Camera3.h"
#include "Controller.h"
#include "Mtx44.h"

/** camera pos should be based on player class **/
bool jump = false;
Vector3 velocity(0, 69, 0);	//value is 0 for horizontal(X and Z) and 20 for vertical(Y ) Larger the value, further the jump
Vector3 jumpVel(velocity);
Vector3 m_gravity(0, -9.8f, 0);
float speed = 19.f;
float groundLevelY = 20.f;	//store the ground level Y

static const float CAMERA_SPEED = 150.f;	//use velocity from now on
static const float CAMERA_ACCELERATION = 50.f;
static const float CAMERA_MAX_VELOCITY = 200.f;
static const float CAMERA_BRAKE_OFFSET = 2.5f;
//if move mouse, acclerate till reach max speed
//if !move mouse and vel > 0, start decelerating

Camera3::Camera3()
{
}

Camera3::~Camera3()
{
}

void Camera3::Init(const Vector3& pos, const Vector3& target, const Vector3& up)
{
	this->position = defaultPosition = pos;
	this->target = defaultTarget = target;
	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	this->up = defaultUp = right.Cross(view).Normalized();
	sCameraType = LAND_CAM;

	pitchMovingUp = pitchMovingDown = yawMovingLeft = yawMovingRight = false;
	walkMovingForward = false;		//is position still changing
	walkMovingBackward = false;
	straftMovingLeft = false;
	straftMovingRight = false;
	pitchVelocity = yawVelocity = 0.f;	//used for x and z (or y if flying)
	movingVelocity = false;	//changing of pos

	straftVelocity = 0.f;
	movingVelocity = 0.f;
	moveAcceleration = 600.f;
	camAcceleration = 10.f;
	camBrakeOffset = 2.5f;
	cameraSpeed = 3.f;
	 yawAngle = 0.f;	//facing X
	 croutching = false;
}

/********************************************************************************
Set Camera Type
********************************************************************************/
void Camera3::SetCameraType(CAM_TYPE sCameraType)
{
	this->sCameraType = sCameraType;
}

/********************************************************************************
Get Camera Type
********************************************************************************/
Camera3::CAM_TYPE Camera3::GetCameraType(void)
{
	return sCameraType;
}

/******************************************************************************
**
Walk forward or backward. You can add in a deadzone here.
*******************************************************************************
*/

void Camera3::Walk(const double dt)
{
	//since -dt denotes backwards, vel is negative and -vel * -dt = positive result, so will when intent is to move back pos moves forward
	//therfore dt for backward pass in -dt, to negate the (-dt) parameter
	/* move forward */
	if(dt > 0)	//forward
	{
		walkMovingForward = true;
		walkMovingBackward = false;
	}
	else
	{
		walkMovingBackward = true;
		walkMovingForward = false;
	}

	/* update vel */
	movingVelocity += moveAcceleration * dt;

	/* cap vel */
	if(movingVelocity > CAMERA_MAX_VELOCITY)
		movingVelocity = CAMERA_MAX_VELOCITY;
	else if(movingVelocity < -CAMERA_MAX_VELOCITY)
		movingVelocity = -CAMERA_MAX_VELOCITY;
	
	if(dt >= 0)
		MoveForward(dt);
	else if(dt < 0)
		MoveBackward(-dt);	
}

/******************************************************************************
**
Strafe left or right. You can add in a deadzone here.
*******************************************************************************
*/
void Camera3::Strafe(const double dt)
{
	if(dt > 0)	//right
	{
		straftMovingRight = true;
		straftMovingLeft = false;
	}
	else
	{
		straftMovingRight = false;
		straftMovingLeft = true;
	}

	/* update */
	straftVelocity += moveAcceleration * dt;

	/* cap vel */
	if(straftVelocity > CAMERA_MAX_VELOCITY)
		straftVelocity = CAMERA_MAX_VELOCITY;
	else if(straftVelocity < -CAMERA_MAX_VELOCITY)
		straftVelocity = -CAMERA_MAX_VELOCITY;
		
	
	if(dt < 0)
		MoveLeft(-dt);
	else if(dt >= 0)
		MoveRight(dt);
}

void Camera3::DecelerateBackward(const double dt) //(-dt)
{
	if(movingVelocity < 0)
	{
		movingVelocity += -moveAcceleration * dt;

		MoveBackward(-dt);	//same explanation in walk()
	}
	else
	{
		movingVelocity = 0.f;
		walkMovingBackward = false;
	}
}
	
void Camera3::DecelerateForward(const double dt) //(dt)
{
	if(movingVelocity > 0)
	{
		movingVelocity += -moveAcceleration * dt;
		MoveForward(dt);
	}
	else
	{
		movingVelocity = 0.f;
		walkMovingForward = false;
	}
}

void Camera3::DecelerateLeft(const double dt)
{
	if(straftVelocity < 0)
	{
		straftVelocity += -moveAcceleration * dt;
		MoveLeft(-dt);
	}
	else
	{
		straftVelocity = 0.f;
		straftMovingLeft = false;
	}
}

void Camera3::DecelerateRight(const double dt)
{
	if(straftVelocity > 0)
	{
		straftVelocity += -moveAcceleration * dt;
		MoveRight(dt);
	}
	else
	{
		straftVelocity = 0.f;
		straftMovingRight = false;
	}
}

/********************************************************************************
Fly
********************************************************************************/
void Camera3::Fly(const double dt)
{
	if(dt > 0)
		MoveUp(dt);
	else if(dt < 0)
		MoveDown(dt);
}

/******************************************************************************
**
Pitch. You can add in a deadzone here. up (-) down (+)
*******************************************************************************
*/

void Camera3::Pitch(const double dt)
{
	/******************** set flags *********************/
	bool Up = Controller::getCameraPitch() < 0.f;
	bool Down = Controller::getCameraPitch() > 0.f;

	if(Up)
	{
		pitchMovingUp = true;
	}
	else if(Down)
	{
		pitchMovingDown = true;
	}

	/******************** update *************************/
	if(pitchMovingUp || pitchMovingDown) //pitch < 0
		pitchVelocity += -camAcceleration * Controller::getCameraPitch() * (float)dt;

	/******************** cap *************************/
	if(pitchVelocity > CAMERA_MAX_VELOCITY)
		pitchVelocity = CAMERA_MAX_VELOCITY;
	else if(pitchVelocity < -CAMERA_MAX_VELOCITY)
		pitchVelocity = -CAMERA_MAX_VELOCITY;

	/******************** decelerate *************************/
	if(!Up && pitchMovingUp)
		DecelerateUp(dt);
	else if(!Down && pitchMovingDown)
		DecelerateDown(dt);

	if(Up)
		LookUp(dt);
	else if(Down)
		LookDown(dt);
}

void Camera3::Yaw(const double dt)
{
	/******************** set flags *********************/
	bool Left = Controller::getCameraYaw() < 0.f;
	bool Right = Controller::getCameraYaw() > 0.f;

	if(Left)//(+vel)
	{
		yawMovingLeft = true;
	}
	else if(Right)//(-vel)
	{
		yawMovingRight = true;
	}
	
	/******************** update *************************/
	if(yawMovingRight || yawMovingLeft)
	{
		yawVelocity += -camAcceleration * Controller::getCameraYaw() * (float)dt;
		yawAngle += -camAcceleration * (float)dt;
	}

	/******************** cap *************************/
	if(yawVelocity > CAMERA_MAX_VELOCITY)
		yawVelocity = CAMERA_MAX_VELOCITY;
	else if(yawVelocity < -CAMERA_MAX_VELOCITY)
		yawVelocity = -CAMERA_MAX_VELOCITY;

	/******************** decelerate *************************/
	if(!Left && yawMovingLeft)
		DecelerateLookLeft(dt);
	else if(!Right && yawMovingRight)
		DecelerateLookRight(dt);
	
	if(Left) 
		LookLeft(dt);
	else if(Right)	
		LookRight(dt);
}

void Camera3::DecelerateUp(const double dt)
{
	if(pitchVelocity - (camAcceleration * camBrakeOffset * dt) > 0)
	{
		pitchVelocity -= camAcceleration * camBrakeOffset * dt;
		LookUp(dt);
	}
	else
	{
		pitchVelocity = 0.f;
		pitchMovingUp = false;
	}
}

void Camera3::DecelerateDown(const double dt)
{
	if(pitchVelocity + (camAcceleration * camBrakeOffset * dt) < 0)
	{
		pitchVelocity += camAcceleration * camBrakeOffset * dt;
		LookDown(dt);
	}
	else
	{
		pitchVelocity = 0.f;
		pitchMovingDown = false;
	}
}

void Camera3::DecelerateLookLeft(const double dt)
{
	if(yawVelocity - (camAcceleration * camBrakeOffset * dt) > 0)
	{
		yawVelocity -= camAcceleration * camBrakeOffset * dt;
		yawAngle -= camAcceleration * camBrakeOffset * dt;
		LookLeft(dt);
	}
	else
	{
		yawVelocity = 0.f;
		yawMovingLeft = false;
	}
}

void Camera3::DecelerateLookRight(const double dt)
{
	if(yawVelocity + (camAcceleration * camBrakeOffset * dt) < 0)
	{
		yawVelocity += camAcceleration * camBrakeOffset * dt;
		yawAngle += camAcceleration * camBrakeOffset * dt;
		LookRight(dt);
	}
	else
	{
		yawVelocity = 0.f;
		yawMovingRight = false;
	}
}

/********************************************************************************
LookUp
********************************************************************************/
void Camera3::LookUp(const double dt)
{
	float pitch = pitchVelocity;
	
	//Controller::incrementPitchAngle(pitch);
	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	up = right.Cross(view).Normalized();
	Mtx44 rotation;
	rotation.SetToRotation(pitch, right.x, right.y, right.z);
	view = rotation * view;
	target = position + view;

	up = rotation * up;
}

/********************************************************************************
LookDown
********************************************************************************/
void Camera3::LookDown(const double dt)
{
	float pitch = pitchVelocity;

	//Controller::incrementPitchAngle(pitch);
	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	up = right.Cross(view).Normalized();
	Mtx44 rotation;
	rotation.SetToRotation(pitch, right.x, right.y, right.z);
	view = rotation * view;
	target = position + view;

	up = rotation * up;
}

/********************************************************************************
LookLeft
********************************************************************************/
void Camera3::LookLeft(const double dt)
{
	float yaw = yawVelocity;
	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	up = right.Cross(view).Normalized();
	Mtx44 rotation;
	rotation.SetToRotation(yaw, 0, 1, 0);
	view = rotation * view;
	target = position + view;

	up = rotation * up;
}

/********************************************************************************
LookRight
********************************************************************************/
void Camera3::LookRight(const double dt)
{
	float yaw = yawVelocity;
	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	up = right.Cross(view).Normalized();
	Mtx44 rotation;
	rotation.SetToRotation(yaw, 0, 1, 0);
	view = rotation * view;
	target = position + view;

	up = rotation * up;
}

void Camera3::Update(double dt, bool* myKeys)
{/*
	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);
	right.y = 0;
	Inertia: instant dir change

	* strafe */
	if(myKeys[KEY_A] && !myKeys[KEY_D])
		Strafe(-dt);
	else if(straftMovingLeft)
		DecelerateLeft(-dt);

	if(myKeys[KEY_D] && !myKeys[KEY_A])
		Strafe(dt);
	else if(straftMovingRight)
		DecelerateRight(dt);

	/* walk */
	if(myKeys[KEY_W] && !myKeys[KEY_S])
		Walk(dt);
	else if(walkMovingForward)
		DecelerateForward(dt);

	if(myKeys[KEY_S] && !myKeys[KEY_W])
		Walk(-dt);
	else if(walkMovingBackward)
		DecelerateBackward(-dt);

	if(myKeys[KEY_C] && !croutching)
		croutching = true;

	croutch(dt);


	/* fly */
	if(myKeys[KEY_K])
		Fly(dt);
	if(myKeys[KEY_L])
		Fly(-dt);

	if(myKeys[KEY_SPACE] && !jump)	//com runs too fast, spacebar 2 times
		setJump(dt);

	if(myKeys[KEY_T])	//reset
	{
		Reset();
	}
	

	/** update jump **/
	if(jump)
		Jump(dt);
	

	/** mouse **/
	Yaw(dt);
	Pitch(dt);
}

/******************************************************************************
Move the camera forward
******************************************************************************/
void Camera3::MoveForward(const double dt)
{
	Vector3 tmp_target(target.x, 0, target.z);
	Vector3 tmp_pos(position.x, 0, position.z);
	Vector3 view = (tmp_target - tmp_pos).Normalized();
	position += view * movingVelocity * (float)dt;
	target += view * movingVelocity * (float)dt;

	//reset flag
	//myKeys[key] = false;
}
/******************************************************************************
Move the camera backward
******************************************************************************/
void Camera3::MoveBackward(const double dt)
{
	Vector3 tmp_target(target.x, 0, target.z);
	Vector3 tmp_pos(position.x, 0, position.z);
	Vector3 view = (tmp_target - tmp_pos).Normalized();
	position += view * movingVelocity * (float)dt;
	target += view * movingVelocity * (float)dt;

	//reset flag
	//myKeys[key] = false;
}
/******************************************************************************
Move the camera left
******************************************************************************/
void Camera3::MoveLeft(const double dt)
{
	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	position += right * straftVelocity * (float)dt;
	target += right * straftVelocity * (float)dt;

	//reset flag
	//myKeys[key] = false;
}
/******************************************************************************
Move the camera right
******************************************************************************/
void Camera3::MoveRight(const double dt)
{
	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	position += right * straftVelocity * (float)dt;
	target += right * straftVelocity * (float)dt;

	//reset flag
	//myKeys[key] = false;
}

/******************************************************************************
Move the camera up
******************************************************************************/
void Camera3::MoveUp(const double dt)
{
	Vector3 upupup(0, 1, 0);
	position += upupup * CAMERA_SPEED * (float)dt;
	target += upupup * CAMERA_SPEED * (float)dt;
	//reset flag
	//myKeys[key] = false;
}

/******************************************************************************
Move the camera down
******************************************************************************/
void Camera3::MoveDown(const double dt)
{
	Vector3 upupup(0, 1, 0);
		position += upupup * CAMERA_SPEED * (float)dt;
		target += upupup * CAMERA_SPEED * (float)dt;
	//reset flag
	//myKeys[key] = false;
}

/******************************************************************************
Jump
******************************************************************************/
void Camera3::Jump(const double dt)
{
	jumpVel += m_gravity * dt * speed;
	position += jumpVel * dt * speed;
	target += jumpVel * dt * speed;
		
	//once reach groundw
	if(position.y <= groundLevelY)	//if next frame touches ground
	{
		target.y += groundLevelY - position.y;
		position.y = groundLevelY;
		jump = false;
		jumpVel = velocity;
	}
}

void Camera3::setJump(const double dt)
{
	Vector3 view = (target - position).Normalized();	//dir of jump (if velocity X and Z are not 0)
	//init velocity
	jumpVel.x *= view.x;
	jumpVel.z *= view.z;
	jump = true;

	//reset flag
	//myKeys[key] = false;
}

void Camera3::TurnLeft(const double dt)
{
}

void Camera3::TurnRight(const double dt)
{
}

void Camera3::SpinClockWise(const double dt)
{
}

void Camera3::SpinCounterClockWise(const double dt)
{
}

void Camera3::Roll(const double dt)
{
}

/******************************************************************************
Update camera status
******************************************************************************/
void Camera3::UpdateStatus(const unsigned char key)
{
	//myKeys[key] = true;
}

void Camera3::Reset()
{
	position = defaultPosition;
	target = defaultTarget;
	up = defaultUp;
}

void Camera3::croutch(const double dt)
{
	
}
