#include <iostream>

#include "arcode.hpp"

using std::cout;
using std::endl;

task_t get_opt ( int argc, char **argv )
{
	task_t task;
	int option;
	task.adaptive    = false;
	task.with_memory = false;
	task.with_model  = false;
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
		case 's':
			task.with_model = true;
			task.file_model = optarg;
			break;
		case 'e':
			task.mode = task_t::encoding;
			break;
		case 'd':
			task.mode = task_t::decoding;
			break;
		case 'a':
			task.adaptive = true;
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

	arcode_t arcode;
	arcode.initial( &task );

	return 0;
}
