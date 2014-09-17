#pragma once
#include "Map.h"
#include <vector>
#include "FFOpenList.h"
#include "FFBinaryHeap.h"

class PathFinder
{
public:
	enum PATHRESULT {
		ERROR_SEARCHPOINT = 0,
		CANNOT_FINDPATH,
		FINDPATH
	};
	enum DIRECTION {
		EAST = 0,
		SOUTHEAST,
		SOUTH,
		SOUTHWEST,
		WEST,
		NORTHWEST,
		NORTH,
		NORTHEAST
	};

	struct PathNode{
		PathNode( int xParam, int yParam, float costSoFarParam, float heuristicParm, float costParam );
		int x;
		int y;
		float costSoFar;
		float heuristic;
		float cost;
		bool operator< ( const PathNode& B ){
			return cost < B.cost;
		}
		bool operator== ( const PathNode& B ){
			return ( x == B.x && y == B.y );
		}
	};

	//typedef FFOpenList< PathNode > OpenList;
	typedef FFBinaryHeap< PathNode > OpenList;

public:
	PathFinder();
	~PathFinder(void);

	int Init( Map* pMap );
	void SetBeginPoint( int bx, int by );
	void SetEndPoint( int ex, int ey );
	PATHRESULT FindPath( std::vector< Map::MapPoint >& vPath );

private:
	int Visit( int x, int y, DIRECTION dir, float c, int* pVisited, OpenList& pq );
	bool Passable(  int x, int y, DIRECTION dir1, DIRECTION dir2, int* pVisited );
private:
	static int s_xDirectOffset[8];
	static int s_yDirectOffset[8];
	Map*    m_pMap;
	Map::MapPoint  m_beginPoint;
	Map::MapPoint  m_endPoint;
};

