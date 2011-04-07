#ifndef FILE_CPP
#define FILE_CPP

#include <stdio.h>
#include <stdlib.h>

class File
{
public:
    File ( const char * filename )
    {
        pFile = fopen( filename, "a" );
        if ( !pFile )
             exit( -1 );
        printf( "File: constructed\n" );
    }

    ~File ( )
    {
        if ( EOF == fclose( pFile ) )
            printf( "File closing error\n" );
        printf( "File: destructed\n" );
    }

    void write ( const char * string)
    {
        if ( EOF == fputs( string, pFile ) )
            printf( "File writing error\n" );
    }

private:
    FILE * pFile;
};

#endif
