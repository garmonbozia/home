#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

#include <CTask.hpp>

static const std::string kFind  = "find";
static const std::string kSort  = "sort";
static const std::string kCount = "count";

CTask::CTask (const int argc, const char** argv ) : operation_ ( argv[1], 0 )
{
	if ( argc == 5 )
	{
		std::stringstream buf( std::stringstream::in |
		                       std::stringstream::out );
		int value;

		buf << argv[2];
		buf >> value;

		operation_.second = value;
	}
}

void CTask::readData ( const char * input_file )
{
	std::ifstream data( input_file, std::ifstream::in );
	if ( !data.good() )
	{
		exit( -1 );
	}

	int buf;
	while ( data >> buf )
		data_.push_back(buf);
}

void CTask::doOperation ( const char * output_file )
{
	std::ofstream result ( output_file, std::ofstream::out );
	std::list<int>::iterator i;

	if ( operation_.first == kFind )
	{
		i            = std::find( data_.begin()
		                        , data_.end()
								, operation_.second );
		int position = std::distance (data_.begin (), i) + 1;
		
		if ( position <= data_.size() )
		{
			result    << position << std::endl;
			std::cout << position << std::endl;
		}
		else
		{
			result    << "Search has failed, not found" << std::endl;
			std::cout << "Search has failed, not found" << std::endl;
		}

	}
	else if ( operation_.first == kCount )
	{
		int count_ = (int) std::count( data_.begin()
		                             , data_.end()
									 , operation_.second
									 );

		if ( count_ > 0 )
		{
			result    << count_ << std::endl;
			std::cout << count_ << std::endl;
		}
		else
		{
			result    << "Search has failed, not found" << std::endl;
			std::cout << "Search has failed, not found" << std::endl;
		}
	}
	else if ( operation_.first == kSort )
	{
		data_.sort();

		for ( i = data_.begin(); i != data_.end(); i++ )
		{
			result    << (*i) << " ";
			std::cout << (*i) << " ";
		}

		result    << std::endl;
		std::cout << std::endl;
	}
	else 
	{
		result    << "Failed, no such operation `";
		result    << operation_.first << "'" << std::endl;
		std::cout << "Failed, no such operation `";
		std::cout  << operation_.first << "'" << std::endl;
	}
}
