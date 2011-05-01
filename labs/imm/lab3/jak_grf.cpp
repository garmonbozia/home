#include "jak_grf.hpp"

void jak_grf::setDimension ( dimension_t dimension_in )
{
	dimension = dimension_in;
}

void jak_grf::initial ( )
{
	cout << "* reading data..." ;
	ifstream file( coord_file );
	string tmp;
	file >> tmp;
	assert( !tmp.compare( "JAK_GRF" ) );
	file >> size_;
	file >> tmp;
	assert( !tmp.compare( "link" ) );

/// reading adjacency matrix
	//adjacency_matrix_ = new bool*[size_];
	int tmp_num, tmp_count, tmp_j;
	for ( int i=0; i<size_; i++ )
	{
		file >> tmp_num;
		assert( tmp_num == i );
		file >> tmp_count;
		//adjacency_matrix_[i] = new bool[size_];
		vector<int> adj_temp (0, 0);
		for ( int j=0; j<tmp_count; j++ )
		{
			file >> tmp_j;
			//adjacency_matrix_[i][tmp_j] = 1;
			adj_temp.push_back( tmp_j );
		}
		adj_.push_back( adj_temp );
	}

/// reading coordinates
	file >> tmp;
	assert( !tmp.compare( "coord" ) );
	double tmp_coord1, tmp_coord2;
	coord_t tmp_coord;
	for (int i=0; i<size_; i++ )
	{
		file >> tmp_num;
		assert( tmp_num == i );
		file >> tmp_coord.x;
		file >> tmp_coord.y;
		if ( dimension == three )
			file >> tmp_coord.z;
		coord_.push_back( tmp_coord );
	}
	file.close();

/// reading groups
	file.open( group_file );
	groups_.assign( size_, 0);
	for ( int i=0; i<size_; i++ )
	{
		file >> groups_[i];
		groups_existance_[groups_[i]] = true;
	}
	file.close();

/// reading borders
	file.open( border_file );
	is_on_border_.assign( size_, false );
	for ( int i=0; i<size_; i++ )
	{
		file >> tmp_j;
		assert( (tmp_j == 0) || (tmp_j == 1) );
		is_on_border_[i] = (tmp_j == 1);
	}
	file.close( );

	cout << "[OK]" << endl;
}

void jak_grf::showAdjacencyMatrix ( )
{
	if ( dimension == three )
		return;
	cout << endl << "* printing adjacency matrix..." << endl;
	cout << " |";
	for ( int i=0; i<size_; i++ )
		cout << i << " ";
	cout << endl;
	cout << "-";
	for ( int i=0; i<size_; i++ )
		cout << "--";
	cout << endl;

	for ( int i=0; i<size_; i++ )
	{
		cout << i << "|";
		for ( int j=0; j<size_; j++ )
		{
			//cout << adjacency_matrix_[i][j] << " ";
			cout << static_cast<int>( existEdge( i, j ) ) << " ";
		}
		cout << endl;
	}
	cout << "* printing adjacency matrix...[OK]" << endl << endl;
}

void jak_grf::showCoord ( )
{
	if ( dimension == three )
		return;
	cout << endl << "* printing coordinates..." << endl;
	int i=0;
	for ( vector<coord_t>::iterator i_coord=coord_.begin()
		; i_coord!=coord_.end()
		; i_coord++
		)
	{
		cout << i << ":";
		cout << " x= " << (*i_coord).x;
		cout << " y= " << (*i_coord).y;
		if ( dimension == three )
			cout << " z= " << (*i_coord).z;
		cout << endl;
		i++;
	}
	cout << "* printing coordinates... [OK]" << endl << endl;
}

void jak_grf::showGroups ( )
{
	if ( dimension == three )
		return;
	cout << endl << "* printing groups..." << endl;
	int i=0;
	for ( vector<int>::iterator i_group=groups_.begin()
		; i_group!=groups_.end()
		; i_group++
		)
	{
		cout << i << ":" << (*i_group) << endl;
		i++;
	}
	cout << "* printing groups... [OK]" << endl << endl;
}

void jak_grf::showBordered ( )
{
	if ( dimension == three )
		return;
	cout << endl << "* printing bordered elements..." << endl;
	int i=0;
	for ( vector<bool>::iterator i_bordered=is_on_border_.begin()
		; i_bordered!=is_on_border_.end()
		; i_bordered++
		)
	{
		if (*i_bordered)
			cout << i << endl;
		i++;
	}
	cout << "* printing bordered elements... [OK]" << endl << endl;
}

max_min_t<double> jak_grf::maxMinGroupSize ( )
{
	max_min_t<double> max_min_size;
	max_min_size.max = 0;
	max_min_size.min = INT_MAX;
	int tmp_count;
	for ( map<int,bool>::iterator i_group=groups_existance_.begin()
		; i_group!=groups_existance_.end()
		; i_group++
		)
	{
		if ( !(*i_group).second )
			continue;
		tmp_count = count( groups_.begin(), groups_.end(), (*i_group).first );
		if ( tmp_count > max_min_size.max )
		{
			max_min_size.max = tmp_count;
			max_min_size.index_max = (*i_group).first;
		}
		if ( tmp_count < max_min_size.min )
		{
			max_min_size.min = tmp_count;
			max_min_size.index_min = (*i_group).first;
		}
	}
	return max_min_size;
}

max_min_t<double> jak_grf::maxMinBorderedGroupSize ( )
{
	max_min_t<double> max_min_size;
	max_min_size.max = 0;
	max_min_size.min = INT_MAX;
	int tmp_count;
	for ( map<int,bool>::iterator i_group=groups_existance_.begin()
		; i_group!=groups_existance_.end()
		; i_group++
		)
	{
		if ( !(*i_group).second )
			continue;
		tmp_count = 0;
		int i=0;
		for ( vector<int>::iterator i_elem=groups_.begin()
			; i_elem!=groups_.end()
			; i_elem++
			)
		{
			if ( (*i_elem) == (*i_group).first )
			{
				if ( is_on_border_[i] )
					tmp_count++;
			}
			i++;
		}
		if ( tmp_count > max_min_size.max )
		{
			max_min_size.max = tmp_count;
			max_min_size.index_max = (*i_group).first;
		}
		if ( tmp_count < max_min_size.min )
		{
			max_min_size.min = tmp_count;
			max_min_size.index_min = (*i_group).first;
		}
	}
	return max_min_size;
}

max_min_t<double> jak_grf::maxMinConnectedOrBorderedGroupSize ( )
{
	max_min_t<double> max_min_size;
	max_min_size.max = 0;
	max_min_size.min = INT_MAX;
	int tmp_counter;
	bool element_matched;
	for ( map<int,bool>::iterator i_group=groups_existance_.begin()
		; i_group!=groups_existance_.end()
		; i_group++
		)
	{
		tmp_counter = 0;
		if ( !(*i_group).second )
			continue;
		for ( vector<int>::iterator i_elem=groups_.begin()
			; i_elem!=groups_.end()
			; i_elem++, element_matched=false
			)
		{
			if ( (*i_elem) == (*i_group).first )
			{
				if ( is_on_border_[(*i_elem)] )
					element_matched = true;
				else
				{
					for ( vector<int>::iterator j_elem=adj_[(*i_elem)].begin()
						; j_elem!=adj_[(*i_elem)].end(), !element_matched
						; j_elem++
						)
					{
						element_matched = not( isInGroup( (*j_elem ), (*i_group).first ) );
					}
				}
				if ( element_matched )
					tmp_counter++;
			}
		}
		if ( tmp_counter > max_min_size.max )
		{
			max_min_size.max       = tmp_counter;
			max_min_size.index_max = (*i_group).first;
		}
		if ( tmp_counter < max_min_size.min )
		{
			max_min_size.min       = tmp_counter;
			max_min_size.index_min = (*i_group).first;
		}
	}
	return max_min_size;
}

int jak_grf::connectiveGroupNumber ( )
{
	int group_number = 0;
	for ( map<int,bool>::iterator i_group=groups_existance_.begin()
		; i_group!=groups_existance_.end()
		; i_group++
		)
	{
		if ( ( (*i_group).second) &&
			 ( isConnectiveGroup( (*i_group).first ))
			 )
				group_number++;
	}
	return group_number;
}

int jak_grf::nonconnectiveGroupNumber ( )
{
	return( getActiveGroupNumber() - connectiveGroupNumber() );
}


bool jak_grf::isConnectiveGroup ( const int group_number )
{
	int visited_pointer = 0;
	int visited_pointer_increase = 0;
	vector<int> visited;
	insert( &visited, getFirstGroupElement( group_number ) );
	int visited_size_prev;
	while ( true )
	{
		visited_size_prev = visited.size( );
		vector<int> push_in_visited;
		vector<int>::iterator i_visited = visited.begin();
		for ( advance( i_visited, visited_pointer )
			; i_visited!=visited.end()
			; i_visited++
			)
		{
			for ( vector<int>::iterator i_neighbour=adj_[(*i_visited)].begin()
				; i_neighbour!=adj_[(*i_visited)].end()
				; i_neighbour++
				)
			{
				if ( not( isInGroup( (*i_neighbour), group_number ) ) )
					continue;
				insert( &push_in_visited, (*i_neighbour) );
			}
		}
		insertFrom( &visited, &push_in_visited );
		visited_pointer_increase = visited.size( ) - visited_size_prev;
		visited_pointer =visited.size() - visited_pointer_increase;
		if ( !visited_pointer_increase )
			break;
	}
	bool result = ( visited.size() == getGroupSize( group_number ) );
	cout << "group #" << group_number;
	if ( result )
		 cout << " is connective" << endl;
	else
		 cout << " is nonconnective" << endl;
	return result;
}

bool jak_grf::member ( const int el, vector<int> elements )
{
	return ( find( elements.begin()
				 , elements.end()
				 , el
				 ) != elements.end() );
}

bool jak_grf::isInGroup ( const int el, const int group_number )
{
	return ( group_number == groups_[el] );
}

bool jak_grf::existEdge ( const int i, const int j )
{
	return member( j, adj_[i] ) ;
}

int jak_grf::getActiveGroupNumber ( )
{
	int active_group_number = 0;
	for ( map<int,bool>::iterator i_group=groups_existance_.begin()
		; i_group!=groups_existance_.end()
		; i_group++
		)
		if ( (*i_group).second )
			active_group_number++;
	return active_group_number;
}

int jak_grf::getFirstGroupElement ( const int group_number )
{
	return ( find( groups_.begin( )
				 , groups_.end( )
				 , group_number
				 ) - groups_.begin( )
		   );
}

int jak_grf::getGroupSize ( const int group_number )
{
	return count( groups_.begin(), groups_.end(), group_number );
}

void jak_grf::insert ( vector<int> * v, const int el )
{
	if ( not( member( el, *v ) ) )
		v->push_back( el );
}

void jak_grf::insertFrom ( vector<int> * v, vector<int> * s )
{
	for ( vector<int>::iterator i_push=s->begin()
		; i_push!= s->end()
		; i_push++
		)
	{
		insert( v, (*i_push) );
	}
}
