#pragma once
#include "tetris_table.h"
#include <curses.h>

typedef struct {
	int x, y, w, h;
}win_config;

inline void wcInit(win_config* wi, int x, int y, int w, int h)
{
    wi->x = x;
    wi->y = y;
    wi->w = w;
    wi->h = h;
}

typedef struct {
    tetris_table table;
    WINDOW *win_table, *win_score, *win_next;
    win_config wc_table, wc_score, wc_next;
    uint32_t points;
    float show_points;

}ncWin;

void ncInit(ncWin* nc);

void ncLoop(ncWin* nc);

void ncFree(ncWin* nc);

/*

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
*/
