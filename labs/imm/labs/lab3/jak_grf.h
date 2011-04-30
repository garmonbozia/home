#ifndef JAK_GRF_H
#define JAK_GRF_H

#include <fstream>
#include <string>
#include <assert.h>
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <climits>

using std::cout;
using std::endl;
using std::string;
using std::ifstream;
using std::map;
using std::vector;
using std::pair;
using std::count;
using std::find;

class coord_t
{
public:
    double x;
    double y;
    double z;
};

template<class T> class max_min_t
{
public:
    T max;
    T min;
    int index_max;
    int index_min;
};

class jak_grf
{
public:
    jak_grf ( const char * coord_file
            , const char * border_file
            , const char * group_file
            ) :
        coord_file( coord_file ),
        border_file( border_file ),
        group_file( group_file )
    { }

    void initial ( )
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
        adjacency_matrix_ = new bool*[size_];
        int tmp_num, tmp_count, tmp_j;
        for ( int i=0; i<size_; i++ )
        {
            file >> tmp_num;
            assert( tmp_num == i );
            file >> tmp_count;
            adjacency_matrix_[i] = new bool[size_];
            for ( int j=0; j<tmp_count; j++ )
            {
                file >> tmp_j;
                adjacency_matrix_[i][tmp_j] = 1;
            }
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
#ifdef DIMENTION_THREE
            file >> tmp_coord.z;
#endif
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

    void show_adjacency_matrix ( )
    {
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
                cout << adjacency_matrix_[i][j] << " ";
            }
            cout << endl;
        }
        cout << "* printing adjacency matrix...[OK]" << endl << endl;
    }

    void show_coord ( )
    {
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
#ifdef DIMENSION_THREE
            cout << " z= " << (*i_coord).z;
#endif
            cout << endl;
            i++;
        }
        cout << "* printing coordinates... [OK]" << endl << endl;
    }

    void show_groups ( )
    {
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

    void show_bordered ( )
    {
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

    max_min_t<double> max_min_group_size ( )
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

    max_min_t<double> max_min_bordered_group_size ( )
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

    max_min_t<double> max_min_connected_or_bordered_group_size ( )
    {
        max_min_t<double> max_min_size;
        max_min_size.max = 0;
        max_min_size.min = INT_MAX;
        int tmp_count;
        bool element_matched;
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
                element_matched = false;
                if ( (*i_elem) == (*i_group).first )
                {
                    if ( is_on_border_[i] )
                        element_matched = true;
                    else
                    {
                        for ( int j=0; j<size_; j++ )
                        {
                            if ( ( adjacency_matrix_[i][j] ) &&
                                 ( groups_[j] != (*i_group).first)
                               )
                                element_matched = true;
                        }
                    }
                    if ( element_matched )
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

    int connective_group_number ( )
    {
        int group_number = 0;
        for ( map<int,bool>::iterator i_group=groups_existance_.begin()
            ; i_group!=groups_existance_.end()
            ; i_group++
            )
        {
            if ( ( (*i_group).second) &&
                 ( is_connective_group( (*i_group).first ))
                 )
                    group_number++;
        }
        return group_number;
    }

    int nonconnective_group_number ( )
    {
        return( get_active_group_number() - connective_group_number() );
    }

    bool is_connective_group ( const int group_number )
    {
        int elem1_num = 0;
        for ( vector<int>::iterator i_elem1=groups_.begin()
            ; i_elem1!=groups_.end()
            ; i_elem1++
            )
        {
            int elem2_num = 0;
            for ( vector<int>::iterator i_elem2=groups_.begin()
                ; i_elem2!=groups_.end()
                ; i_elem2++
                )
            {
                if ( ( (*i_elem1) == group_number ) &&
                     ( (*i_elem2) == group_number ) &&
                     ( elem1_num < elem2_num )
                   )
                {
                    vector<int> visited_elements;
                    if ( !is_connected_elements( group_number
                                               , elem1_num
                                               , elem2_num
                                               , visited_elements )
                                               )
                        return false;
                }
                elem2_num++;
            }
            elem1_num++;
        }
        return true;
    }

    bool is_connected_elements ( int group_number
                               , int element1
                               , int element2
                               , vector<int> visited_elements )
    {
        for ( int j=0; j<size_; j++ )
        {
            if ( ( groups_[j] == group_number )     &&
                 ( adjacency_matrix_[element1][j] ) &&
                 ( j != element1 )                  &&
                 ( !member( j, visited_elements ) )
                )
                {
                    if ( j == element2 )
                        return true;
                    visited_elements.push_back( j );
                    return is_connected_elements( group_number
                                                , j
                                                , element2
                                                , visited_elements
                                                );
                }
        }
    }

    bool member ( int el, vector<int> elements )
    {
        return ( find( elements.begin()
                     , elements.end()
                     , el
                     ) != elements.end() );
    }

    int get_active_group_number ( )
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

private:
    int             size_;
    bool **         adjacency_matrix_;
    vector<coord_t> coord_;
    vector<int>     groups_;
    map<int, bool>  groups_existance_;
    vector<bool>    is_on_border_;

    const char *    coord_file;
    const char *    group_file;
    const char *    border_file;
};

#endif

