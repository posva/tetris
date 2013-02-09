#include <iostream>
#include "Table.hpp"

int main (int argc, const char * argv[])
{
	    
	/*
	Block b(5); b.setType(0);
	
	for (uint16_t k(0); k < KindEnd; ++k)
	{
		b.setKind(static_cast<BlockKind>(k));
		
		b.print();
		b.printPositions();
		b.rotate();
		b.print();
		b.printPositions();
		
	}
	 */
	
	Table t;
	t.print();
	
	Control c;
	c.doStep = true;
	
	for (int i=0; i<20; i++)
		t.step(c);
	
	t.print();
	

	return EXIT_SUCCESS;
}
