#ifndef CAMERA_3_H
#define CAMERA_3_H

#include "Camera.h"

class Camera3 : public Camera
{
public:// Update Camera status
	enum CAM_TYPE 
	{ 
		LAND_CAM,			
		AIR_CAM,			
		NUM_CAM_TYPE 
	};

	Vector3 defaultPosition;
	Vector3 defaultTarget;
	Vector3 defaultUp;
	CAM_TYPE sCameraType;

	float yawAngle;

			/* bool flags for inertia */
			bool pitchMovingUp, pitchMovingDown;
			bool yawMovingRight, yawMovingLeft;
			bool walkMovingForward;	
			bool walkMovingBackward;
			bool straftMovingLeft;
			bool straftMovingRight;

			bool croutching;

			/* inertia variables */
			float pitchVelocity, yawVelocity;	//up / down    left / right
			float movingVelocity;	//forward / backward
			float straftVelocity;	//left / right
			float moveAcceleration;
			float camAcceleration;	//pitch / yaw
			float camBrakeOffset;	//for rotating
			float cameraSpeed;		//speed

	virtual void SetCameraType(CAM_TYPE sCameraType);
	virtual CAM_TYPE GetCameraType(void);

	Camera3();
	~Camera3();
	virtual void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
	virtual void Update(double dt, bool* myKeys);
	virtual void Reset();
	virtual void UpdateStatus(const unsigned char key);

	virtual void SpinClockWise(const double dt);
	virtual void SpinCounterClockWise(const double dt);

	/* inertia functions */
			virtual void TurnLeft(const double dt);
			virtual void TurnRight(const double dt);
			virtual void LookUp(const double dt);
			virtual void LookDown(const double dt);
			virtual void LookLeft(const double dt);
			virtual void LookRight(const double dt);

			void MoveForward(const double dt);
			void MoveBackward(const double dt);
			void MoveLeft(const double dt);
			void MoveRight(const double dt);
			void MoveUp(const double dt);
			void MoveDown(const double dt);

			void DecelerateBackward(const double dt);
			void DecelerateForward(const double dt);
			void DecelerateLeft(const double dt);
			void DecelerateRight(const double dt);
			void DecelerateUp(const double dt);
			void DecelerateDown(const double dt);
			void DecelerateLookLeft(const double dt);
			void DecelerateLookRight(const double dt);
	/* inertia functions */
	
	// Applied methods
	virtual void setJump(const double dt);
	virtual void Pitch(const double dt);
	virtual void Yaw(const double dt);
	virtual void Roll(const double dt);
	virtual void Walk(const double dt);
	virtual void Strafe(const double dt);
	virtual void Jump(const double dt);
	virtual void Fly(const double dt);
	void croutch(const double dt);
};


#endif