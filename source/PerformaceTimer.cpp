#include "StdAfx.h"
#include "PerformaceTimer.h"


PerformaceTimer::PerformaceTimer(void) : 
	m_llStartTime( 0 ), m_llEndTime( 0 ), m_dTimeElapse( 0 )
{
	QueryPerformanceFrequency( (LARGE_INTEGER*) &m_llPerfCountFreq);
	m_dTimeElapse = 1.0 / m_llPerfCountFreq;
}


PerformaceTimer::~PerformaceTimer(void)
{
}

void PerformaceTimer::Start()
{
	QueryPerformanceCounter( (LARGE_INTEGER*) &m_llStartTime );
}

void PerformaceTimer::End()
{
	QueryPerformanceCounter( (LARGE_INTEGER*) &m_llEndTime );
}

double PerformaceTimer::GetElapseTime()
{
	return ( m_llEndTime - m_llStartTime ) * m_dTimeElapse;
}