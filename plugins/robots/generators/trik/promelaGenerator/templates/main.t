#include "defines"
#include "setProcess"

#define mainproc 1
@@CHANNELS@@chan mainchan = [0] of {int, array};

@@VARIABLES@@

@@SUBPROGRAMS@@

@@THREADS@@

active proctype main()
{
	@@MAIN_CODE@@
}

@@NEVER@@