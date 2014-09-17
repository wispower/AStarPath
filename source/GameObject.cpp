#include "StdAfx.h"
#include "GameObject.h"
#include <vector>
#include "PerformaceTimer.h"

extern Map g_map;

GameObject::GameObject(void) : m_xPos( 0 ), m_yPos( 0 ), m_vPath()
{
}


GameObject::~GameObject(void)
{
}

int GameObject::InitObject()
{
	m_oldPen = NULL;
	m_oldBrush = NULL;
	m_pen = CreatePen( PS_SOLID, 1, RGB(0, 255, 255) );
	m_brush = CreateSolidBrush( RGB( 0, 255, 255 ) );
	m_pathPen = CreatePen( PS_SOLID, 1, RGB(255, 0, 255) );
	m_pathBrush = CreateSolidBrush( RGB(255, 0, 255) );
	m_destPen = CreatePen( PS_SOLID, 1, RGB(255, 0, 0) );
	m_destBrush = CreateSolidBrush( RGB(255, 0, 0) );

	m_pathFinder.Init( &g_map );
	m_nPathStep = 0;
	m_vPath.clear();

	return 0;
}

int GameObject::UpdateObject( float fTimeElapse )
{
	if( m_nPathStep < (int)m_vPath.size() ){
		m_xPos = m_vPath[ m_nPathStep ].x;
		m_yPos = m_vPath[ m_nPathStep ].y;
		m_nPathStep++;
	}

	return 0;
}

int GameObject::DrawObject( HDC hdc )
{
	m_oldPen = (HPEN)SelectObject( hdc, m_destPen );
	m_oldBrush = (HBRUSH)SelectObject( hdc, m_destBrush );

	Rectangle( hdc, m_xDesPos*GRID_WIDTH, m_yDesPos*GRID_HEIGHT, m_xDesPos*GRID_WIDTH + GRID_WIDTH+1, m_yDesPos*GRID_HEIGHT + GRID_HEIGHT+1 );

	SelectObject( hdc, m_pen );
	SelectObject( hdc, m_brush );

	Ellipse( hdc, m_xPos * GRID_WIDTH, m_yPos * GRID_HEIGHT, m_xPos * GRID_WIDTH+GRID_WIDTH, m_yPos * GRID_HEIGHT+GRID_HEIGHT ); 


	SelectObject( hdc, m_pathPen );
	SelectObject( hdc, m_pathBrush );

	int xPos, yPos;
	for( int i = 0; i < m_nPathStep - 1; i++ ){
		xPos = m_vPath[i].x * GRID_WIDTH;
		yPos = m_vPath[i].y * GRID_HEIGHT;
		Ellipse( hdc, xPos, yPos, xPos+GRID_WIDTH, yPos+GRID_HEIGHT ); 
	}

	SelectObject( hdc, m_oldPen );
	SelectObject( hdc, m_oldBrush );

	return 0;
}

void GameObject::SetDestination( int ex, int ey )
{
	m_nPathStep = 0;

	m_xDesPos = ex;
	m_yDesPos = ey;
	m_pathFinder.SetBeginPoint( m_xPos, m_yPos );
	m_pathFinder.SetEndPoint( m_xDesPos, m_yDesPos );
	//PerformaceTimer perfTimer;
	//perfTimer.Start();
	PathFinder::PATHRESULT ret = m_pathFinder.FindPath( m_vPath );
	//perfTimer.End();
	//double dTimeElapsed = perfTimer.GetElapseTime();
	//char strTime[100];
	//sprintf( strTime, "花费时间%lfs", dTimeElapsed );
	//MessageBox( NULL, strTime, "error", 0 );
	if( ret == PathFinder::CANNOT_FINDPATH ){
		MessageBox( NULL, "Cannot get there	", "Info", 0 );
	}
}
