#ifndef FILE_CPP
#define FILE_CPP

#include <stdio.h>
#include <stdlib.h>

class File
{
public:
    File ( const char * filename )
    {
        pFile = fopen( filename, "w" );
        if ( !pFile )
             exit( -1 );
        printf( "File: constructed\n" );
    }

    ~File ( )
    {
        fclose( pFile );
        printf( "File: destructed\n" );
    }

    int write ( const char * string)
    {
        fputs( string, pFile );
    }

private:
    FILE * pFile;
};

#endif
