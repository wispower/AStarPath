#include "StdAfx.h"
#include "Map.h"
#include <fstream>
#include "PathFinder.h"

Map::Map(void) : m_nWidth( WINDOW_WIDTH/GRID_WIDTH ), 
	m_nHeight( WINDOW_HEIGHT/GRID_HEIGHT )
{
}


Map::~Map(void)
{
}

int Map::InitMap(  const char* strPath  )
{
	memset( m_mapData, 0, m_nWidth*m_nHeight );
	ReadDataFromFile( strPath );

	m_oldPen = NULL;
	m_oldBrush = NULL;
	m_obstaclePen = CreatePen( PS_SOLID, 1, RGB(0, 0, 255) );
	m_reachablePen = CreatePen( PS_SOLID, 1, RGB(200, 200, 200) );
	m_obstacleBrush = CreateSolidBrush( RGB( 0, 0, 255 ) );
	m_reachableBrush = CreateSolidBrush( RGB( 255, 255, 255) );
	return 0;
}

int Map::ReadDataFromFile( const char* strPath )
{
	std::ifstream ifile( strPath );
	for( int i = 0; i < m_nHeight; i++ ){
		for( int j = 0; j < m_nWidth; j++ ){
			ifile >> m_mapData[i][j];
		}
	}
	ifile.close();

	return 0;
}

void Map::Draw( HDC hdc )
{
	m_oldPen = (HPEN)SelectObject( hdc, m_reachablePen );
	m_oldBrush = (HBRUSH)SelectObject( hdc, m_reachableBrush );

	for( int i = 0; i < m_nHeight; i++ ){
		MoveToEx( hdc, 0, i * GRID_HEIGHT, NULL );
		LineTo( hdc, WINDOW_WIDTH, i * GRID_HEIGHT );
	}
	for( int i = 0; i < m_nWidth; i++ ){
		MoveToEx( hdc, i * GRID_WIDTH, 0, NULL );
		LineTo( hdc, i * GRID_WIDTH, WINDOW_HEIGHT );
	}

	SelectObject( hdc, m_obstaclePen );
	SelectObject( hdc, m_obstacleBrush );
	int xPos = 0;
	int yPos = 0;
	for( int i = 0; i < WINDOW_HEIGHT / GRID_HEIGHT; i++ ){
		yPos = i * GRID_HEIGHT;
		for( int j = 0; j < WINDOW_WIDTH / GRID_WIDTH; j++ ){
			xPos = j * GRID_WIDTH;
			if( m_mapData[i][j] == 1 ){
				Rectangle( hdc, xPos, yPos, xPos + GRID_WIDTH+1, yPos + GRID_HEIGHT+1 );
			}
		}
	}

	SelectObject( hdc, m_oldPen );
	SelectObject( hdc, m_oldBrush );
}

int Map::GetWidth()
{
	return m_nWidth;
}
int Map::GetHeight()
{
	return m_nHeight;
}
