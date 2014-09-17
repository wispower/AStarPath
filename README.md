AStarPath
=========

A* pathfinding using c++, with win32 window.

This project implements A* algorithm using c++. It can be run in windows only because I use win32 GDI to draw graphics. Nevertheless, the algorithm and drawing are seperated well so you can use the algorithm easily with other GUI framework.

The map is created with a file at bin\2.map, you can edit the file to createa new map.

The openlist has two implements, ordered vector and binary heap. However, you must switch them in code at PathFinder.h.
	
	//typedef FFOpenList< PathNode > OpenList;
	typedef FFBinaryHeap< PathNode > OpenList;

Again, you can change the heuristic code using euclidean distance or manhattan distance at PathFinder.cpp.
	
	float heuristic = sqrt( (float)( xn - m_endPoint.x ) * ( xn - m_endPoint.x ) + ( yn - m_endPoint.y ) * ( yn - m_endPoint.y ) );
	//float heuristic = abs( xn - m_endPoint.x ) + abs( yn - m_endPoint.y );
    
In the window, double click left mouse button will lead the point move from original position to mouse's position along the algorithm's result.
