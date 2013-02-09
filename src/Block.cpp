//
//  Block.cpp
//  Tetris
//
//  Created by Edu San Martin Morote on 07/02/13.
//  Copyright 2013 Posva Games. All rights reserved.
//

#include "Block.hpp"
#include <cmath>
#include <iostream>
#include <algorithm>

Block::Block(uint16_t size) : m_pos(size), m_type(0)
{
}

Block::Block(const Block& copy) : m_pos(copy.m_pos), m_type(copy.m_type)
{
}


Block::~Block()
{
	m_pos.clear();
}

uint16_t Block::getBotHeight() const
{
	uint16_t s(0);
	for (std::vector<Position>::const_iterator it(m_pos.begin()); it != m_pos.end(); ++it)
	{
		if (it->y > s)
			s = it->y;
	}
	return s;
}

uint16_t Block::getHalfWidth() const
{
	uint16_t s(0);
	for (std::vector<Position>::const_iterator it(m_pos.begin()); it != m_pos.end(); ++it)
	{
		if (abs(it->x) > s)
			s = abs(it->x);
	}
	return s;
}

int16_t Block::getLeftWidth() const
{
	int16_t s(0xffff);
	for (std::vector<Position>::const_iterator it(m_pos.begin()); it != m_pos.end(); ++it)
	{
		if (it->x < s)
			s = it->x;
	}
	return s;
}

void Block::rotate(bool right)
{
	for (std::vector<Position>::iterator it(m_pos.begin()); it != m_pos.end(); ++it)
	{
		int16_t x = it->x;
		it->x = (!right)?it->y:-it->y;
		it->y = (!right)?-x:x;
	}
	
}

bool PosCmp(const Position& a, const Position& b);
bool PosCmp(const Position& a, const Position& b)
{
	if (a.y < b.y || (a.y == b.y && a.x < b.x))
		return true;
	else
		return false;
		
}

void Block::print() const
{
	std::vector<Position> pos(m_pos);
	std::sort(pos.begin(), pos.end(), PosCmp);
	
	int16_t lasty(1000);
	int16_t leftw(getLeftWidth());
	for (std::vector<Position>::iterator it(pos.begin()); it != pos.end(); ++it)
	{
		if (lasty != it->y)
		{
			if (lasty < it->y)
				std::cout<<std::endl;

			for (int i(0); i<abs(leftw)+it->x; ++i)
				std::cout<<" ";

			
			lasty = it->y;
		}
		std::cout<<m_type;
	}
	std::cout<<std::endl;
	
}

void Block::printPositions() const
{
	for (std::vector<Position>::const_iterator it(m_pos.begin()); it != m_pos.end(); ++it)
		std::cout<<"("<<it->x<<", "<<it->y<<")\n";
}

void Block::setKind(BlockKind kind)
{
	setBlockCount(4);
	switch (kind) {
		case I:
			setPosition(0, Position(0, 0));
			setPosition(1, Position(0, 1));
			setPosition(2, Position(0, -1));
			setPosition(3, Position(0, -2));
			m_type = 'I';
			break;
		
		case L:
			setPosition(0, Position(0, 0));
			setPosition(1, Position(0, -1));
			setPosition(2, Position(0, 1));
			setPosition(3, Position(1, 1));
			m_type = 'L';
			break;
			
		case J:
			setPosition(0, Position(0, 0));
			setPosition(1, Position(0, -1));
			setPosition(2, Position(0, 1));
			setPosition(3, Position(-1, 1));
			m_type = 'J';
			break;
			
		case O:
			setPosition(0, Position(0, 0));
			setPosition(1, Position(1, 0));
			setPosition(2, Position(1, 1));
			setPosition(3, Position(0, 1));
			m_type = 'O';
			break;
			
		case T:
			setPosition(0, Position(0, 0));
			setPosition(1, Position(0, -1));
			setPosition(2, Position(-1, 0));
			setPosition(3, Position(1, 0));
			m_type = 'T';
			break;
			
		case S:
			setPosition(0, Position(0, 0));
			setPosition(1, Position(-1, 0));
			setPosition(2, Position(0, -1));
			setPosition(3, Position(1, -1));
			m_type = 'S';
			break;
		
		case Z:
			setPosition(0, Position(0, 0));
			setPosition(1, Position(1, 0));
			setPosition(2, Position(0, -1));
			setPosition(3, Position(-1, -1));
			m_type = 'Z';
			break;
			
		default:
			setKind(O);
			break;
	}
}
