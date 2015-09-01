#include "Controller.h"
#include "View_Level.h"
#include "Model_Level1.h"
#include "Model_Level2.h"
#include "Model_Level3.h"
#include "Model_Level4.h"
#include "Model_Screen.h"

int main( void )
{
	/* Memory leak checker */
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );	//c all this if program does not exit at same place everytime

	/* Create Model */
	Model_Screen* mmptr;
	Model_Level1* mptr;
	Model_Level2* m2ptr;
	Model_Level3* m3ptr;
	Model_Level4* m4ptr;
	vector<Model_Level*> myModel;
	
	/* Create screens*/
	mmptr = new Model_Screen;

	/* Create level 1 */
	mptr = new Model_Level1;
	myModel.push_back(mptr);

	/* Create level 2 */
	m2ptr = new Model_Level2;
	myModel.push_back(m2ptr);

	/* Create level 3 */
	m3ptr = new Model_Level3;
	myModel.push_back(m3ptr);

	/* Create level 3 */
	m4ptr = new Model_Level4;
	myModel.push_back(m4ptr);

	/* Create View and pass in address of model you want, console dimemsions and Mode */
	View_Level myView(896, 700, View::TWO_D);

	/* Pass in View into Controller and set mode (2D/3D) */
	Controller myController(myModel, mmptr, &myView);

	/* Init, Run and Exit */
	myController.Init();
	
	myController.Run();
	myController.Exit();
}
