#include "Errors.h"

#include <iostream>
#include <SDL/SDL.h>

void fatalError(std::string errMsg)
{
	std::cout << errMsg << std::endl;

	SDL_Quit();
	exit(1);
}
