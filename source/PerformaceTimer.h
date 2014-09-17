#pragma once
class PerformaceTimer
{
public:
	PerformaceTimer(void);
	~PerformaceTimer(void);

	void Start();
	void End();
	double GetElapseTime();
private:
	LONGLONG m_llPerfCountFreq;
	LONGLONG m_llStartTime;
	LONGLONG m_llEndTime;

	double m_dTimeScale;
	double m_dTimeElapse;
};

