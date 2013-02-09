//
//  Block.hpp
//  Tetris
//
//  Created by Edu San Martin Morote on 07/02/13.
//  Copyright 2013 Posva Games. All rights reserved.
//
#pragma once

#include <vector>
#include <stdint.h>
#include <cassert>

struct Position {
	int16_t x, y;
	Position(int16_t _x = 0, int16_t _y = 0) : x(_x), y(_y) {}
};

typedef uint16_t BlockType;

enum BlockKind {
	I=0,
	J=1,
	L=2,
	O=3,
	S=4,
	T=5,
	Z=6,
	KindEnd=7
	
	};

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