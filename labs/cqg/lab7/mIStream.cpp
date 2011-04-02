#ifndef MISTREAM_CPP
#define MISTREAM_CPP

#include <stdio.h>
#include <stdlib.h>

class mIStream
{
public:
    mIStream ( const char * filename )
    {
        pFile = fopen( filename, "w" );
        if ( !pFile )
             exit( -1 );
        printf( "mIStream: constructed\n" );
    }

    ~mIStream ( )
    {
        fclose( pFile );
        printf( "mIStream: destructed\n" );
    }

    int write ( const char * string)
    {
        fputs( string, pFile );
    }

private:
    FILE * pFile;
};

#endif
