#ifndef FRONTIER_H

const int WIDTH = 10;
const int HEIGHT = 20;
const int TIME_TO_WAIT = 5; // units are in tenths of a second
const int NUM_BLOCKS = 7;

#include <vector>

#define FRONTIER_H

class Frontier  {

    char blocks[WIDTH][HEIGHT];

public:
    // variables 
    bool shouldSpawn;
    bool isAlive[WIDTH][HEIGHT];
    bool ghostGrid[WIDTH][HEIGHT]; // contains locations for where the ghost piece is, and prints it 
    int originX;
    int originY;
    int whichBlock;
    int score;
    int highScore;
    std::vector<int> blockVec;
    bool paused;
    int currentBlock;
    int storedBlock;
    bool swapAvailable;


    //functions

    // general game logic
    void printFrontier(); // prints the frontier using iostream; cout
    void fillFrontierWithDots(); // fills frontier with dots
    void spawnBlock(); // spawns a pseudo-random block using rand()
    void turn(); // the game logic for each "step" of the game
    void drop(); // drops all blocks by 1
    void move(int,int,int,int); // moves all blocks from (i,j) to (x,y)
    void setAllDead(); // sets all blocks to dead
    void cleanLines(); // goes through every line, and if it finds a line of dead blocks (not '.'s) then it deletes that entire row
    void deleteAndShiftLine(int); // deletes and shifts line
    void fillLine(); // test function; fills one line at the bottom with a certain amount of blocks
    void moveAllAlive(int); // moves all alive according to the integer
    bool isAllowed(int); // checks to see if the movement is allowed
    	// instructions for isAllowed and moveAllALive
	// 	Pass in 0 to check left movement
	// 	Pass 1n 1 to check right movement
	// 	Pass in 2 to check down movement
    void checkRotationAndRotateAllAlive();
    // clockwise rotation
    bool isInBounds(int,int); // checks if the coordinates passed in (i,j) are in bounds
    void generateGhost(); // generates the ghost piece
    void resetGhostGrid(); // resets the grid for the ghost piece
    void moveToGhost();
    void initializeGame();
    bool checkGameOver();
    void restart();
};

#endif
