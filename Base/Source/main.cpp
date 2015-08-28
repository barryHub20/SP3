#include "Controller.h"
#include "View.h"
#include "Model_2D.h"

int main( void )
{
	/* Memory leak checker */
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );	//call this if program does not exit at same place everytime

	/* Create Model */
	Model_2D* mptr;
	vector<Model_Level*> myModel;
	
	/* Create level 1 */
	mptr = new Model_2D;
	myModel.push_back(mptr);

	/* Create level 2 */
	mptr = new Model_2D;
	myModel.push_back(mptr);

	///* Create level 3 */
	//mptr = new Model_2D;
	//myModel.push_back(mptr);
	//
	/* Create View and pass in address of model you want, console dimemsions and Mode */
	View myView(896, 700, View::TWO_D);

	/* Pass in View into Controller and set mode (2D/3D) */
	Controller myController(myModel, &myView);

	/* Init, Run and Exit */
	myController.Init();
	
	myController.Run();
	myController.Exit();
}
