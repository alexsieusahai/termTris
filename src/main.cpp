
#include "frontier.h" 
#include <iostream>

int main()  {
    // consider tossing all of the below into an initalize() function
    // initializes game
    Frontier io;
    io.highScore = 0;
    io.fillFrontierWithDots();
    io.setAllDead();
    io.initializeGame();
    io.resetGhostGrid();

    // handles game mechanics
    while(true)	{
	    io.turn();
	    io.printFrontier();
    }
    return 0;
}

