
#include "frontier.h" 
#include <iostream>

int main()  {
    // consider tossing all of the below into an initalize() function
    // initializes game
    Frontier io;
    io.fillFrontierWithDots();
    io.setAllDead();

    // handles game mechanics
    for (int i = 0; i < 30; ++i)	{
	    io.turn();
	    io.printFrontier();
    }
    return 0;
}

