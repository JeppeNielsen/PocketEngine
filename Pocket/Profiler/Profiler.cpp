#include "Profiler.hpp"
#include <sstream>
#include <iostream>

using namespace Pocket;

Profiler::Profiler() {
	fps = 0;
	time = 0;
	count = 0;
}

Profiler::~Profiler() {
}

void Profiler::Begin() {
	timer.Begin();
}

double Profiler::End(std::string name) {
	double duration = timer.End();

	double fps = 1.0 / duration;

	fpsList.push_back(fps);
	while (fpsList.size()>60) {
		fpsList.pop_front();
	}

	double avg = 0;
	for(FpsList::iterator it = fpsList.begin(); it!=fpsList.end(); ++it) {
		avg += (*it);
	}
	avg *= (1.0 / fpsList.size());

	std::stringstream s;
	s<<name<<": "<<duration<<" sec \n"<<" fps: "<<avg;

	std::cout << s.str().c_str() << std::endl;
	return duration;
	/*


	time += t;
	count++;

	if (count>30) {

		LARGE_INTEGER li;
		if(!QueryPerformanceFrequency(&li)) {
			return 0;
		}

		double freq = double(li.QuadPart);
		double duration = (time / count) / freq;

		fps = 1.0f / duration;
		count = 0;
		time = 0;
	}

	return fps;
	*/
}

