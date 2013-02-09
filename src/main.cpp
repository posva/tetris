#include <iostream>
#include "Table.hpp"
#include <ncurses.h>
#include "ncurses.hpp"

int main (int argc, const char * argv[])
{
	ncurses *prog = new ncurses;
	prog->loop();
	delete prog;
	/*
	initscr();
	raw();
	noecho();
	cbreak();
	start_color();
	init_pair(1, COLOR_WHITE, COLOR_BLACK);
	init_pair(2, COLOR_BLACK, COLOR_WHITE);
	attron(COLOR_PAIR(2));
	for (int i = 0; i < LINES; ++i)
		for (int j= 0; j < COLS; ++j)
			printw(" ");
		
	refresh();
	keypad(stdscr, TRUE);
	
	WINDOW* win;
	win = newwin(22, 11, 1, 4);
	wattron(win, COLOR_PAIR(2));
	wborder(win, '|', '|', '-', '-', '+', '+', '+', '+');
	for (int i = 0; i <= 11; ++i)
		for (int j= 0; j <= 22; ++j)
			wprintw(win, " ");
	

	wborder(win, '|', '|', '-', '-', '+', '+', '+', '+');
	wmove(win, 1, 1);
	

	wprintw(win, "Hey");
	wrefresh(win);

	//
	refresh();
	
	getch();
	
	wborder(win, ' ', ' ', ' ',' ',' ',' ',' ',' ');
	wrefresh(win);
	delwin(win);
	endwin();
	
	*/
	
	/*
	Table t;
	t.print();
	
	Control c;
	c.doStep = true;
	
	for (int i=0; i<20; i++)
		t.step(c);
	
	t.print();
	*/

	return EXIT_SUCCESS;
}
