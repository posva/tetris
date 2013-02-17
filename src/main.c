#include "ncWin.h"

int main (int argc, const char * argv[])
{
	ncWin prog;
        ncInit(&prog);
	ncLoop(&prog);
        ncFree(&prog);
	
        return EXIT_SUCCESS;
}
