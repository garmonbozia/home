#include "ScopedTimer.cpp"
#include <memory>

void printTime ( double Time )
{
    printf ( "Total scope_life time= %0.2f usecs\n", Time );
}

int main ( )
{
    {
        std::auto_ptr<ScopedTimer> timer( new ScopedTimer ( printTime ) );
        for ( int i=0; i < 1000000; i++ ) continue;
    }
    
    printf( "out of scope\n" );
    return 0;
}
