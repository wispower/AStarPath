#pragma once
#include "PathFinder.h"

class GameObject
{
public:
	GameObject(void);
	~GameObject(void);
	int InitObject();
	int UpdateObject( float fTimeElapse);
	int DrawObject( HDC hdc );
	void SetDestination( int ex, int ey );
private:
	int m_xPos;
	int m_yPos;
	int m_xDesPos;
	int m_yDesPos;

	HPEN m_oldPen;
	HPEN m_pen;
	HPEN m_pathPen;
	HBRUSH m_oldBrush;
	HBRUSH m_brush;
	HBRUSH m_pathBrush;
	HPEN m_destPen;
	HBRUSH m_destBrush;

	PathFinder m_pathFinder;
	std::vector< Map::MapPoint > m_vPath;
	int m_nPathStep;
};

