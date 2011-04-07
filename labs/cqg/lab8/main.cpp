#include <sstream>
#include <boost/random.hpp>

#include "ScopedTimer.cpp"
#include "File.cpp"
#include "OHLCAggregator.cpp"

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

void printOHLC2File ( const OHLC_t ohlc )
{
    std::stringstream msg;
    msg << "O= " << ohlc.Open  << ", ";
    msg << "H= " << ohlc.High  << ", ";
    msg << "L= " << ohlc.Low   << ", ";
    msg << "C= " << ohlc.Close << std::endl;
    File out_file( "OHLC.log" );
    out_file.write( msg.str( ).data( ) );
}

void task1 ( )
/// testing ScopedTimer
{
    {
        ScopedTimer timer ( printTime );
        for ( int i=0; i < 1000000; i++ ) continue;
    }
    std::cout << "out of scope\n" << std::endl;
}

void task2 ( )
{
    {
        ScopedTimer timer ( printTime2File );
        for ( int i=0; i < 1000000; i++ ) continue;
    }
    std::cout << "out of scope\n" << std::endl;
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

