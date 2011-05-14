/*
 * fractal.cpp - Sierpinski Triangle simple drawer
 *
 *  Created on: May 14, 2011
 *      Author: lk
 */

#include <GL/glut.h>
#include <GL/freeglut.h>
#include <utility>
#include <iostream>

using std::cout;
using std::endl;

#define ASSERT(A, MSG)\
	if ( !A ) cout << MSG << endl;

#define WINDOW_SIZE_WIDTH      500
#define WINDOW_SIZE_HEIGHT     500
#define WINDOW_INIT_POSITION_X 0
#define WINDOW_INIT_POSITION_Y 0
#define DRAW_BACKGROUND_COLOR  0.0, 0.0, 0.0
#define DRAW_TRI_BACK_COLOR    1.0, 0.93, 0.29
#define DRAW_EDGES_LINE_SIZE   1
#define RECURSION_DEPTH        9

class point
{
public:
	point ( double in_x, double in_y ) : x(in_x), y(in_y) {};
	void print ( )
	{
		cout << "(" << x << ", " << y << ")" << endl;
	}
	double x;
	double y;
};

void drawLine2 ( const point a, const point b )
{
	glVertex2f( a.x, a.y );
	glVertex2f( b.x, b.y );
}

void drawTriangle ( const point a, const point b, const point c )
{
	glBegin( GL_POLYGON );
		drawLine2( a, b );
		drawLine2( b, c );
	glEnd( );
}

void drawSierpinskiTriangle ( const point a
		                    , const point b
		                    , const point c
		                    , const int depth
		                    )
{
	//cout << "depth= " << depth << endl;
	if ( depth == 0 )
		return;
	ASSERT( a.y == c.y, "" );
	ASSERT( b.x == 0.5*(c.x-a.x), "" );

	point a_( a.x+0.75*(c.x - a.x), a.y+0.5*(b.y-a.y) );
	point b_( b.x                 , a.y               );
	point c_( a.x+0.25*(c.x - a.x), a.y+0.5*(b.y-a.y) );
	drawTriangle( c_, a_, b_ );
	drawSierpinskiTriangle( a , c_, b_, depth-1 );
	drawSierpinskiTriangle( c_, b , a_, depth-1 );
	drawSierpinskiTriangle( b_, a_, c , depth-1 );
}

void myDisplayFunc ( )
{
    glClear( GL_COLOR_BUFFER_BIT );
    glPushMatrix( );

	glLineWidth( DRAW_EDGES_LINE_SIZE );

/// draw 0's background triangle
	glColor3d( DRAW_TRI_BACK_COLOR );
	point a( -1, -1 );
	point b( 0, 1 );
	point c( 1, -1 );
	drawTriangle( a, b , c );

/// draw all triangles
	glColor3d( DRAW_BACKGROUND_COLOR );
	drawSierpinskiTriangle( a, b, c, RECURSION_DEPTH );
    glPopMatrix( );
    glutSwapBuffers( );
}

void myKeyboardFunc ( unsigned char key, int x, int y )
{
    cout << "key (" << static_cast<int>( key ) << ") pressed" << endl;
    if ( key == 27 )
/// exit (escape)
    	exit( 0 );
}

int main ( int argc, char **argv )
{
	glutInit( &argc, argv );
	glutInitWindowSize( WINDOW_SIZE_WIDTH, WINDOW_SIZE_HEIGHT );
	glutInitWindowPosition( WINDOW_INIT_POSITION_X, WINDOW_INIT_POSITION_Y );
	glutInitDisplayMode( GLUT_RGBA );
	glutCreateWindow( "Sierpinski Triangle" );
	glutKeyboardFunc( myKeyboardFunc );
	glutDisplayFunc( myDisplayFunc );
	glutMainLoop( );

	return( 0 );
}
