#include <iostream>
#include <ncurses.h> // for handling input, arduino.h will provide similar functionality (I'm making sure of that)
#include <cstdlib> // for rand()
#include <vector>

#include "frontier.h"

// NOTE TO WHOEVER READS THIS CODE AT THIS TIME
// I chose to be wasteful with my code in order to make porting it onto an arduino extremely easy, in the near future data structures in stl

// LIST OF BUGS:

using namespace std;

void Frontier::printFrontier()	{
	// NOTE TO MAX
	// Notice how the coordinate grid of characters is a 12 row by 10 column grid, as you specified
	// If it's alive, draw the grid as some color discriminating on if they are alive A's, dead A's, or just empty dots ('.')
	// Eventually we can make blocks hold a block structure, which would contain its color (and we should refractor so it holds the alive attribute as well)
	
	generateGhost();
	//cout << "i'm running printFrontier!\n";
	// print line by line
	for (int j = 0; j < HEIGHT; ++j)	{ 
		for (int i = 0; i < WIDTH; ++i)	{
			if (ghostGrid[i][j] && blocks[i][j] == '.')	{
				cout << '*';
			} else	{
				cout << blocks[i][j];
			}
		}
		if (j == 1)	{
			cout << "\tYOUR CURRENT SCORE IS " << score;
		}
		if (j == 2)	{
			cout << "\tYOUR HIGH SCORE IS " << highScore;
		}
		if (j == 6)	{
			cout << "\tNEXT BLOCK:";
		}
		if (j == 7)	{
			if (blockVec.back() == 0)	{
				cout << "\tSQUARE";
			}
			if (blockVec.back() == 1)	{
				cout << "\tT";
			}
			if (blockVec.back() == 2)	{
				cout << "\tLeft Pointing L";
			}
			if (blockVec.back() == 3)	{
				cout << "\tRight Pointing L";
			}
			if (blockVec.back() == 4)	{
				cout << "\tLine Block";
			}
			if (blockVec.back() == 5)	{
				cout << "\tGreen Skew";
			}
			if (blockVec.back() == 6)	{
				cout << "\tRed Skew";
			}
		}
		if (j == 9)	{
			cout << "\tSTORED BLOCK";
		}
		if (j == 10)	{
			if (storedBlock == 0)	{
				cout << "\tSQUARE";
			}
			if (storedBlock == 1)	{
				cout << "\tT";
			}
			if (storedBlock == 2)	{
				cout << "\tLeft Pointing L";
			}
			if (storedBlock == 3)	{
				cout << "\tRight Pointing L";
			}
			if (storedBlock == 4)	{
				cout << "\tLine Block";
			}
			if (storedBlock == 5)	{
				cout << "\tGreen Skew";
			}
			if (storedBlock == 6)	{
				cout << "\tRed Skew";
			}
		}
		
		cout << '\n'; // after printing out one line, print out a new line
		// show the user what the next block is going to be
	}
	cout << '\n'; // so the frontiers printed one after the other have a line between them
	cout << flush;
	resetGhostGrid();
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

void Frontier::resetGhostGrid()	{
	for (int i = 0; i < WIDTH; ++i)	{
		for (int j = 0; j < HEIGHT; ++j)	{
			ghostGrid[i][j] = false;
		}
	}
}

void Frontier::generateGhost()	{
	// first, generate a ghost where our piece is
	int counter = 0;
	for (int i = 0; i < WIDTH; ++i)	{
		for (int j = 0; j < HEIGHT; ++j)	{
			if (isAlive[i][j])	{
				ghostGrid[i][j] = true;
				//cout << "ghostGrid for " << i << ' ' << j << " is true!\n";
				counter++;
			}
		}
	}
	//cout << counter << endl;
	if (counter == 0)	{ // if none of the blocks were alive
		return; 
	}
	// now, keep dropping it down until you can't anymore
	while (true)	{
		for (int i = 0; i < WIDTH; ++i)	{
			for (int j = 0; j < HEIGHT; ++j)	{
				if (ghostGrid[i][j])	{
					if (j+1 >= HEIGHT)	{
						return;
					} else	{
						if (!isAlive[i][j+1])	{
							if (blocks[i][j+1] != '.')	{
								return;
							}
						}
					}
				}
			}
		}
		vector<int> xToFill;
		vector<int> yToFill;
		for (int i = 0; i < WIDTH; ++i)	{
			for (int j = 0; j < HEIGHT; ++j)	{
				if (ghostGrid[i][j])	{
					ghostGrid[i][j] = false;
					xToFill.push_back(i);
					yToFill.push_back(j+1);
				}
			}
		}

		while (!xToFill.empty())	{
			int x = xToFill.back();
			xToFill.pop_back();
			int y = yToFill.back();
			yToFill.pop_back();
			ghostGrid[x][y] = true;
		}
	}
}

void Frontier::moveToGhost()	{
	generateGhost();
	cout << "got here!\n";
	char temp;
	vector<int> newX,newY;
	for (int i = 0; i < WIDTH; ++i)	{
		for (int j = 0; j < HEIGHT; ++j)	{
			if (isAlive[i][j])	{
				temp = blocks[i][j];
				isAlive[i][j] = false;
				blocks[i][j] = '.';
			}
			if (ghostGrid[i][j])	{
				newX.push_back(i);
				newY.push_back(j);
			}
		}
	}
	resetGhostGrid();
	while (!newX.empty())	{
		int x = newX.back();
		newX.pop_back();
		int y = newY.back();
		newY.pop_back();
		blocks[x][y] = temp;
	}
	shouldSpawn = true;
}

bool Frontier::isInBounds(int i, int j) {
    //cout << "this is what was passed to isInBounds " << i << ' ' << j << endl;
    if (i >= 0 && i < WIDTH && j < HEIGHT && j > 0) {
        //cout << "what you're trying to do is fine!\n";
    } else  {
        //cout << "what you're trying to do is out of bounds!\n";
    }
    return (i >= 0 && i < WIDTH && j < HEIGHT && j >= 0);
}

void Frontier::checkRotationAndRotateAllAlive()	{
	// checks all blocks that are alive, and checks to see if rotation is valid
	// if it is valid, then the blocks are rotated
	// let 0 be counterclockwise
	// let 1 be clockwise
    if (whichBlock == 0)	{ // it's a cube
	    return; // don't rotate it at all
    }
    //cout << "my origin coordinates are " << originX << ' ' << originY << endl;
    bool shouldRotate = true; // assume it's a valid move, then search for a counterexample if false
	bool breakLoop = false;
	char temp = 'A'; // replace it later once we find one block that is alive

	// lets search all blocks to see if we can find a counterexample
	for (int i = 0; i < WIDTH; ++i)	{
		if (breakLoop) break; // is this one liner fine?
		for (int j = 0; j < HEIGHT; ++j)	{
			if (isAlive[i][j])	{
				// now check the rotation
				// first check to see if the rotation is even in bounds
				temp = blocks[i][j];
				if (!isInBounds(originX+(j-originY), originY-(i-originX)))	{
					// if it's out of bounds, then set shouldRotate to false
					shouldRotate = false;
					//cout << "set shouldRotate to false because it was out of bounds!\n";
					breakLoop = true;
					break;
				} else	{
					// if it's in bounds, check to see if the rotation spot desired is covered
					if (blocks[originX+(j-originY)][originY-(i-originX)] != '.' && !isAlive[originX+(j-originY)][originY-(i-originX)])	{
						// if the spot is covered, don't allow the rotation
						//cout << "set shouldRotate to false because the desired block was occupied!\n";
						shouldRotate = false;
						breakLoop = true;
						//cout << "desired block is occupied!\n" <<
							//"the block there is " << blocks[originX+(j-originY)][originY-(i-originX)] << '\n' << 
							//"the block coordinates are " << 
							//originX+(j-originY) << ' ' <<
							//originY-(i-originX) << '\n';
						break;
					}
				}
			}
		}
	}	
	// now, assuming that the rotation is true:
	if (shouldRotate)	{
			vector<int> xToUnpack;
			vector<int> yToUnpack;
			for (int i = 0; i < WIDTH; ++i)	{
				for (int j = 0; j < HEIGHT; ++j)	{
					if (isAlive[i][j])  {
						blocks[i][j] = '.'; // set the old position to empty
						isAlive[i][j] = false; // kill the old block
						//cout << "I placed the block " << i << ' ' << j <<
							//" here " << originX+(j-originY) << ' '
							//<< originY-(i-originX) << '\n';
						
						// fill a vector with the coordinates then unpack it later
						xToUnpack.push_back(i);
						yToUnpack.push_back(j);
					}
				}
			}
		while (!xToUnpack.empty())  { // note xToUnpack and yToUnpack are the same size
			int  i = xToUnpack.back();
			xToUnpack.pop_back();
			int j = yToUnpack.back();
			yToUnpack.pop_back();
			blocks[originX+(j-originY)][originY-(i-originX)] = temp; // create new block
			isAlive[originX+(j-originY)][originY-(i-originX)] = true; // make it alive
		}
	}
}

void Frontier::initializeGame()	{ // handles initialization of game
	score = 0;
	blockVec.push_back(rand()%NUM_BLOCKS);
	score = 0;
	storedBlock = -1; // no block stored
	paused = false;
	swapAvailable = true;
	whichBlock = rand()%NUM_BLOCKS;
	// print the game instructions then wait for the user to input enter once he's read the instructions
	cout << "Welcome to Terminal Tetris!\nMade by ItsPax.\nUse WASD To move, and press Q to rotate the blocks. Press P to pause, and R to restart.\nPlease press enter to start the game!\n";;
	cin.get(); // effectively waits for the player to press enter
	// NOTE TO MAX 
	// 	Just forget about this function, ignore it entirely
}

	
void Frontier::spawnBlock()	{
	// first, spawn a block by dropping some character onto the top of the board
	// TODO:
	// after the above is implemented, make sure to GAME OVER if the spot where we spawned the tetronimo is covered
	// spawn some random kind of block (have to implement a hasMoved array and check it otherwise move will not work)
	if (whichBlock == -1)	{
		whichBlock = blockVec.back();
		blockVec.pop_back();
		blockVec.push_back(rand()%NUM_BLOCKS);
		swapAvailable = true;
	}
	// NOTE TO MAX:
	// 	Ignore the above entirely and just set whichBlock = rand()%NUM_BLOCKS

	// now put the raw implementation of the blocks here
	// NOTE: Switch to normal tetris implementation of blocks
	currentBlock = whichBlock;
	if (whichBlock == 0)	{ //square block
		int spawnX = 5;
		blocks[spawnX][0] = 'Y';
		blocks[spawnX][1] = 'Y';
		blocks[spawnX+1][0] = 'Y';
		blocks[spawnX+1][1] = 'Y';
		isAlive[spawnX][0] = true;
		isAlive[spawnX][1] = true;
		isAlive[spawnX+1][0] = true;
		isAlive[spawnX+1][1] = true;
		// here the origin doesn't matter since rotation doesn't change the orientation of the block 
	}
	
	if (whichBlock == 1)	{ // T shaped block
		int spawnX = 5;
		// spawn the block graphics
		blocks[spawnX-1][1] = 'P';
		blocks[spawnX][1] = 'P';
		blocks[spawnX+1][1] = 'P';
		blocks[spawnX][0] = 'P';
		// set the game logic
		isAlive[spawnX-1][1] = true;
		isAlive[spawnX][1] = true;
		isAlive[spawnX+1][1] = true;
		isAlive[spawnX][0] = true;

		// set the origin
		originX = spawnX;
		originY = 1;
	}

	if (whichBlock == 2)	{ // L shaped block (left pointing)
		//int spawnX = rand()%(WIDTH-4); // this is wrong; gives a seg fault
		int spawnX = 5;
		blocks[spawnX-2][1] = 'B';
		blocks[spawnX-1][1] = 'B';
		blocks[spawnX][1] = 'B';
		blocks[spawnX][0] = 'B';
		isAlive[spawnX-2][1] = true;
		isAlive[spawnX-1][1] = true;
		isAlive[spawnX][1] = true;
		isAlive[spawnX][0] = true;

		// set the origin
		originX = spawnX-1;
		originY = 1;
	}

	if (whichBlock == 3)	{ // L shaped block (right pointing)
		int spawnX = 5;
		blocks[spawnX-2][1] = 'O';
		blocks[spawnX-1][1] = 'O';
		blocks[spawnX][1] = 'O';
		blocks[spawnX-2][0] = 'O';
		isAlive[spawnX-2][1] = true;
		isAlive[spawnX-1][1] = true;
		isAlive[spawnX][1] = true;
		isAlive[spawnX-2][0] = true;

		//set the origin
		originX = spawnX-1;
		originY = 1;
	}

	if (whichBlock == 4)	{ // line block
		int spawnX = 5;
		blocks[spawnX-3][0] = 'C';
		blocks[spawnX-2][0] = 'C';
		blocks[spawnX-1][0] = 'C';
		blocks[spawnX][0] = 'C';
		isAlive[spawnX-3][0] = true;
		isAlive[spawnX-2][0] = true;
		isAlive[spawnX-1][0] = true;
		isAlive[spawnX][0] = true;

		// set the origin, but this rotation is hardcoded to match SRS standard
		originX = spawnX-1;
		originY = 0; 
		// note; rotate them accordingly now
	}

	if (whichBlock == 5)	{ // green skew
		int spawnX = 5;
		blocks[spawnX-1][1] = 'G';
		blocks[spawnX][1] = 'G';
		blocks[spawnX][0] = 'G';
		blocks[spawnX+1][0] = 'G';
		isAlive[spawnX-1][1] = true;
		isAlive[spawnX][1] = true;
		isAlive[spawnX][0] = true;
		isAlive[spawnX+1][0] = true;

		// set the origin
		originX = spawnX;
		originY = 1;

	}

	if (whichBlock == 6)	{ // red skew
		int spawnX = 5;
		blocks[spawnX][0] = 'R';
		blocks[spawnX+1][0] = 'R';
		blocks[spawnX+1][1] = 'R';
		blocks[spawnX+2][1] = 'R';
		isAlive[spawnX][0] = true;
		isAlive[spawnX+1][0] = true;
		isAlive[spawnX+1][1] = true;
		isAlive[spawnX+2][1] = true;

		//set the origin
		originX = spawnX+1;
		originY = 1;

	}
}

bool Frontier::checkGameOver()	{
	for (int i = 3; i < 8; ++i)	{
		if (blocks[i][0] != '.' && !isAlive[i][0])	{
			cout << "GAME OVER!\n\n\n";
			return true;
		}
	}
	return false;
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
	// might be a breaking point later
	
	bool shouldContinue = true;
	for (int i = 0; i < WIDTH; ++i)	{
		for (int j = HEIGHT-1; j >= 0; --j)	{
			if (shouldContinue)	{
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
							// handle movement here
							if (isAllowed(2))	{ // checking if downward movement is allowed
								//cout << "I am executing moveAllAlive(2) to move all down by 2!\n";
								moveAllAlive(2);
								shouldContinue = false;
								break;
							}
						}

					} else	{
						//cout << "I can't move down!\n";
						setAllDead();  // if our tetronimo can't move farther for any block, then just set all to dead
					}
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
			if (!isAlive[i][j] && blocks[i][j] != '.')	{ // if the block isn't alive and if it isn't empty (where empty is defined as containing '.')
				counter++;
			}
		}
		if (counter == WIDTH)	{
			score++;
			if (score > highScore)	{
				highScore = score;
			}
			deleteAndShiftLine(j);
		}
	}
}

bool Frontier::isAllowed(int move)	{
	// this is a bandaid fix
	// for every single item  in the list, check if the movement is possible
	// if it's possible FOR EVERY SINGLE BLOCK, then return true
	// otherwise return false
	//
	//BUGS:
	// I don't think this works
	
	for (int i = 0; i < WIDTH; ++i)	{
		for (int j = 0; j < HEIGHT; ++j)	{
			if (isAlive[i][j])	{
				if (move == 0)	{ // left movement
					//cout << "isAllowed is checking left movement!\n";
					if (!isAlive[i-1][j] && i > 0)	{ // if the block we're checking isn't part of the tetronimo, so we'll just end up checking the relevant edges
						if (blocks[i-1][j] != '.')	{
							//cout << "out of bounds!\n";
							return false;
						}
					} 
					if (i - 1 < 0)	{
						return false;
					}
				} else if (move == 1)	{ // right movement
					if (!isAlive[i+1][j] && i < WIDTH-1)	{
						if (blocks[i+1][j] != '.')	{
							//cout << "out of bounds!\n";
							return false;
						}
					}
					if (i + 1 > WIDTH-1)	{
						//cout << "out of bounds; attempting right movement!\n";
						return false;
					}
				} else if (move == 2)	{// downwards movement
					if (!isAlive[i][j+1])	{
						if (blocks[i][j+1] != '.')	{
							//cout << "out of bounds! failed on the downward movement!\n";
							// if the downwards movement isn't allowed, kill all blocks
							setAllDead();
							return false;
						}
					}
				}
			}
		}
	}
	//cout << "this movement appears to be allowed!\n";
	return true;
}

void Frontier::moveAllAlive(int direction)	{
	// direction mapping
	// direction = 0 => left
	// direction = 1 => right
	// direction = 2 => down
	
	// this function goes through the entire grid, then moves all alive blocks to the specified direction
	// I am assuming that this is allowed; I have checked it before with isAllowed(direction)
	
	// have to walk through the array different ways depending on which direction i'm going
	if (direction == 0)	{ // left movement
		for (int i = 0; i < WIDTH; ++i)	{
			for (int j = HEIGHT-1; j >= 0; --j)	{ // this way I will never propagate a movement unintended
				if (isAlive[i][j])	{
					move(i,j,i-1,j);
				}
			}
		}
		// shift the origin
		originX -= 1;
	}
	if (direction == 1)	{ // right movement
		for (int i = WIDTH-1; i >= 0; --i)	{
			for (int j = HEIGHT-1; j >= 0; --j)	{
				if (isAlive[i][j])	{
					move(i,j,i+1,j); //move to the right
				}
			}
		}
		//shift the origin 
		originX += 1;
	}
	if (direction == 2)	{
		for (int i = 0; i < WIDTH; ++i)	{
			for (int j = HEIGHT-1; j >= 0; --j)	{
				if (isAlive[i][j])	{
					move(i,j,i,j+1); //move up
				}
			}
		}
		// shift the origin
		originY += 1;
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
	// 	Don't worry about any of the features like pause, restart etc
	// 	***** JUST IMPLEMENT MOVEMENT AND HARD DROP ALLOWANCE USING THE JOYSTICK
	
	initscr(); // start curses mode
	noecho(); // I don't want the user to see what he's inputting!
	halfdelay(TIME_TO_WAIT); // this sets how long getch() will wait for something to appear on istream before it just returns -1 (fail state)
	char currentMove = getch(); // this will grab a character on istream at this moment if there's anything to grab
	//cout << "getch() caught " << int(currentMove) << ' ' << currentMove << endl;
	endwin();  // end curses mode

	// before anything, lets check to see if the game is paused
	if (paused)	{
		cout << "The game is paused!\n";
		if (currentMove == 'p')	{
			paused = !paused;
			cout << "Unpausing the game!\n";
		}
		return;
	}

	if (currentMove == 'p')	{
		paused = !paused;
		cout << "Pausing the game!\n";
	}

	if (currentMove == 'e' && swapAvailable)	{ //the player has issued a command to swap
		// check if a swap is allowed according to rules
		// sweep through the board and find any alive blocks, then kill and empty them, effectively despawning the piece
		int temp = storedBlock;
		storedBlock = currentBlock;
		swapAvailable = false;
		if (temp != -1)	{
			whichBlock = temp;  
		}
		shouldSpawn = true;
		for (int i = 0; i < WIDTH; ++i)	{
			for (int j = 0; j < HEIGHT; ++j)	{
				if (isAlive[i][j])	{
					blocks[i][j] = '.';
					isAlive[i][j] = false;
				}
			}
		}
		return;
	}


	
	// implementing it more true to tetris
	if (currentMove == 'a')	{  // if i want to move to the left
		//cout << "I'm moving to the left!\n";
		bool shouldContinue = true;
		for (int i = 0; i < WIDTH; ++i)	{
			for (int j = HEIGHT-1; j >= 0; --j)	{ // this way I will never propagate a movement unintended
				if (isAllowed(0))	{ // BAD: BANDAGE FIX TO PROBLEM, ADDS UNECESSARY SLOWDOWN
					// please refer to isAllowed documentation on header file for instructions on using isAllowed(int)A
					// even better, please fix later
					if (isAlive[i][j] && i > 0 && blocks[i-1][j] == '.')	{
						moveAllAlive(0);
						break;
					}
				} else	{
					shouldContinue = false;
				}
			}
		}
		if (shouldContinue)	{
			return; //feels more like tetris this way, and more fair
		}
	}

	if (currentMove == 'd')	{ // if i want to move to the right
		//cout << "I'm moving to the right!\n";
		bool shouldContinue = true;
		for (int i = WIDTH-1; i >= 0; --i)	{
			for (int j = HEIGHT-1; j >= 0; --j)	{
				if (isAllowed(1))	{
					if (isAlive[i][j] && i < WIDTH-1 && blocks[i+1][j] == '.')	{
						moveAllAlive(1);
						break;
					}
				} else	{
					shouldContinue = false;
				}
			}
		}
		if (shouldContinue)	{
			return; //feels more like tetris this way, and more fair
		}
	}

	if (int(currentMove) == 32)	{ // spacebar has been pressed
		moveToGhost(); // move the tetronimo to the ghost piece
		return;
	}

	// Note to max on the above two functions: 
	// 	Deprecate 'd' press, and use joystick press for straight up fastfall

	if (currentMove == 'q')	{ // a desire to shift counterclockwise
		checkRotationAndRotateAllAlive();
		return; //feels more like tetris this way, and more fair
	}


	if (currentMove == 's')	{ // implementation of fastfall
		// BUG: this causes segmentation fault sometimes I think
		// this is also a bandage solution, true fastfall has a bug where if I do it on the farthest left
		// something weird happens and I land somewhere in the middle (no clue how that happens)
		//cout << "i wanna fastfall!\n";
		if (isAllowed(2))	{
			drop();
			return; // so you only drop once
		}
	}

	if (currentMove == 'r')	{
		restart();
	}


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

	// finally check for game over
	if (checkGameOver())	{ // not conventional tetris game over but whatever
		//cout << "Initialzing game over screen...\n";
		restart();
	}
	whichBlock = -1; //if you reach the end, set whichBlock to negative 1
}

void Frontier::restart()	{ // what I'm going to do here is initialize everything that I initialized at the beginning, then it's effectively a restart
	if (score > highScore)	{
		highScore = score;
	}
	fillFrontierWithDots();
	setAllDead();
	initializeGame();
	resetGhostGrid();
}
