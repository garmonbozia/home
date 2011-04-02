#include "stdio.h"
#include <memory>
#include "ScopedTimer.cpp"
#include "mIStream.cpp"

void printTime ( const double Time )
{
    printf( "Total scope_life time= %0.2f usecs\n", Time );
}

void printTime2File ( const double Time)
{
    char msg [100];
    sprintf( msg, "Total scope_life time= %0.2f usecs\n", Time );
    mIStream out_file( "times.log" );
    out_file.write( msg );
}

int main ( )
{
/// task1 (testing ScopedTimer)
    {
        std::auto_ptr<ScopedTimer> timer( new ScopedTimer ( printTime ) );
        for ( int i=0; i < 1000000; i++ ) continue;
    }
    printf( "out of scope\n\n" );

/// task2 (testing mIStream)
    {
        mIStream out_file( "output" );
        out_file.write( "test message" );
    }
    printf( "out of scope\n\n" );

/// task3 
    {
        std::auto_ptr<ScopedTimer> timer( new ScopedTimer ( printTime2File ) );
        for ( int i=0; i < 1000000; i++ ) continue;
    }
    printf( "out of scope\n\n" );
    return 0;
}
