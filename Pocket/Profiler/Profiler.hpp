#pragma once
#include "Timer.hpp"
#include <stdlib.h>
#include <string>
#include <map>
#include <deque>

namespace Pocket {
	class Profiler
	{
	public:
		Profiler();
		~Profiler();

		void Begin();
		double End(std::string name);

	private:

        Timer timer;
        
		double fps;
		double time;
		int count;

		typedef std::deque<double> FpsList;
		FpsList fpsList;

	};
}
