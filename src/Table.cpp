//
//  Table.cpp
//  Tetris
//
//  Created by Edu San Martin Morote on 07/02/13.
//  Copyright 2013 Posva Games. All rights reserved.
//

#include "Table.hpp"
#include <ctime>
#include <iostream>


Table::Table(uint16_t w, uint16_t h) : m_currentPos(), m_tab(h), m_maxRand(3)
{
	initBlockList();
	
	for (std::vector<std::vector<TableCell> >::iterator it(m_tab.begin()); it != m_tab.end(); ++it)
		it->resize(w);
	m_currentPos.x = w/2;
}

void Table::restart()
{
	for (std::list<Block*>::iterator it(m_blockList.begin()); it != m_blockList.end(); ++it)
		delete *it;
	m_blockList.clear();
	
	initBlockList();
	
	for (uint16_t y(m_tab.size()-1); y < m_tab.size(); --y)
		for (uint16_t x(0); x < m_tab[0].size(); ++x)
			m_tab[y][x].empty = true;

}

Table::~Table()
{
	m_tab.clear();
	
	for (std::list<Block*>::iterator it(m_blockList.begin()); it != m_blockList.end(); ++it)
		delete *it;
	m_blockList.clear();
}

void Table::print() const
{
	std::vector<std::vector<TableCell> > tab(m_tab);
	
	for (uint16_t s(0); s < m_blockList.front()->getBlockCount(); ++s)
	{
		uint16_t	x(m_currentPos.x + m_blockList.front()->getPosition(s).x),
					y(m_currentPos.y + m_blockList.front()->getPosition(s).y);
		
		if (x < m_tab[0].size() && y < m_tab.size())
		{
			tab[y][x].type = m_blockList.front()->getType();
			tab[y][x].empty = false;
		}
	}
	
	for (std::vector<std::vector<TableCell> >::iterator it(tab.begin()); it != tab.end(); ++it)
	{
		std::cout<<"#";
		for (std::vector<TableCell>::iterator jt(it->begin()); jt != it->end(); ++jt)
		{
			if (jt->empty)
				std::cout<<" ";
			else
				std::cout<<jt->type;
		}
		std::cout<<"#\n";
	}
	
	for (uint16_t i(0); i < tab[0].size()+2; ++i)
		std::cout<<"#";
	std::cout<<std::endl;
}

void Table::initBlockList()
{
	for (uint16_t i(0); i < m_maxRand; ++i)
		generateNewBlock();
}

void Table::generateNewBlock()
{
	m_blockList.push_back(new Block());
	m_blockList.back()->setKind(static_cast<BlockKind>(rand()%KindEnd));
	uint16_t t(rand()%4);
	for (uint16_t i(0); i<t ; ++i)
		 m_blockList.back()->rotate();
}


void Table::fillWithBlock()
{
	Block* b(m_blockList.front());
	for (uint16_t s(0); s < m_blockList.front()->getBlockCount(); ++s)
	{
		uint16_t	x(m_currentPos.x + b->getPosition(s).x),
		y(m_currentPos.y + b->getPosition(s).y);
		
		if (x < m_tab[0].size() && y < m_tab.size())
		{
			m_tab[y][x].type = m_blockList.front()->getType();
			m_tab[y][x].empty = false;
		}
	}
}

void Table::removeBlock()
{
	Block* b(m_blockList.front());
	for (uint16_t s(0); s < m_blockList.front()->getBlockCount(); ++s)
	{
		uint16_t	x(m_currentPos.x + b->getPosition(s).x),
		y(m_currentPos.y + b->getPosition(s).y);
		
		if (x < m_tab[0].size() && y < m_tab.size())
			m_tab[y][x].empty = true;
	}
}

bool Table::canMoveDown()
{
	Block* b(m_blockList.front());
	for (uint16_t i(0); i < b->getBlockCount(); ++i)
	{
		uint16_t	x(m_currentPos.x + b->getPosition(i).x),
		y(m_currentPos.y + b->getPosition(i).y + 1);
		
		if (x < m_tab[0].size())
			if (y >= m_tab.size() || !m_tab[y][x].empty)
				return false;
	}
	
	return true;
}


bool Table::canMove(bool right)
{
	Block* b(m_blockList.front());
	for (uint16_t i(0); i < b->getBlockCount(); ++i)
	{
		uint16_t	x(m_currentPos.x + b->getPosition(i).x + ((right)?1:-1)),
		y(m_currentPos.y + b->getPosition(i).y);
		
		if (y < m_tab.size())
			if (x >= m_tab[0].size() || !m_tab[y][x].empty)
				return false;
	}
	
	return true;
}

bool Table::canTurn(bool rigth)
{
	Block b(*m_blockList.front());
	b.rotate(rigth);
	
	for (uint16_t i(0); i < b.getBlockCount(); ++i)
	{
		uint16_t	x(m_currentPos.x + b.getPosition(i).x),
		y(m_currentPos.y + b.getPosition(i).y);
		if (x >= m_tab[0].size() || y >= m_tab.size() || !m_tab[y][x].empty)
			return false;
	}
	
	return true;
}


bool Table::step(const Control &control)
{
	if (control.rot == Left && canTurn(false))
		m_blockList.front()->rotate(false);
	else if (control.rot == Right && canTurn(true))
		m_blockList.front()->rotate();
	
	if (control.mov == Left && canMove(false))
		--m_currentPos.x;
	else if (control.mov == Right && canMove(true))
		++m_currentPos.x;
	
	if (control.doStep)
	{
		if (canMoveDown())
			++m_currentPos.y;
		else
		{
			fillWithBlock();
			delete m_blockList.front();
			m_blockList.pop_front();
			generateNewBlock();
			m_currentPos.y = 0;
			m_currentPos.x = m_tab[0].size()/2;
			cleanFilledLines();
			
			if (!m_tab[0][m_currentPos.x].empty)
				return true;
		}
	}
	
	return false;
}

uint16_t Table::cleanFilledLines()
{
	uint16_t rowsCleaned(0);
	for (uint16_t y(m_tab.size()-1); y < m_tab.size(); --y)
	{
		bool full(true);
		for (uint16_t x(0); x < m_tab[0].size(); ++x)
		{
			if (m_tab[y][x].empty)
			{
				full = false;
				break;
			}
		}
		
		if (full)
		{
			++rowsCleaned;
			
			for (uint16_t j(y); j < y+1; --j)
				for (uint16_t x(0); x < m_tab[0].size(); ++x)
				{
					if (j == 0) //first row
						m_tab[j][x].empty = true;
					else
						m_tab[j][x] = m_tab[j-1][x];
				}
			
			//One row is cleaned so we need to recheck the current row
			++y;
		}
	}
	return rowsCleaned;
}