#ifndef _BLOCK_INCLUDE_
#define _BLOCK_INCLUDE_
#include <stdint.h>


typedef struct {
        int16_t x, y;
} position;

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

void blockInitWithBlock(block* b, const block* copy);

void blockInitWithKind(block* b, block_kind k);

void blockFree(block* b);

uint16_t blockSize(const block* b);

block_type blockType(const block* b);

position blockPosition(const block* b, uint16_t i);

void blockSetPosition(const block* b, uint16_t i, position pos);

void blockRotate(block*b, char right);
#endif

