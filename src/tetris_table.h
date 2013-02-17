#ifndef _TETRIS_TABLE_INCLUDE_
#define _TETRIS_TABLE_INCLUDE_
#include "block.h"
#include <time.h>

typedef struct {
	block_type type;
	char empty;
	//TableCell(uint16_t _type = 0, bool _empty = true) : type(_type), empty(_empty) {}
} table_cell;

void tcDefault(table_cell* t);

typedef enum {
	dir_none,
	dir_left,
	dir_right
	}direction ;

typedef struct {
	direction rot, mov;
	char force, doStep;
	
	//Control(Rotation _rot = None, Rotation _mov = None, bool _force = false, bool _doStep = false) : rot(_rot), mov(_mov), force(_force), doStep(_doStep) {}
} control;

void cnDefault(control* c);

typedef struct {
    position currentPos, size;
    table_cell **tab;
    block* next_block[2];

} tetris_table;

void ttInit(tetris_table* t, uint16_t w, uint16_t h);

void ttFree(tetris_table* t);

char ttStep(tetris_table* t, const control* c, uint32_t *points_var);

void ttRestart(tetris_table* t);

inline position ttGetSize(const tetris_table* t) { return t->size; }

inline const table_cell* ttGetTableCell(const tetris_table* t, uint16_t x, uint16_t y) { return &t->tab[y][x]; }

inline const block* ttGetCurrentBlock(const tetris_table* t) { return t->next_block[0]; }

inline const block* ttGetNextBlock(const tetris_table* t) { return t->next_block[1]; }

inline position ttGetCurrentPosition(const tetris_table* t) { return t->currentPos; }
#endif

