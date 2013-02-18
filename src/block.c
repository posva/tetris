#include "block.h"


position blocks[7][4] = {

	{{0, 0}, //I
	{0, 1},
	{0, -1},
	{0, -2}
	},

	{{0, 0}, //J
	{0, -1},
	{0, 1},
	{-1, 1}
	},

	{{0, 0}, //L
	{0, -1},
	{0, 1},
	{1, 1}
	},

	{{0, 0}, //O
	{1, 0},
	{1, -1},
	{0, -1}
	},

        {{0, 0}, //S
	{-1, 0},
	{0, -1},
	{1, -1}
	},

	{{0, 0}, //T
	{0, -1},
	{1, 0},
	{-1, 0}
	},

	{{0, 0}, //Z
	{1, 0},
	{0, -1},
	{-1, -1}
	},

};

void blockInit(block* b, uint16_t size, block_type t)
{
    b->pos = NULL;
    b->pos = (position*)malloc(sizeof(position)*size);
    assert(b->pos);
    b->pos_size = size;
    b->type = t;
}

void blockInitWithBlock(block* b, const block* copy)
{
    b->pos = NULL;
    b->pos = (position*)malloc(sizeof(position)*copy->pos_size);
    assert(b->pos);
    b->pos_size = copy->pos_size;
    for (uint16_t i = 0; i < copy->pos_size; ++i)
        b->pos[i] = copy->pos[i];
    b->type = copy->type;
}

void blockFree(block* b)
{
    free(b->pos);
    b->pos_size = 0;
    b->pos = NULL;
}

void blockInitWithKind(block* b, block_kind k)
{
    blockInit(b, 4, (block_type)k);

    if (k >= kind_end)
        blockFree(b);
    else
    {
        for (uint16_t i = 0; i < 4; ++i)
            b->pos[i] = blocks[(uint16_t)k][i];
    }

}

position blockPosition(const block* b, uint16_t i)
{
    assert(i < b->pos_size); return b->pos[i];
}


void blockSetPosition(const block* b, uint16_t i, position pos)
{
    assert(i < b->pos_size); b->pos[i] = pos;
}

void blockRotate(block*b, char right)
{
    int16_t x;
    for (uint16_t i = 0; i< b->pos_size; ++i)
    {
        x = b->pos[i].x;
        b->pos[i].x = (right)?-b->pos[i].y:b->pos[i].y;
        b->pos[i].y = (right)?x:-x;
    }
}

uint16_t blockSize(const block* b) { return b->pos_size; }

block_type blockType(const block* b) { return b->type; }

