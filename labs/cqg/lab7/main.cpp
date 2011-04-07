#include <memory>
#include <iostream>
#include <sstream>

#include "ScopedTimer.cpp"
#include "File.cpp"

void printTime ( const double Time )
{
    std::cout << "Total scope_life time= " << Time << " usecs" << std::endl;
}

void printTime2File ( const double Time )
{
    std::stringstream msg;
    msg << "Total scope_life time= " << Time << " usecs" << std::endl;
    File out_file( "times.log" );
    out_file.write( msg.str( ).c_str( ) );
}

void task1 ( )
{
/// (testing ScopedTimer)
    {
/// in that case it's no benefit using std::auto_ptr
        //std::auto_ptr<ScopedTimer> timer( new ScopedTimer ( printTime ) );
        ScopedTimer timer ( printTime );
        for ( int i=0; i < 1000000; i++ ) continue;
    }
    std::cout << "out of scope\n" << std::endl;
}

void task2 ( )
{
/// (testing File)
    {
        File out_file( "output" );
        out_file.write( "test message" );
    }
    std::cout << "out of scope\n" << std::endl;
}

void task3 ( )
{
    {
/// in that case it's no benefit using std::auto_ptr
        //std::auto_ptr<ScopedTimer> timer( new ScopedTimer ( printTime2File ) );
        ScopedTimer timer ( printTime2File );
        for ( int i=0; i < 1000000; i++ ) continue;
    }
    std::cout << "out of scope\n" << std::endl;
}

int main ( )
{
    task1( );
    task2( );
    task3( );

    return 0;
}
