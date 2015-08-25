#include "Controller.h"
#include "View.h"
#include "Model_3D.h"
#include "Model_2D.h"

int main( void )
{
	/* Memory leak checker */
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );	//call this if program does not exit at same place everytime


	/* Create Model */
	Model_2D* myModel;
	myModel = new Model_2D;
	
	/* Create View and pass in address of model you want, console dimemsions and Mode */
	View myView(myModel, 896, 700, View::TWO_D);

	/* Pass in View into Controller and set mode (2D/3D) */
	Controller myController(&myView);

	/* Init, Run and Exit */
	myController.Init();
	
	myController.Run();
	myController.Exit();
}
