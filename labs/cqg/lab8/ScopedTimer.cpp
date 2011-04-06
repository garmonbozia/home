#ifndef SCOPED_TIMER_CPP
#define SCOPED_TIMER_CPP

#include <sys/time.h>
#include <stdio.h>
#include <boost/function.hpp>
#include <boost/bind.hpp>

class ScopedTimer
{
public:
    ScopedTimer ( void fun (double) )
    {
        gettimeofday( &timeStart, NULL );
        print_function = fun;
        printf( "ScopedTimer: constructed\n" );
    }

    ~ScopedTimer ( )
    {
        gettimeofday( &timeFinish, NULL );
        timeTotal = (timeFinish.tv_sec  - timeStart.tv_sec )*1000000
                  + (timeFinish.tv_usec - timeStart.tv_usec);
        boost::bind<void>( print_function, _1 )( timeTotal );	
        printf( "ScopedTimer: destructed\n" );
    }

private:
    boost::function<void( double )> print_function;
    timeval timeStart;
    timeval timeFinish;
    double timeTotal;
};

#endif
