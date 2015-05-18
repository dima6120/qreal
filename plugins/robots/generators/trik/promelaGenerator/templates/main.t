#include "defines"

/*threads channels*/
#define mainproc 0
@@CHANNELS@@
chan mainchan = [0] of {int, message};

/*const strings*/
@@CONST_STRINGS@@

inline initialization()
{
@@CONST_STRINGS_INIT@@
}

@@VARIABLES@@

@@SUBPROGRAMS@@

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