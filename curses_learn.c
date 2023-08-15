#include <ncurses.h>

int main()
{	int ch;

	initscr();			/* Start curses mode 		*/
	raw();				/* Line buffering disabled	*/
	keypad(stdscr, TRUE);		/* We get F1, F2 etc..		*/
	noecho();			/* Don't echo() while we do getch */

    	printw("Type any character to see it in bold\n");
	ch = getch();			/* If raw() hadn't been called
					 * we have to press enter before it
					 * gets to the program 		*/
    bool fKeyPressed = false;
    for(int i = 1; i <= 12; i++)
    {
        if(ch == KEY_F(i))		/* Without keypad enabled this will */
        {
		    printw("F%i Key pressed", i);/*  not get to us either	*/
            fKeyPressed = true;
        }
    }
	
					/* Without noecho() some ugly escape
					 * charachters might have been printed
					 * on screen			*/
	if(!fKeyPressed)
	{	printw("The pressed key is ");
		attron(A_BOLD);
		printw("%c", ch);
		attroff(A_BOLD);
	}
	refresh();			/* Print it on to the real screen */
    	getch();			/* Wait for user input */
	endwin();			/* End curses mode		  */

	return 0;
}