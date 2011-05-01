#include <iostream>

#define DIMENTION_THREE
#include "jak_grf.h"

using std::cout;
using std::endl;

int main ( )
{
#ifdef DIMENTION_THREE
	jak_grf graph( "5.ig", "5_border", "5_groupet" );
#else
	jak_grf graph( "6.ig", "6_border", "6_groupet" );
#endif
	graph.initial( );
#ifndef DIMENTION_THREE
	graph.show_adjacency_matrix( );
	graph.show_coord( );
	graph.show_groups( );
	graph.show_bordered( );
#endif

	max_min_t<double> max_min_size;
	max_min_size = graph.maxMinGroupSize();
	cout << "max group size= " << max_min_size.max;
	cout << "(" << max_min_size.index_max << ")" << endl;
	cout << "min group size= " << max_min_size.min;
	cout << "(" << max_min_size.index_min << ")" << endl;

	max_min_size = graph.maxMinBorderedGroupSize();
	cout << "max bordered group size= " << max_min_size.max;
	cout << "(" << max_min_size.index_max << ")" << endl;
	cout << "min bordered group size= " << max_min_size.min;
	cout << "(" << max_min_size.index_min << ")" << endl;

	max_min_size = graph.maxMinConnectedOrBorderedGroupSize();
	cout << "max connected or bordered group size= " << max_min_size.max;
	cout << "(" << max_min_size.index_max << ")" << endl;
	cout << "min connected or bordered group size= " << max_min_size.min;
	cout << "(" << max_min_size.index_min << ")" << endl;

	cout << "connective group number= " << graph.connectiveGroupNumber( ) << endl;
	return( 0 );
}
