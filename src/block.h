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

/*
class Block {
	std::vector<Position> m_pos;
	BlockType m_type; //like color or something
	
	int16_t getLeftWidth() const;
	
public:
	Block(uint16_t size = 4);
	Block(const Block& copy);
	~Block();
	
	
	inline BlockType getType() const { return m_type; }
	inline void setType(BlockType type) { m_type = type; }
	
	inline uint16_t getBlockCount() const { return m_pos.size(); }
	inline void setBlockCount(uint16_t size) { m_pos.resize(size); }
	
	inline const Position& getPosition(uint16_t i) const { assert(i < m_pos.size()); return m_pos[i]; }
	inline void setPosition(uint16_t i, const Position& pos) { assert(i < m_pos.size()); m_pos[i] = pos; }
	
	
	uint16_t getHalfWidth() const;
	uint16_t getBotHeight() const;
	
	void rotate(bool right = true);
	
	void setKind(BlockKind kind);
	
	void print() const;
	void printPositions() const;
	
};
*/
