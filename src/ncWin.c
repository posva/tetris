#include "ncWin.h"
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#define BACK_COLOR COLOR_BLACK
#define FPS 8
#define DEFAULT_PAIR 24
#define TABX 10
#define TABY 20
#define OFFY 0

void wcInit(win_config* wi, int x, int y, int w, int h)
{
        wi->x = x;
        wi->y = y;
        wi->w = w;
        wi->h = h;
}


void ncInit(ncWin* nc)
{
        ttInit(&nc->table, TABX, TABY);
        nc->points = 0;
        nc->win_table = NULL;
        nc->win_score = NULL;
        nc->win_next = NULL;
        wcInit(&nc->wc_table, 5, OFFY, TABX*2+2, TABY+OFFY);
        wcInit(&nc->wc_score,
                        nc->wc_table.x + nc->wc_table.w + 4,
                        nc->wc_table.y,
                        20,
                        5);
        wcInit(&nc->wc_next,
                        nc->wc_score.x,
                        nc->wc_score.y + nc->wc_score.h + 1,
                        20,
                        10);
        initscr();
        start_color();

        raw();
        noecho();
        cbreak();

        init_pair(DEFAULT_PAIR, COLOR_WHITE, BACK_COLOR);
        keypad(stdscr, TRUE);

        srand((unsigned int)time(NULL));

        init_color(COLOR_RED, 100, 800, 100);

        init_pair(1, BACK_COLOR, COLOR_RED);
        init_pair(2, BACK_COLOR, COLOR_WHITE);
        init_pair(3, BACK_COLOR, COLOR_MAGENTA);
        init_pair(4, BACK_COLOR, COLOR_BLUE);
        init_pair(5, BACK_COLOR, COLOR_GREEN);
        init_pair(6, BACK_COLOR, COLOR_YELLOW);
        init_pair(7, BACK_COLOR, COLOR_CYAN);


}

void ncFree(ncWin* nc)
{
        endwin();
        ttFree(&nc->table);
}

void fillWithColor(WINDOW *win, const win_config *wc, const char *c)
{
        wmove(win, 0, 0);
        for (int i = 0; i < wc->h; ++i)
                for (int j = 0; j < wc->w; ++j)
                        wprintw(win, c);
        wborder(win, '|', '|', '-', '-', '+', '+', '+', '+');
}

void ncPrintTable(ncWin* nc, int y, int x)
{
        WINDOW *win = nc->win_table;
        position size = ttGetSize(&nc->table);

        for (uint16_t i = 0; i <size.y; ++i)
                for (uint16_t j = 0; j < size.x; ++j)
                {
                        const table_cell *c = ttGetTableCell(&nc->table, j, i);
                        if (!c->empty)
                        {
                                wmove(win, y+i, x+j*2);
                                wattron(win, COLOR_PAIR(c->type + 1));
                                wprintw(win, "  ");
                                wattroff(win, COLOR_PAIR(c->type + 1));
                        }
                }
}

void winPrintBlock(WINDOW *win, int y, int x, const block *b)
{
        for (uint16_t i = 0; i < blockSize(b); ++i)
        {
                if (y + blockPosition(b, i).y <= 0) continue;
                wmove(win, y + blockPosition(b, i).y, (x + blockPosition(b, i).x)*2 -1);
                wattron(win, COLOR_PAIR(blockType(b) + 1));
                wprintw(win, "  ");
                wattroff(win, COLOR_PAIR(blockType(b) + 1));
        }
}

void ncUpdateTabWin(ncWin* nc)
{
        wattron(nc->win_table, COLOR_PAIR(DEFAULT_PAIR));
        fillWithColor(nc->win_table, &nc->wc_table, " ");
        ncPrintTable(nc, OFFY-1, 1);
        winPrintBlock(nc->win_table, ttGetCurrentPosition(&nc->table).y - OFFY-1, ttGetCurrentPosition(&nc->table).x + 1, ttGetCurrentBlock(&nc->table));
}

void ncUpdateScoreWin(ncWin* nc)
{
        wattron(nc->win_score, COLOR_PAIR(DEFAULT_PAIR));

        fillWithColor(nc->win_score, &nc->wc_score, " ");

        wmove(nc->win_score, 1, nc->wc_score.w/2 - 2);
        wattron(nc->win_score, A_BOLD);
        wprintw(nc->win_score, "Score");
        wattroff(nc->win_score, A_BOLD);
        char num[255];
        sprintf(num, "%u", nc->points);
        wmove(nc->win_score, 3, nc->wc_score.w - 1 - strlen(num));
        wprintw(nc->win_score, num);
}

void ncUpdateNextWin(ncWin* nc)
{
        wattron(nc->win_next, COLOR_PAIR(DEFAULT_PAIR));

        fillWithColor(nc->win_next, &nc->wc_next, " ");

        wmove(nc->win_next, 1, nc->wc_next.w/2 - 2);
        wattron(nc->win_next, A_BOLD);
        wprintw(nc->win_next, "Next");
        wattroff(nc->win_next, A_BOLD);

        winPrintBlock(nc->win_next, nc->wc_next.h/2, nc->wc_next.w/2 -5, ttGetNextBlock(&nc->table));
}

void ncLoop(ncWin *nc)
{
        char over = 0, needRefresh = 1;
        int ch;

        ttRestart(&nc->table);
        nc->points= 0;

        attron(COLOR_PAIR(DEFAULT_PAIR));
        for (int i = 0; i < LINES; ++i)
                for (int j= 0; j < COLS; ++j)
                        printw(" ");
        refresh();

        nc->win_table = newwin(nc->wc_table.h, nc->wc_table.w, nc->wc_table.y, nc->wc_table.x);
        nc->win_score = newwin(nc->wc_score.h, nc->wc_score.w, nc->wc_score.y, nc->wc_score.x);
        nc->win_next = newwin(nc->wc_next.h, nc->wc_next.w, nc->wc_next.y, nc->wc_next.x);

        ncUpdateNextWin(nc);
        ncUpdateScoreWin(nc);
        ncUpdateTabWin(nc);

        wrefresh(nc->win_table);
        wrefresh(nc->win_score);
        wrefresh(nc->win_next);
        refresh();

        float doStep = 0.f;

        while (!over)
        {
                nodelay(stdscr, 1);
                for (int x = 0; x < 50; ++x)
                {
                        ch = getch();
                        usleep((1000/FPS));


                        control con;
                        cnDefault(&con);
                        switch (ch) {
                                case 27:
                                case 'Q':
                                case 'q':
                                        over = 1;
                                        break;

                                case KEY_LEFT:
                                case 'H':
                                case 'h':
                                        con.mov = dir_left;
                                        needRefresh = 1;
                                        break;

                                case KEY_RIGHT:
                                case 'l':
                                case 'L':
                                        con.mov = dir_right;
                                        needRefresh = 1;
                                        break;

                                case 'Z':
                                case 'z':
                                case 'k':
                                case 'K':
                                        con.rot = dir_left;
                                        needRefresh = 1;
                                        break;
                                case 'X':
                                case 'x':
                                        con.rot = dir_right;
                                        needRefresh = 1;
                                        break;

                                case KEY_DOWN:
                                case 'j':
                                case 'J':
                                        con.doStep = 1;
                                        needRefresh = 1;
                                        break;

                                default:
                                        break;
                        }

                        doStep += 0.1f;

                        if (doStep > 500.f)
                        {
                                doStep = 0.f;
                                con.doStep = 1;
                                needRefresh = 1;
                        }

                        if (ttStep(&nc->table, &con, &nc->points))
                                ttRestart(&nc->table), nc->points = 0;

                        if (needRefresh)
                        {
                                ncUpdateTabWin(nc);
                                ncUpdateScoreWin(nc);
                                ncUpdateNextWin(nc);

                                wrefresh(nc->win_table);
                                wrefresh(nc->win_score);
                                wrefresh(nc->win_next);
                                refresh();
                                needRefresh = 0;
                        }
                }
        }

        delwin(nc->win_table);
        delwin(nc->win_score);
        delwin(nc->win_next);
}

