#include <iostream>
#include <stdlib.h>

#include "model_t.hpp"

using std::cout;
using std::endl;

task_t get_opt ( int argc, char **argv )
{
	task_t task;
	int option;
	task.adaptive    = false;
	task.with_memory = false;
	while ( (option=getopt(argc,argv,"i:o:s:deam")) != -1 )
	{
		switch ( option )
		{
		case 'i':
			task.file_in    = optarg;
			break;
		case 'o':
			task.file_out   = optarg;
			break;
		case 'm':
			task.with_memory = true;
			break;
		default :
			cout << "ERROR: unrecognized option " << option << endl;
			abort( );
		}
	}
	return task;
}

int main ( int argc, char **argv )
{
	task_t task = get_opt( argc, argv );

	model_t model;
	model.generate( &task );
	model.gen_prob( );
	//model.show_prob( );
	cout << model.get_entropy( ) << endl;
	return 0;
}
