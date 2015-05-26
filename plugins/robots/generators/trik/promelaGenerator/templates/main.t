typedef string { int size; int a[8] };
typedef data { int type; /*string s;*/ int i };
typedef array { int size; data a[3] };
typedef message { int size; int a[2] };

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

/*@@LTL_BEGIN@@*/
/*@@LTL_END@@*/


