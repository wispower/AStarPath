#include "StdAfx.h"
#include "PathFinder.h"

typedef std::vector< Map::MapPoint >::iterator MapPointIter;

int PathFinder::s_xDirectOffset[8] = { 1, 1, 0, -1, -1, -1,  0,  1 };
int PathFinder::s_yDirectOffset[8] = { 0, 1, 1,  1,  0, -1, -1, -1 };

PathFinder::PathFinder()
{
	m_pMap = NULL;

	m_beginPoint.x = -1;
	m_beginPoint.y = -1;
	m_endPoint.x = -1;
	m_endPoint.y = -1;
}

int PathFinder::Init( Map* pMap )
{
	m_pMap = pMap;

	return 0;
}


PathFinder::~PathFinder(void)
{
}

PathFinder::PathNode::PathNode( int xParam, int yParam, float costSoFarParam, float heuristicParm, float costParam )
{
	x = xParam;
	y = yParam;
	costSoFar = costSoFarParam;
	heuristic = heuristicParm;
	cost = costParam;
}

void PathFinder::SetBeginPoint( int bx, int by )
{
	m_beginPoint.x = bx;
	m_beginPoint.y = by;
}
void PathFinder::SetEndPoint( int ex, int ey )
{
	m_endPoint.x = ex;
	m_endPoint.y = ey;
}

PathFinder::PATHRESULT PathFinder::FindPath( std::vector< Map::MapPoint >& vPath )
{	
	vPath.clear();

	if( m_beginPoint.x < 0 || m_beginPoint.y < 0 || 
		m_endPoint.x < 0 || m_endPoint.y < 0 ||
		m_beginPoint.x > m_pMap->GetWidth() || m_endPoint.x > m_pMap->GetWidth() ||
		m_beginPoint.y > m_pMap->GetHeight() || m_endPoint.y > m_pMap->GetHeight() ){
			return ERROR_SEARCHPOINT;
	}
	if( m_beginPoint.x == m_endPoint.x && m_beginPoint.y == m_endPoint.y ){
		return ERROR_SEARCHPOINT;
	}

	int* visited = new int[ m_pMap->GetWidth() * m_pMap->GetHeight() ];
	memcpy( visited, m_pMap->m_mapData, sizeof(int)*m_pMap->m_nWidth*m_pMap->m_nHeight );
	visited[m_beginPoint.y * m_pMap->m_nWidth + m_beginPoint.x] = 2;

	Map::MapPoint parent[WINDOW_HEIGHT/GRID_HEIGHT][WINDOW_WIDTH/GRID_WIDTH];
	ZeroMemory( parent, sizeof(Map::MapPoint) * m_pMap->m_nWidth * m_pMap->m_nHeight );

	OpenList pq;
	pq.push( PathNode(m_beginPoint.x, m_beginPoint.y, 0, 0, 0) );

	bool bReachEnd = false;
	Map::MapPoint tmpPoint;
	// 8 direction
	// (x+1, y), (x+1, y+1), (x, y+1), (x-1, y+1), (x-1, y), (x-1, y-1), (x, y-1), (x+1, y-1)
	while( !pq.empty() ){
		PathNode pd = pq.top();
		pq.pop();

		float cost[]     = { 1.0f,     1.4f,     1.0f,     1.4f,     1.0f,     1.4f,     1.0f,     1.4f  };
		for( int i = 0; i < 8; i++ ){
			int visitRet = Visit( pd.x, pd.y, (DIRECTION)i, pd.costSoFar + cost[i], visited, pq );
			if( visitRet != 2 ){
				parent[pd.y + s_yDirectOffset[i]][pd.x + s_xDirectOffset[i]].x = pd.x;
				parent[pd.y + s_yDirectOffset[i]][pd.x + s_xDirectOffset[i]].y = pd.y;
			}
			if( visitRet == 1 ){
				bReachEnd = true;
				break;
			}
		}

		if( bReachEnd ){
			std::vector< Map::MapPoint > reversePath;
			int x = m_endPoint.x;
			int y = m_endPoint.y;
			Map::MapPoint parentPoint = parent[y][x];
			while( !( parentPoint.x == m_beginPoint.x && parentPoint.y == m_beginPoint.y ) ){
				tmpPoint.x = x;
				tmpPoint.y = y;
				reversePath.push_back( tmpPoint );
				x = parentPoint.x;
				y = parentPoint.y;
				parentPoint = parent[y][x];
			}
			tmpPoint.x = x;
			tmpPoint.y = y;
			reversePath.push_back( tmpPoint );

			reversePath.push_back( m_beginPoint );
			int nSize = reversePath.size();
			for( int i = nSize - 1; i >= 0; i-- ){
				vPath.push_back( reversePath[i] );
			}

			break;
		}
	}

	delete[] visited;

	if( !bReachEnd ){
		return CANNOT_FINDPATH;
	}

	return FINDPATH;
}

bool PathFinder::Passable(  int x, int y, DIRECTION dir1, DIRECTION dir2, int* pVisited )
{
	int x1 = x + s_xDirectOffset[ dir1 ];
	int y1 = y + s_yDirectOffset[ dir1 ];
	int x2 = x + s_xDirectOffset[ dir2 ];
	int y2 = y + s_yDirectOffset[ dir2 ];
	if( pVisited[x1 + y1 * m_pMap->m_nWidth] == 1 ||
		pVisited[x2 + y2 * m_pMap->m_nWidth] == 1 )
	{
		return false;
	}

	return true;
}
int PathFinder::Visit( int x, int y, DIRECTION dir, float c, int* pVisited, OpenList& pq )
{
	int xn = x + s_xDirectOffset[ dir ];
	int yn = y + s_yDirectOffset[ dir ];

	if( xn < 0 || yn < 0 || xn >= m_pMap->m_nWidth || yn >= m_pMap->m_nHeight || pVisited[xn + yn * m_pMap->m_nWidth] == 1 ){
		return 2;
	}

	if( dir == SOUTHEAST && !Passable( x, y, EAST, SOUTH, pVisited ) ){
		return 2;
	}
	else if( dir == SOUTHWEST && !Passable( x, y, WEST, SOUTH, pVisited ) ){
		return 2;
	}
	else if( dir == NORTHWEST && !Passable( x, y, WEST, NORTH, pVisited ) ){
		return 2;
	}
	else if( dir == NORTHEAST && !Passable( x, y, EAST, NORTH, pVisited ) ){
		return 2;
	}

	if( xn == m_endPoint.x && yn == m_endPoint.y ){
		return 1;
	}

	float heuristic = sqrt( (float)( xn - m_endPoint.x ) * ( xn - m_endPoint.x ) + ( yn - m_endPoint.y ) * ( yn - m_endPoint.y ) );
	//float heuristic = abs( xn - m_endPoint.x ) + abs( yn - m_endPoint.y );
	float dist = c + heuristic;

	if( pVisited[xn + yn * m_pMap->m_nWidth] == 2 ){
		if( !( pq.refresh( PathNode( xn, yn, c, heuristic, dist ) ) ) ){
			return 2;
		}
	}
	else{
		pVisited[xn + yn * m_pMap->m_nWidth] = 2;
		pq.push( PathNode( xn, yn, c, heuristic, dist ) );
	}
	return 3;
}
