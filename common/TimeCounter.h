/** \file
	\brief Utility class to measure execution time.

	Prints time elapsed from creation to destruction when destructed using
	OutputDebugString.
	Use as automatic object if scope is clearly defined or using TC_START/TC_STOP
	macros when adding additional scope would be inconvenient.
	Usage: TimeCounter tc("SomeName") or with supplied pair of macros.
*/

#include "Log.h"
#include <stdio.h>

/** \brief Class that reports it's lifetime when destructed
*/
class TimeCounter
{
public:
	/** \brief Constructor
		\param name string that will be visible in printed report
	*/
	TimeCounter(char* name):
		name(name)
	{
		QueryPerformanceFrequency(&freq);
		QueryPerformanceCounter(&beginTime);
	}

	~TimeCounter()
	{
		LARGE_INTEGER endTime;
		double nCalcTime;

		QueryPerformanceCounter(&endTime);
		nCalcTime = (double)(endTime.QuadPart - beginTime.QuadPart) *
			1000.0/(double)freq.QuadPart;
		char s[100];
		snprintf(s, sizeof(s), "%s  %.3f ms\n", name, nCalcTime);
		LOG(s);
	}

protected:
	LARGE_INTEGER freq;
	LARGE_INTEGER beginTime;

	char* name;
};

#define TS_START(id, name) TimeCounter* TC##id = new TimeCounter(name)
#define TS_STOP(id) delete TC##id
