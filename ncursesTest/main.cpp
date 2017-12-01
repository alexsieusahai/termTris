#include <iostream>

#include <ncurses.h>

using namespace std;

int main()	{
	initscr(); // starts curses
	cbreak(); // allows user to end program with ctrl c
	noecho(); // don't echo anything user outputs
	while(true)	{
		char c = getch();
		if (c == 'a')	{
			cout << c << endl;
		} else	{
				cout << "that's not c!\n";
		}
	}
	return 0;
}
