#ifndef FRONTIER_H

const int WIDTH = 10;
const int HEIGHT = 12;
const int TIME_TICKS = 100000;

#define FRONTIER_H

class Frontier  {
    char blocks[WIDTH][HEIGHT];
public:
    // variables 
    bool shouldSpawn;
    bool isDead[WIDTH][HEIGHT];
    char currentMove;
    int score;

    //functions

    //main game logic
    void turn();
    void printFrontier();
    void fillFrontier();
    void setAllDead();

    // game mechanics
    void spawnBlock();
    void move(int a,int b, int c, int d);
    void moveLateral(char currentMove);
    void removeAndShift(int j);
    void killLines();
    bool isOccupied(int i, int j);
    bool isInBounds(int,int);

};

#endif
