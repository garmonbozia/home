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
using std::advance;

typedef enum { two=0, three=1 } dimension_t;

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

    void setDimension( dimension_t dimension );
    void initial ( );
    void showAdjacencyMatrix ( );
    void showCoord ( );
    void showGroups ( );
    void showBordered ( );
    max_min_t<double> maxMinGroupSize ( );
    max_min_t<double> maxMinBorderedGroupSize ( );
    max_min_t<double> maxMinConnectedOrBorderedGroupSize ( );
    int connectiveGroupNumber ( );
    int nonconnectiveGroupNumber ( );
    bool isConnectiveGroup ( const int group_number );
    bool member ( const int el, vector<int> elements );
    bool isInGroup ( const int el, const int group_number );
    bool existEdge ( const int i, const int j );
    int getActiveGroupNumber ( );
    int getFirstGroupElement ( const int group_number );
    int getGroupSize ( const int group_number );
    void insert ( vector<int> * v, const int el );
    void insertFrom ( vector<int> * v, vector<int> * s );

private:
    int                  size_;
    bool **              adjacency_matrix_;
    vector<vector<int> > adj_;
    vector<coord_t>      coord_;
    vector<int>          groups_;
    map<int, bool>       groups_existance_;
    vector<bool>         is_on_border_;

    dimension_t dimension;

    const char * coord_file;
    const char * group_file;
    const char * border_file;
};

#endif
