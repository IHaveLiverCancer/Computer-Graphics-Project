#include "StarterApp.h"
#include "GeometryTute.h"

#include <iostream>



int main()
{
	MyApplicationClass* myApp = new StarterApp();
	myApp->Run("Free Cheese", 1280, 720, false);
	
	return 0;
}  