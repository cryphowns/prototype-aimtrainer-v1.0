//cryphowns <Ryan> | glocation87@gmail.com
/*
* Rushed prototype for an aim trainer
* Code isn't commented properly neither is it optimized and readable, though it should suffice as a boilerplate
*/
#include <iostream>
#include "Engine.h"

int main() 
{
	int mode;

	std::cout << "Please select your desired training mode: \n[1] Tracking\n[2] Reaction" << std::endl;
	std::cin >> mode;

	Engine gameEngine(mode);

	while (gameEngine.isWindowOpen())
	{
		gameEngine.update();
		gameEngine.render();
	}

	return 0;
}