#include <ncurses.h>
#include <string.h>

// char* someStuff(char* message)
// {
// 	char buff[100];
// 	sprintf(buff, message);
// 	return &buff;
// }

// char* goofyAhh()
// {
// 	char buff[] = "Lmao whachu doin?";
// 	return buff;
// }

int main()
{
	char mesg[]="Just a string";		/* message to be appeared on the screen */
	int row,col;				/* to store the number of rows and *
						* the number of colums of the screen */
	initscr();				/* start the curses mode */
	getmaxyx(stdscr,row,col);		/* get the number of rows and columns */
	mvprintw(row/2,(col-strlen(mesg))/2,"%s",mesg);
									/* print the message at the center of the screen */
	char buff[100];
	//sprintf(&buff, "This screen has %d rows and %d columns\n",row,col);
	//mvprintw(row-2,0,"This screen has %d rows and %d columns\n",row,col);
	int index = 0;
	// char* stufString = goofyAhh();//someStuff("Some kind of message");
	// for(int i = 0; i < 100; i++)
	// {
	// 	free(stufString[i]);
	// }
	//free(stufString);
	char tryingToOverride[] = "Goofy aah!"; 
	// while (*(stufString + index) != '\0' && index != 99) {
	// 	buff[index] = *(stufString + index);
	// 	index++;
	// }
	//buff[0] = 
	mvaddstr(row/2 + 1,0, buff);
	printw("Try resizing your window(if possible) and then run this program again");
	refresh();
	getch();
	endwin();

	return 0;
}