#ifndef OHLC_AGGREGATOR_CPP
#define OHLC_AGGREGATOR_CPP

#include <iostream>
#include <stdio.h>
#include <boost/timer.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>

/*
class OHLC
{
    double Open,
    double High,
    double Low,
    double Close
};
*/

class OHLCAggregator
{
public:
    OHLCAggregator ( void fun ( double, double, double, double ), const double timeSec ) : 
        timeout( timeSec ),
        print_function( fun ),
        timer_( new boost::timer )
    {
        init( );
    }

    void push ( const double value )
    {
        if ( timer_->elapsed( ) >= timeout )
        {
            print( );
            init( );
            return;
        }
        if ( firstValue )
        {
            valueOpen  = value;
            valueHigh  = value;
            valueLow   = value;
            firstValue = false;
        }
        else
        {
            if ( value > valueHigh ) valueHigh = value;
            if ( value < valueLow  ) valueLow  = value;
        }
        valueClose = value;
    }

    ~OHLCAggregator ( )
    {
        print( );
        delete( timer_ );
    }

private:
    void init ( )
    {
        firstValue = true;
        valueOpen  = 0;
        valueHigh  = 0;
        valueLow   = 0;
        valueClose = 0;
        timer_ -> restart( );
    }

    void print ( )
    {
        print_function( valueOpen, valueHigh, valueLow, valueClose );
    }

    boost::function<void( double, double, double, double )> print_function;
    boost::timer * timer_;
    double timeout;

    double valueOpen;
    double valueHigh;
    double valueLow;
    double valueClose;
    bool   firstValue;
};

#endif
