#ifndef OHLC_AGGREGATOR_CPP
#define OHLC_AGGREGATOR_CPP

#include <iostream>
#include <stdio.h>
#include <boost/timer.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>

struct OHLC_t
{
    double Open;
    double High;
    double Low;
    double Close;
};

class OHLCAggregator
{
public:
    OHLCAggregator ( void fun ( const OHLC_t * ), const double timeSec ) : 
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
            ohlc.Open  = value;
            ohlc.High  = value;
            ohlc.Low   = value;
            firstValue = false;
        }
        else
        {
            if ( value > ohlc.High ) ohlc.High = value;
            if ( value < ohlc.Low  ) ohlc.Low  = value;
        }
        ohlc.Close = value;
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
        ohlc.Open  = 0;
        ohlc.High  = 0;
        ohlc.Low   = 0;
        ohlc.Close = 0;
        timer_ -> restart( );
    }

    void print ( )
    {
        print_function( &ohlc );
    }

    boost::function<void( const OHLC_t * )> print_function;
    boost::timer * timer_;
    double timeout;

    OHLC_t ohlc;
    bool   firstValue;
};

#endif
