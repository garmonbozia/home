#ifndef SCOPED_TIMER_CPP
#define SCOPED_TIMER_CPP

#include <sys/time.h>
#include <stdio.h>

typedef void (*pt2Func_t)(double); // pointer to function type

class ScopedTimer
{
public:
    ScopedTimer ( void fun (double) )
    {
        gettimeofday( &timeStart, NULL );
        pt2Func = fun;
        printf( "CONSTRUCTED\n" );
    }

    ~ScopedTimer ( )
    {
        gettimeofday( &timeFinish, NULL );
	    timeTotal = (timeFinish.tv_sec  - timeStart.tv_sec )*1000000
			      + (timeFinish.tv_usec - timeStart.tv_usec);
        (* pt2Func) ( timeTotal ); 
        printf( "DESTRUCTED\n" );
    }

private:
    pt2Func_t pt2Func;
    timeval timeStart;
    timeval timeFinish;
    double timeTotal;
};

#endif
