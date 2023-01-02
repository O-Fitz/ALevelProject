#include "Application.h"
#include <iostream>



int main(int, char**)
{
	
	Application application = Application();

	if (application.isSetup()) {
		application.run();
	}
	else {
		std::cout << "Exit Program: Fatal Error" << std::endl;
		return 1;
	}

	return 0;
}

