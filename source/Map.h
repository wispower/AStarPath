#pragma once
#include <vector>
#include <queue>
class PathFinder;

class Map
{
	friend class PathFinder;
public:
	Map(void);
	~Map(void);
	int InitMap( const char* strPath );
	void Draw( HDC hdc );
	int GetWidth();
	int GetHeight();

public:
	struct MapPoint{
		int x;
		int y;
	};

private:
	int ReadDataFromFile( const char* strPath );
private:
	int m_mapData[WINDOW_HEIGHT/GRID_HEIGHT][WINDOW_WIDTH/GRID_WIDTH];
	int m_nWidth;
	int m_nHeight;
	HPEN m_oldPen;
	HPEN m_obstaclePen;
	HPEN m_reachablePen;
	HBRUSH m_oldBrush;
	HBRUSH m_obstacleBrush;
	HBRUSH m_reachableBrush;
};
