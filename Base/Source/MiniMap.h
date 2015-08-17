#pragma once

#include "Mesh.h"

class MiniMap
{
private:
	// Rotation from first angle
	int angle;

	// Offset in the minimap
	int x;
	int y;

	// Minimap size
	int size_x;
	int size_y;

public:
	MiniMap(void);
	virtual ~MiniMap(void);

	Mesh* m_Minimap_Background;
	Mesh* m_Minimap_Border;
	Mesh* m_Minimap_Avatar;

	// Set the background mesh to this class
	bool SetBackground(Mesh* aBackground);
	// Get the background mesh to this class
	Mesh* GetBackground(void);

	// Set the border mesh to this class
	bool SetBorder(Mesh* aBorder);
	// Get the border mesh to this class
	Mesh* GetBorder(void);

	// Set the Avatar mesh to this class
	bool SetAvatar(Mesh* anAvatar);
	// Get the Avatar mesh to this class
	Mesh* GetAvatar(void);

	// Set the angle of avatar
	bool SetAngle(const int angle);
	// Get the angle of avatar
	int GetAngle(void);

	// Set the position of avatar in minimap
	bool SetPosition(const int x, const int y);
	// Get position x of avatar in minimap
	int GetPosition_x(void);
	// Get position y of avatar in minimap
	int GetPosition_y(void);

	// Set the size of minimap(calculation of avatar in minimap)
	bool SetSize(const int size_x, const int size_y);
	// Get the size of minimap(x)
	int GetSize_x(void);
	// Get the size of minimap(y)
	int GetSize_y(void);


};

