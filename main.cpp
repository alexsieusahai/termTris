
#include "frontier.h" 
#include <iostream>

int main()  {
    Frontier io;
    io.shouldSpawn = true;
    io.fillFrontier();
    io.printFrontier();
    int i = 0;
    while (true)    {
        io.turn();
        io.printFrontier();
        ++i;
    }
    return 0;
}

