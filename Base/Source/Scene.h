#ifndef SCENE_H
#define SCENE_H

enum SCENE_STATES
{
	SCENE_GAME,
	SCENE_PAUSE,
	TOTAL_SCENE,
};

class Scene
{
	bool firstCall;
public:
	Scene() {firstCall = true;}
	~Scene() {}

	virtual void Init() = 0;
	virtual void Update(double dt) = 0;
	virtual void Render() = 0;
	virtual void Exit() = 0;
	virtual void UpdateCameraStatus(const unsigned char key) = 0;

	/* getter setters */
	bool getFirstCall() {return firstCall;};
	void setFirstCall(bool b) {firstCall = b;};
};

#endif