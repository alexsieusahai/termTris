#include <iostream>
#include <ncurses.h> // for handling input, arduino.h will provide similar functionality (I'm making sure of that)

#include "frontier.h"

using namespace std;

void Frontier::printFrontier()	{
	// NOTE TO MAX
	// Notice how the coordinate grid of characters is a 12 row by 10 column grid, as you specified
	// If it's alive, draw the grid as some color discriminating on if they are alive A's, dead A's, or just empty dots ('.')
	// Eventually we can make blocks hold a block structure, which would contain its color (and we should refractor so it holds the alive attribute as well)
	
	// print line by line
	for (int j = 0; j < HEIGHT; ++j)	{ 
		for (int i = 0; i < WIDTH; ++i)	{
			cout << blocks[i][j];
		}
		cout << '\n'; // after printing out one line, print out a new line
	}
	cout << '\n'; // so the frontiers printed one after the other have a line between them
	cout << flush;
}

void Frontier::fillFrontierWithDots()	{
	// this fills the entire frontier with just dots
	for (int i = 0; i < WIDTH; ++i)	{
		for (int j = 0; j < HEIGHT; ++j)	{
			blocks[i][j] = '.';
		}
	}
}

void Frontier::setAllDead()	{
	// this function sets all blocks in the blocks[][] array to the state dead (! alive)
	// note that whenever i'd call this function, i'd want to prep a spawn for another tetronimo
	for (int i = 0; i < WIDTH; ++i)	{
		for (int j = 0; j < HEIGHT; ++j)	{
			isAlive[i][j] = false;
		}
	}
	shouldSpawn = true;
}
	
void Frontier::spawnBlock()	{
	// first, spawn a block by dropping some character onto the top of the board
	// TODO:
	// implement some random function to drop it randomly
	// after the above is implemented, make sure to GAME OVER if the spot where we spawned the tetronimo is covered
	blocks[WIDTH/2][0] = 'A';
	isAlive[WIDTH/2][0] = true;
}

void Frontier::move(int a, int b, int c, int d)	{
	// moves one ALIVE block to a DEAD space (if it doesn't do this, game logic breaks)
	
	// note that I will have to check for collision before moving!
	
	// first, switch the block characters
	char tempChar = blocks[c][d];
	blocks[c][d] = blocks[a][b];
	blocks[a][b] = tempChar;
	
	// now, switch the alive states
	bool tempBool = isAlive[c][d]; 
	isAlive[c][d] = isAlive[a][b];
	isAlive[a][b] = tempBool;
}

void Frontier::drop()	{
	// drop all blocks 1 step below if they are alive
	//
	// NOTE:
	// There's an error with this logic for tetronimos that aren't one square; I'd end up moving some parts of the tetronimo down
	// I'd have to fix this...
	//
	for (int i = 0; i < WIDTH; ++i)	{
		for (int j = HEIGHT-1; j >= 0; --j)	{
			if (isAlive[i][j])	{ // only drop a block if it's considered to be alive
				// alright, so if it's alive i need to decide whether to continue to drop it or to stop it from moving
				// first check to see if I'm going to be out of bounds on the next move
				if (j != HEIGHT-1)	{ // if it's sitting on the very bottom, which means i shouldn't be able to continue moving

					// okay so I know it's not at the bottom, but I have to account for collision with other blocks now
					if (blocks[i][j+1] != '.')	{ // if it's not empty
						//cout << "I'm going to collide if I move down, so I'm just gonna stop...\n";
						setAllDead(); // kill all tetronimos
					} else	{
						//cout << "i'm dropping " << i << ' ' << j << '\n';
						move(i,j,i,j+1);
					}

				} else	{
					//cout << "I can't move down!\n";
					setAllDead();  // if our tetronimo can't move farther for any block, then just set all to dead
				}
			}
		}
	}
}

void Frontier::deleteAndShiftLine(int j)	{
	// this function will delete everything on line j then shift down
	
	// first, lets handle the deletion
	for (int i = 0; i < WIDTH; ++i)	{
		blocks[i][j] = '.';
	}

	// now, lets shift all down
	for (int newJ = j; newJ > 0; --newJ)	{
		// grab the line above it
		for (int i = 0; i < WIDTH; ++i)	{
			blocks[i][newJ] = blocks[i][newJ-1];
		}
	}
}

void Frontier::cleanLines()	{ 
	// this function will go through every line, and if a line is full of DEAD A's then 
	// it will delete the line using deleteLine(int)
	for (int j = 0; j < HEIGHT; ++j)	{
		int counter = 0;
		for (int i = 0; i < WIDTH; ++i)	{
			if (!isAlive[i][j] && blocks[i][j] == 'A')	{
				counter++;
			}
		}
		if (counter == WIDTH)	{
			deleteAndShiftLine(j);
		}
	}
}

void Frontier::fillLine()	{ 
	// a function used to speed up testing time
	// it will fill the farthest line downwards with a bunch of dead A's
	for (int i = 0; i < WIDTH-1; ++i)	{
		blocks[i][HEIGHT-1] = 'A';
	}
}
	

void Frontier::turn()	{
	// this handles what to do every single "turn"
	// I am going to define a turn as what is done between every single dt
	
	// Not the best code, but it will work; clean it up later!
	// This does not perform how tetris normally does; replay tetris then copy mechanics to polish
	
	
	// lets handle any input
	// lets use ncurses for this for the terminal implementation

	// NOTE TO MAX:
	// 	I will write what's going on with the ncurses implementation, just take that functionality and move it to Arduino using arduino libraries
	
	initscr(); // start curses mode
	noecho(); // I don't want the user to see what he's inputting!
	halfdelay(TIME_TO_WAIT); // this sets how long getch() will wait for something to appear on istream before it just returns -1 (fail state)
	char currentMove = getch(); // this will grab a character on istream at this moment if there's anything to grab
	endwin();  // end curses mode

	if (currentMove == 'a')	{ 
		//cout << "I'm moving to the left!\n";
		for (int i = 0; i < WIDTH; ++i)	{
			for (int j = HEIGHT-1; j >= 0; --j)	{ // this way I will never propagate a movement unintended
				if (isAlive[i][j] && i > 0 && blocks[i-1][j] == '.')	{
					move(i,j,i-1,j);
				}
			}
		}
	}

	if (currentMove == 'd')	{
		//cout << "I'm moving to the right!\n";
		for (int i = WIDTH-1; i >= 0; --i)	{
			for (int j = HEIGHT-1; j >= 0; --j)	{
				if (isAlive[i][j] && i < WIDTH-1 && blocks[i+1][j] == '.')	{
					move(i,j,i+1,j);
				}
			}
		}
	}

	// TODO:
	// Implement a fast fall using W

	// lets handle the drop down of all alive blocks
	drop();

	// finally, lets handle the case where I need to clean up and shift the lines down
	// this isn't the best solution but it will work
	for (int i = 0; i < HEIGHT-1; ++i)	{ // we will have to clean up at MOST HEIGHT-1 lines
		cleanLines();  // i should test to see 
	}

	// lets see if we need to spawn a block, and if we do lets spawn it for the next iteration of this function
	if (shouldSpawn)	{
		spawnBlock();
		shouldSpawn = false;
	}
}
