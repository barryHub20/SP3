#include "Controller.h"
#include "View.h"
#include "Model_Level1.h"
#include "Model_Level2.h"
#include "Model_Level3.h"

int main( void )
{
	/* Memory leak checker */
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );	//call this if program does not exit at same place everytime

	/* Create Model */
	Model_Level1* mptr;
	Model_Level2* m2ptr;
	Model_Level3* m3ptr;
	vector<Model_Level*> myModel;
	
	/* Create level 1 */
	mptr = new Model_Level1;
	myModel.push_back(mptr);

	/* Create level 2 */
	m2ptr = new Model_Level2;
	myModel.push_back(m2ptr);

	/* Create level 3 */
	m3ptr = new Model_Level3;
	myModel.push_back(m3ptr);

	/* Create View and pass in address of model you want, console dimemsions and Mode */
	View myView(896, 700, View::TWO_D);

	/* Pass in View into Controller and set mode (2D/3D) */
	Controller myController(myModel, &myView);

	/* Init, Run and Exit */
	myController.Init();
	
	myController.Run();
	myController.Exit();
}
