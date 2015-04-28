#include "defines"
#include "setProcess"

/*threads channels*/
#define mainproc 1
@@CHANNELS@@chan mainchan = [0] of {int, array};

/*const strings*/
@@CONST_STRINGS@@

inline initialization()
{
@@CONST_STRINGS_INIT@@
}

/* variables */
@@VARIABLES@@

/*subprograms*/
@@SUBPROGRAMS@@

/*threads*/
@@THREADS@@

proctype main()
{
	@@MAIN_CODE@@
}

init
{
	initialization();
	run main();
}

@@NEVER@@