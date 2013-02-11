#pragma once

#include <stdint.h>
#include <assert.h>

typedef struct {
	int16_t x, y;
} position;

void posSet(position* pos, int16_t _x, int16_t _y)
{
    pos->x = _x;
    pos->y = _y;
}

typedef uint16_t block_type;

typedef enum {
	I=0,
	J=1,
	L=2,
	O=3,
	S=4,
	T=5,
	Z=6,
	kind_end=7
	
} block_kind;

typedef struct {
    position* pos;
    uint16_t pos_size;
    block_type type;
} block;

void blockInit(block* b, uint16_t size, block_type t);

void blockInitWithKind(block* b, block_kind k);

void blockFree(block* b);

inline uint16_t blockSize(const block* b) { return b->pos_size; }

inline position blockPosition(const block* b, uint16_t i) { assert(i < b->pos_size); return b->pos[i]; }


inline void blockSetPosition(const block* b, uint16_t i, position pos) { assert(i < b->pos_size); b->pos[i] = pos; }

