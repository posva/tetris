#ifndef _NCWIN_INCLUDE_
#define _NCWIN_INCLUDE_
#include "tetris_table.h"
#include <ncurses.h>

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
}ncWin;

void ncInit(ncWin* nc);

void ncLoop(ncWin* nc);

void ncFree(ncWin* nc);
#endif

