#pragma once
#include "defs.h"
class Timer
{
public:
	static inline double now() {
		LARGE_INTEGER time, freq;
		if (!QueryPerformanceFrequency(&freq)) {
			//  Handle error
			return 0;
		}
		if (!QueryPerformanceCounter(&time)) {
			//  Handle error
			return 0;
		}
		return (double)time.QuadPart / freq.QuadPart;
	}
	static double cpu_now() {
		FILETIME a, b, c, d;
		if (GetProcessTimes(GetCurrentProcess(), &a, &b, &c, &d) != 0) {
			//  Returns total user time.
			//  Can be tweaked to include kernel times as well.
			return
				(double)(d.dwLowDateTime |
				((unsigned long long)d.dwHighDateTime << 32)) * 0.0000001;
		}
		else {
			//  Handle error
			return 0;
		}
	}

};

