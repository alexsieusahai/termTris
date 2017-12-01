#include <iostream>
#include <cstdlib> // for random
#include <unistd.h> // for usleep
#include <cstdio> // for getchar

#include "frontier.h"

using namespace std;

void Frontier::setAllDead()   {
    for (int i = 0; i < WIDTH; ++i) {
        for (int j = 0; j < HEIGHT; ++j)    {
            isDead[i][j] = false;
        }
    }
}

void Frontier::fillFrontier() {
    for (int j = 0; j < HEIGHT; ++j) {
        for (int i = 0; i < WIDTH; ++i) {
            blocks[i][j] = '.';
        }
    }
}

void Frontier::printFrontier()  {
    for (int j = 0; j < HEIGHT; ++j) {
        for (int i = 0; i < WIDTH; ++i) {
            cout << blocks[i][j];
        }
        cout << '\n';
    }
    cout << '\n' << flush;
}

void Frontier::spawnBlock() {
    int randomVar = rand()%WIDTH;
    blocks[randomVar][0] = 'A';
}

bool Frontier::isOccupied(int i, int j)  {
    return (blocks[i][j] != '.');
}

bool Frontier::isInBounds(int i, int j)   {
    return (j < HEIGHT && i >= 0 && i < WIDTH);
}

void Frontier::removeAndShift(int j)  {
    score++;
    for (int i = 0; i < WIDTH; ++i) {
        blocks[i][j] = '.';
    }
}

void Frontier::killLines()  {
    for (int j = HEIGHT-1; j >= 0; --j)   {
        int counter = 0;
        for (int i = 0; i < WIDTH; ++i)   {
            if (blocks[i][j] == 'A' && isDead[i][j])    {
                counter++;
            }
        }
        cout << "for the row " << j << " I got the counter " << counter << endl;
        if (counter == WIDTH) {  
            cout << "I want to delete " << j << endl;
            removeAndShift(j);
        }
    }
}

void Frontier::move(int i0, int j0, int i1, int j1) {
    if (!isOccupied(i1,j1) && isInBounds(i1,j1))    { // if it should be allowed to move given our physical logic
        blocks[i1][j1] = blocks[i0][j0];
        blocks[i0][j0] = '.';
    } 
    if (isDead[i0][j0+1])    { // if it hits below something
        isDead[i0][j0] = true; // implement a vector of coordinates that contains all coordinates that are alive, then iterate through that vector to set all to false
        shouldSpawn = true;
    }
}

void Frontier::moveLateral(char currentMove)    {
    if (currentMove == 'a') {
        for (int i = 0; i < WIDTH; ++i) {
            for (int j = 0; j < HEIGHT; ++j)    {
                if (blocks[i][j] == 'A')    {
                move(i,j,i+1,j);
                }
            }
        }
    }
    if (currentMove == 'd') {
        for (int j = HEIGHT-1; j >= 0; --j) {
            for (int i = WIDTH-1; i >= 0; --i) {
                if (blocks[i][j] == 'A')    {
                    move(i,j,i-1,j);
                }
            }
        }
    }
}

void Frontier::turn()   {
    // implement stepping 
    for (int i = 0; i < TIME_TICKS/1000; ++i)   {
        usleep(TIME_TICKS/1000);
        // how the actual flying FUCK do you stream input and grab the last fucking thing?
        currentMove = 'd';

        //cout << "passing this into moveLateral: " << currentMove << '\n';
        moveLateral(currentMove);

        //cout << "got out of the while loop\n";
    }
    if (shouldSpawn)    { // handle block spawning
        shouldSpawn = false;
        spawnBlock();
    }

    for (int j = HEIGHT-1; j >= 0; --j) {
        for (int i = WIDTH-1; i >= 0; --i) {
            if (blocks[i][j] == 'A')    {
                move(i,j,i,j+1);
            }
        }
    }
    
    // clean up lines
    killLines();
}

