#ifndef VIEW_LEVEL_H
#define VIEW_LEVEL_H
#include "View.h"

//Include GLEW
#include <GL/glew.h>

//Include GLFW
#include <GLFW/glfw3.h>

/**
 For rendering of game level
 **/
class View_Level : public View
{
private:
/********************** model ptr **********************/
	Model_Level* model_level; //the current model
public:
/********************** constructor/destructor *****************************/
	View_Level();
	View_Level(unsigned short console_width, unsigned short console_height, MODE mode);
	~View_Level();

/********************** Core functions *****************************/
	virtual void SetModel(Model_Level* model_level);	//if not model_Level, set to NULL
	virtual void Init();
	virtual void Render(const float fps);
	virtual void Exit();

/**************** render ****************/
	void RenderLight();
	void RenderObject();
	void RenderCollideBox();
	void RenderInventory(Inventory* inventory);
	void RenderHUD();
	void RenderMainMenu();
	void RenderInstruction();
	void RenderWinScreen();
	void RenderLoseScreen();
	void RenderGame();
	void RenderTransition();
	void RenderTile(Mesh* mesh, bool enableLight, int tileNum);

	/* If theres TileMap */
	void RenderTileMap();
	void RenderRearMap();
};

#endif