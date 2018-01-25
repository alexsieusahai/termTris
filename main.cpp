
#include "frontier.h" 
#include <iostream>
#include <fstream>

int main()  {
    // consider tossing all of the below into an initalize() function
    // initializes game
    Frontier io;
    // load highScore from fstream
    std::ifstream storedHs;
    storedHs.open("highScore.txt");
    storedHs >> io.highScore; 
    storedHs.close();
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

