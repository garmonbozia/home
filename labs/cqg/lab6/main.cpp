#include <stdlib.h>

#include "CTask.hpp"

#define INPUT_FILE_ARG  (argc-2)
#define OUTPUT_FILE_ARG (argc-1)

void check_input ( const int argc )
{
	if ( argc != 4 && argc != 5 )
	{
		exit( -1 );
	}

}

int main ( const int argc, const char** argv )
{
	check_input  ( argc       );
	CTask my_task( argc, argv );

	my_task.readData   ( argv[INPUT_FILE_ARG ] );
	my_task.doOperation( argv[OUTPUT_FILE_ARG] );

	return 0;
}
