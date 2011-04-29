#include <iostream>

#include "arcode.cpp"

using std::cout;
using std::endl;

task_t get_opt ( int argc, char **argv )
{
	task_t task;
	int option;
	while ( (option=getopt(argc,argv,"i:o:deam")) != -1 )
	{
		switch ( option )
		{
		case 'i':
			task.file_in = optarg;
			break;
		case 'o':
			task.file_out = optarg;
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
	//arcode.initial( argc, argv );
	arcode.initial( &task );
	return 0;
}
