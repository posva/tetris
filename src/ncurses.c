//
//  ncurses.cpp
//  Tetris
//
//  Created by Edu San Martin Morote on 09/02/13.
//  Copyright 2013 Posva Games. All rights reserved.
//

#include "ncurses.hpp"
#include <cstring>
#define BACK_COLOR COLOR_BLACK
#define FPS 10
#define DEFAULT_PAIR 24
#define TABX 10
#define TABY 20


ncurses::ncurses() : m_tab(TABX, TABY), m_table(NULL), m_score(NULL), m_next(NULL), m_tableWinConfig(5, 2, TABX+2, TABY+2), m_scoreWinConfig(5+TABX+6, 2, 20, 5), m_nextWinConfig(5+TABX+6, 2+1+m_scoreWinConfig.h, 20, 10), m_points(0), m_showPoints(0.f)
{
	initscr();
	start_color();
	
	raw();
	noecho();
	cbreak();
	start_color();
	init_pair(DEFAULT_PAIR, COLOR_WHITE, BACK_COLOR);
	keypad(stdscr, TRUE);
	
	srand(static_cast<unsigned int>(time(NULL)));
	
	init_color(COLOR_RED, 100, 800, 100);
	
	init_pair(1, BACK_COLOR, COLOR_RED);
	init_pair(2, BACK_COLOR, COLOR_WHITE);
	init_pair(3, BACK_COLOR, COLOR_MAGENTA);
	init_pair(4, BACK_COLOR, COLOR_BLUE);
	init_pair(5, BACK_COLOR, COLOR_GREEN);
	init_pair(6, BACK_COLOR, COLOR_YELLOW);
	init_pair(7, BACK_COLOR, COLOR_CYAN);
	
}

ncurses::~ncurses()
{
	endwin();
}

void ncurses::updateTabWin()
{
	
	wattron(m_table, COLOR_PAIR(DEFAULT_PAIR));
	fillWithColor(m_table, m_tableWinConfig, " ");
	printTable(m_table, 1, 1);
	printBlock(m_table, m_tab.getCurrentPosition().y+1, m_tab.getCurrentPosition().x+1, m_tab.getCurrentBlock());
	
	

}

void ncurses::updateScoreWin()
{
	wattron(m_score, COLOR_PAIR(DEFAULT_PAIR));
	
	fillWithColor(m_score, m_scoreWinConfig, " ");
	
	wmove(m_score, 1, m_scoreWinConfig.w/2 - 2);
	wattron(m_score, A_BOLD);
	wprintw(m_score, "Score");
	wattroff(m_score, A_BOLD);
	char num[255];
	sprintf(num, "%u", static_cast<uint32_t>(m_showPoints));
	wmove(m_score, 3, m_scoreWinConfig.w-1-strlen(num));
	wprintw(m_score, num);

}

void ncurses::updateNextWin()
{
	wattron(m_next, COLOR_PAIR(DEFAULT_PAIR));
	
	fillWithColor(m_next, m_nextWinConfig, " ");
	
	wmove(m_next, 1, m_nextWinConfig.w/2 - 2);
	wattron(m_next, A_BOLD);
	wprintw(m_next, "Next");
	wattroff(m_next, A_BOLD);

	printBlock(m_next, m_nextWinConfig.h/2, m_nextWinConfig.w/2, m_tab.getNextBlock());
}

void ncurses::fillWithColor(WINDOW *win, const winConfig &conf, const char *c)
{
	wmove(win, 0, 0);
	for (int i = 0; i<conf.h; ++i)
		for (int j = 0; j<conf.w; ++j)
			wprintw(win, c);
	wborder(win, '|', '|', '-', '-', '+', '+', '+', '+');
}

void ncurses::loop()
{
	bool over = false, needRefresh = true;
	int ch;
	
	m_tab.restart();
	m_showPoints = 0.f;
	m_points = 0;
	
	attron(COLOR_PAIR(DEFAULT_PAIR));
	for (int i = 0; i < LINES; ++i)
		for (int j= 0; j < COLS; ++j)
			printw(" ");
	refresh();
	
	m_table = newwin(m_tableWinConfig.h, m_tableWinConfig.w, m_tableWinConfig.y, m_tableWinConfig.x);
	m_score = newwin(m_scoreWinConfig.h, m_scoreWinConfig.w, m_scoreWinConfig.y, m_scoreWinConfig.x);
	m_next = newwin(m_nextWinConfig.h, m_nextWinConfig.w, m_nextWinConfig.y, m_nextWinConfig.x);
	
	updateNextWin();
	updateScoreWin();
	updateTabWin();
	
	wrefresh(m_table);
	wrefresh(m_score);
	wrefresh(m_next);
	refresh();
	
	float doStep(0.f);
	
	while (!over)
	{
		nodelay(stdscr, true);
		for (int x=0; x<50; x++)
		{
			ch = getch();
			//usleep(1000/30);
			usleep((1000/FPS));
			
			
			Control control;
			
			switch (ch) {
				case 27:
					over = true;
					break;
					
				case KEY_LEFT:
					control.mov = Left;
					needRefresh = true;
					break;
					
				case KEY_RIGHT:
					control.mov = Right;
					needRefresh = true;
					break;
					
				case 'Z':
				case 'z':
					control.rot = Left;
					needRefresh = true;
					break;
				case 'X':
				case 'x':
					control.rot = Right;
					needRefresh = true;
					break;
					
				case KEY_DOWN:
					control.doStep = true;
					needRefresh = true;
					break;
					
				default:
					break;
			}
			
			doStep += 0.1f;
			
			if (doStep > 500.f)
			{
				doStep = 0.f;
				control.doStep = true;
				needRefresh = true;
			}
			
			if (m_tab.step(control, m_points))
				m_tab.restart(), m_showPoints = 0.f, m_points = 0;
			
				if (m_showPoints < m_points)
				m_showPoints += 0.01f, needRefresh = true;
				
				if (needRefresh)
				{
					updateTabWin();
					updateScoreWin();
					updateNextWin();
					
					wrefresh(m_table);
					wrefresh(m_score);
					wrefresh(m_next);
					refresh();
					needRefresh = false;
				}


		}
				
	}
	
	delwin(m_table);
	delwin(m_score);
	delwin(m_next);
}


void ncurses::printBlock(WINDOW *win, int y, int x, const Block &b)
{
	
	for (uint16_t i(0); i<b.getBlockCount(); ++i)
	{
		if (y+b.getPosition(i).y <= 0) continue;
		wmove(win, y+b.getPosition(i).y, x+b.getPosition(i).x);
		wattron(win, COLOR_PAIR(b.getType()+1));
		wprintw(win, " ");
		wattroff(win, COLOR_PAIR(b.getType()+1));
		
		/*
		wmove(win, 1, 1);
		wprintw(win, "%u", b.getType()+1);
		wrefresh(win);
		refresh();
		getch();
		*/
	}
}


void ncurses::printTable(WINDOW *win, int y, int x)
{
	Position size(m_tab.getSize());
	/*
	wmove(win, 1, 1);
	wprintw(win, "%u,%u", size.x, size.y);
	wrefresh(win);
	refresh();
	getch();
	 */
	
	for (uint16_t i = 0; i <size.y; ++i)
		for (uint16_t j = 0; j < size.x; ++j)
		{
			const TableCell& c(m_tab.getCell(j, i));
			if (!c.empty)
			{
				wmove(win, y+i, x+j);
				wattron(win, COLOR_PAIR(c.type+1));
				wprintw(win, " ");
				wattroff(win, COLOR_PAIR(c.type+1));
			}
			/*
			wrefresh(win);
			refresh();
			getch();*/
			
		}
}

