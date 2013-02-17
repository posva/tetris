#include "block.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>


position blocks[7][4] = {

	{{0, 0},
	{0, 1},
	{0, -1},
	{0, -2}
	},

	{{0, 0},
	{0, -1},
	{0, 1},
	{1, 1}
	},

	{{0, 0},
	{0, -1},
	{0, 1},
	{1, 1}
	},  

	{{0, 0},
	{1, 0},
	{1, 1},
	{0, 1}
	},

	{{0, 0},
	{0, -1},
	{1, 1},
	{1, 0}
	}, 

	{{0, 0},
	{-1, 0},
	{0, -1},
	{1, -1}
	},

	{{0, 0},
	{1, 0},
	{0, -1},
	{-1, -1}
	}
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


