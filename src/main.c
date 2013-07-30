#include "ncWin.h"

int main(void)
{
        ncWin prog;
        ncInit(&prog);
        ncLoop(&prog);
        ncFree(&prog);

        return 0;
}

