//
//  ncurses.hpp
//  Tetris
//
//  Created by Edu San Martin Morote on 09/02/13.
//  Copyright 2013 Posva Games. All rights reserved.
//
#pragma once
#include "Table.hpp"
#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
#include <curses.h>
#elif defined(linux) || defined(__linux) || defined(__linux__)
#include <ncurses.h>
#endif
#include <stdint.h>

struct winConfig {
	int x, y, w, h;
	winConfig(int _x, int _y, int _w, int _h) : x(_x), y(_y), w(_w), h(_h) {}
};

class ncurses {
	Table m_tab;
	WINDOW *m_table, *m_score, *m_next;
	winConfig m_tableWinConfig, m_scoreWinConfig, m_nextWinConfig;
	uint32_t m_points;
	float m_showPoints;
	
	void fillWithColor(WINDOW* win, const winConfig& conf, const char *c);
	
	void updateTabWin();
	void updateScoreWin();
	void updateNextWin();
	
	void printBlock(WINDOW* win, int y, int x, const Block& b);
	void printTable(WINDOW* win, int y, int x);
	
public:
	
	ncurses();
	~ncurses();
	
	void loop();
	
};

