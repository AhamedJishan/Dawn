#include <iostream>

#include "Core/Application.h"

int main(int argc, char* argv)
{
	Dawn::Application application;
	
	if (application.Init(1280, 720))
	{
		// TODO: Push layers to app
		application.Run();
	}
	return 0;
}