#ifndef SPRITEANIMATION_H
#define SPRITEANIMATION_H
#include "Mesh.h"
#include "Vector2.h"

/*** store animation info ***/
//struct Animation
//{
//	 Animation() {}
//	void Set(int startCol, int startRow, float time) 
//	{
//		this->startRow = startRow; 
//		this->startCol = startCol;
//		this->animTime = time;
//	}
//	int startRow;	//which row to start
//	int startCol;	//which col to start
//	float animTime;	//time per cycle
//};

/*** mesh class for animation ***/
class SpriteAnimation : public Mesh
{
public:
	/* constructor/destructor */
	 SpriteAnimation(const std::string &meshName, int row, int col);
	 ~SpriteAnimation();

	/* init/update/render */
	 //oppDir: if true, going from bottom to top and right to left
	void init(float time, int startCol, int startRow, int endCol, int endRow, bool oppDir);
	void Update(double dt);
	virtual void Render();

	/* variables */
	int m_row;
	int m_col;
	int startRow;
	int endRow;
	int startCol;
	int endCol;
	bool oppDir;

	float m_frameTime;		//time per frame
	float m_currentTime;	//keep track of current time
	int m_currentRow;		//keep track of current row
	int m_currentCol;	//keep track of current col
	int m_totalFrame;		//store total frame
};

#endif