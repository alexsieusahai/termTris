#ifndef FRONTIER_H

const int WIDTH = 10;
const int HEIGHT = 12;
const int TIME_TO_WAIT = 10; // units are in tenths of a second
const int NUM_BLOCKS = 5;

#include <string>

#define FRONTIER_H

class Frontier  {

    char blocks[WIDTH][HEIGHT];

public:
    // variables 
    bool shouldSpawn;
    bool isAlive[WIDTH][HEIGHT];
    int originX;
    int originY;
    int rotationState; // 0 = 0deg, 1 = 90deg, ... 


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
    void moveAllAlive(int);
    bool isAllowed(int);
    	// instructions for isAllowed and moveAllALive
	// 	Pass in 0 to check left movement
	// 	Pass 1n 1 to check right movement
	// 	Pass in 2 to check down movement
    void checkRotationAndRotateAllAlive(int);
	// instructions for above function
	// 	Pass in 0 for counterclockwise rotation
	// 	Pass in 1 for clockwise rotation
};

#endif
