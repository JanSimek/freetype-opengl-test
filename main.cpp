// SDL 2 template

#include <string>
#include <iostream>
#include <sys/sysctl.h>

#include "Game.h"

Game* g_game = 0;

int main( int argc, const char* argv[] )
{
	g_game = new Game();
	g_game->init();
	
	while( g_game->running() )
	{
		g_game->handleEvents();
		g_game->update();
		g_game->render();
	}

	g_game->clean();

    delete g_game;

    /*
	unsigned int n = 720;
	
	std::cout << "Next highest power of two from " << n;
	n--;
	n |= n >> 1;   // Divide by 2^k for consecutive doublings of k up to 32,
	n |= n >> 2;   // and then or the results.
	n |= n >> 4;
	n |= n >> 8;
	n |= n >> 16;
	n++;           // The result is a number of 1 bits equal to the number
	// of bits in the original number, plus 1. That's the
	// next highest power of 2.
	
	std::cout << " is " << n << std::endl;
    */
	return 0;
}

