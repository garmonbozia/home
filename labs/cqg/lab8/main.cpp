#include "stdio.h"
#include <memory>
#include "ScopedTimer.cpp"
#include "File.cpp"
//#include "shit.cpp"
#include "OHLCAggregator.cpp"
#include <boost/random.hpp>

void printTime ( const double Time )
{
    printf( "Total scope_life time= %0.2f usecs\n", Time );
}

void printTime2File ( const double Time )
{
    char msg [100];
    sprintf( msg, "Total scope_life time= %0.2f usecs\n", Time );
    File out_file( "times.log" );
    out_file.write( msg );
}

void printOHLC2File ( const double O,
                      const double H,
                      const double L,
                      const double C )
{
    char msg [100];
    sprintf( msg, "O= %f, H= %f, L= %f, C= %f\n", O, H, L, C );
    File out_file( "OHLC.log" );
    out_file.write( msg );
}

void task1 ( )
/// testing ScopedTimer
{
    {
        ScopedTimer timer ( printTime );
        for ( int i=0; i < 1000000; i++ ) continue;
    }
    printf( "out of scope\n\n" );
}

void task2 ( )
{
    {
        ScopedTimer timer ( printTime );
        for ( int i=0; i < 1000000; i++ ) continue;
    }
    printf( "out of scope\n\n" );
}

void task3 ( )
{
    OHLCAggregator aggregator ( printOHLC2File, 0.2 );
    for ( double i=0; i < 100000; i+=0.01 )
    {
        aggregator.push ( i );
    }
}

int main ( )
{
    task1( );
    task2( );
    task3( );
    return 0;
}

