#ifndef CTASK_HPP
#define CTASK_HPP

#include <list>
#include <string>

class CTask
{
public:
	CTask( int argc, const char** argv );

	void readData   ( const char * input_file  );
	void doOperation( const char * output_file );

private:
	std::pair<std::string, int> operation_;
	std::list<int>              data_;
};

#endif

