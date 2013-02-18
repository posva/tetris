#include "tetris_table.h"


void tcDefault(table_cell* t)
{
    t->type = 0;
    t->empty = 1;
}

void cnDefault(control* c)
{
    c->rot = dir_none;
    c->mov = dir_none;
    c->force = 0;
    c->doStep = 0;
}


position ttGetSize(const tetris_table* t) { return t->size; }

const block* ttGetCurrentBlock(const tetris_table* t) { return t->next_block[0]; }

const table_cell* ttGetTableCell(const tetris_table* t, uint16_t x, uint16_t y) { return &t->tab[y][x]; }

const block* ttGetNextBlock(const tetris_table* t) { return t->next_block[1]; }

position ttGetCurrentPosition(const tetris_table* t) { return t->currentPos; }

void ttResetTable(tetris_table* t)
{
    for (uint16_t i = 0; i < t->size.y; ++i)
        for (uint16_t j = 0; j < t->size.x; ++j)
            tcDefault(&t->tab[i][j]);
}

void ttNewNextBlock(tetris_table* t)
{
    if (t->next_block[0])
    {
        blockFree(t->next_block[0]);
        free(t->next_block[0]);
    }

    t->next_block[0] = t->next_block[1];
    t->next_block[1] = (block*)malloc(sizeof(block));
    assert(t->next_block[1]);

    blockInitWithKind(t->next_block[1], (block_kind)(rand()%kind_end));
    uint16_t times = rand()%4;
    for (uint16_t i = 0; i < times; ++i)
        blockRotate(t->next_block[1], 1);

}

inline void ttResetCurrentPosition(tetris_table* t)
{
    t->currentPos.x = t->size.x/2;
    t->currentPos.y = 0;
}

inline void ttRestart(tetris_table* t)
{
    ttResetTable(t);
    ttNewNextBlock(t);
    ttNewNextBlock(t);
    ttResetCurrentPosition(t);
}

void ttInit(tetris_table* t, uint16_t w, uint16_t h)
{
    t->tab = (table_cell**)malloc(sizeof(table_cell*)*h);
    assert(t->tab);
    for (uint16_t i = 0; i < h; ++i)
    {
        t->tab[i] = (table_cell*)malloc(sizeof(table_cell)*w);
        assert(t->tab[i]);
    }

    t->size.x = w;
    t->size.y = h;

    t->next_block[0] = NULL;
    t->next_block[1] = NULL;

    ttResetTable(t);
}

void ttFree(tetris_table* t)
{
    for (uint16_t i = 0; i < t->size.y; ++i)
       free(t->tab[i]);
    free(t->tab);

    if (t->next_block[0])
    {
        blockFree(t->next_block[0]);
        free(t->next_block[0]);
    }

    if (t->next_block[1])
    {
        blockFree(t->next_block[1]);
        free(t->next_block[1]);
    }
}

void ttFillWithCurrentBlock(tetris_table* t)
{
    block* b = t->next_block[0];
    for (uint16_t s = 0; s < blockSize(b); ++s)
    {
        position p = blockPosition(b, s);
        p.x += t->currentPos.x;
        p.y += t->currentPos.y;

        if (p.x < t->size.x && p.y < t->size.y && p.x >= 0 && p.y >= 0)
        {
            t->tab[p.y][p.x].type = blockType(b);
            t->tab[p.y][p.x].empty = 0;
        }
    }
}

char ttCanMoveDown(tetris_table* t)
{
    block* b = t->next_block[0];
    for (uint16_t s = 0; s < blockSize(b); ++s)
    {
        position p = blockPosition(b, s);
        p.x += t->currentPos.x;
        p.y += t->currentPos.y + 1;

        if (p.x < t->size.x && p.y >= 0 && p.x >= 0)
            if (p.y >= t->size.y || !t->tab[p.y][p.x].empty)
                return 0;
    }
    return 1;
}

char ttCanMove(tetris_table* t, char right)
{
    block* b = t->next_block[0];
    for (uint16_t s = 0; s < blockSize(b); ++s)
    {
        position p = blockPosition(b, s);
        p.x += t->currentPos.x + ((right)?1:-1);
        p.y += t->currentPos.y;

        if (p.y < t->size.y && p.y >= 0)
            if (p.x >= t->size.x || p.x < 0 || !t->tab[p.y][p.x].empty)
                return 0;
    }
    return 1;
}

char ttCanTurn(tetris_table* t, char right)
{
    block b;
    blockInitWithBlock(&b, t->next_block[0]);
    blockRotate(&b, right);

    for (uint16_t i = 0; i < blockSize(&b); ++i)
    {
        position p = blockPosition(&b, i);
        p.x += t->currentPos.x;
        p.y += t->currentPos.y;

        if (p.x < 0 || p.y >= t->size.y || p.x >= t->size.x || (p.y >= 0 && !t->tab[p.y][p.x].empty))
        {
            blockFree(&b);
            return 0;
        }
    }

    blockFree(&b);
    return 1;
}

uint32_t ttCleanFilledLines(tetris_table* t)
{
    uint32_t rows = 0;
    for (uint16_t y = t->size.y - 1; y < t->size.y; --y)
    {
        char full = 1;

        for (uint16_t x = 0; x < t->size.x; ++x)
            if (t->tab[y][x].empty)
            {
                full = 0;
                break;
            }

        if (full)
        {
            ++rows;

            for (uint16_t j = y; j < y+1; --j) // if y == 0, y-1 = 0xffff
                for (uint16_t x = 0; x < t->size.x; ++x)
                {
                    if (j == 0) //first row
                        t->tab[j][x].empty = 1;
                    else
                        t->tab[j][x] = t->tab[j-1][x];
                }
            ++y;
            // we need to check again the current row because we just changed it
        }
    }
    return rows;
}


char ttStep(tetris_table* t, const control* c, uint32_t *points_var)
{
    if (c->rot == dir_left && ttCanTurn(t, 0))
        blockRotate(t->next_block[0], 0);
    else if (c->rot == dir_right && ttCanTurn(t, 1))
        blockRotate(t->next_block[0], 1);

    if (c->mov == dir_left && ttCanMove(t, 0))
        --t->currentPos.x;
    else if (c->mov == dir_right && ttCanMove(t, 1))
        ++t->currentPos.x;

    if (c->doStep)
    {
        if (ttCanMoveDown(t))
            ++t->currentPos.y;
        else
        {
            ttFillWithCurrentBlock(t);
            *points_var += blockType(t->next_block[0]);
            ttNewNextBlock(t);
            ttResetCurrentPosition(t);
            uint32_t p = ttCleanFilledLines(t);
            if (p > 0)
            {
                uint32_t v = 1;
                for (uint16_t k = 0; k < p; ++k)
                    v *= 2;
                *points_var += 100*v;
            }

            if (!t->tab[0][t->currentPos.x].empty)
                return 1;
        }
    }
    return 0;
}

