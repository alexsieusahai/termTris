#ifndef FRONTIER_H

const int WIDTH = 10;
const int HEIGHT = 12;
const int TIME_TO_WAIT = 10; // units are in tenths of a second

#include <vector>

#define FRONTIER_H

class Frontier  {

    char blocks[WIDTH][HEIGHT];

public:
    // variables 
    bool shouldSpawn;
    bool isAlive[WIDTH][HEIGHT];


    //functions

    // general game logic
    void printFrontier();
    void fillFrontierWithDots();
    void spawnBlock();
    void turn();
    void drop();
    void move(int,int,int,int);
    void setAllDead();
    void cleanLines();
    void deleteAndShiftLine(int);
    void fillLine();
};

#endif
