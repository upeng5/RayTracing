#ifndef STOPWATCH_H
#define STOPWATCH_H

#include <chrono>
#include <string>
#include <iostream>

class StopWatch {
	public:
		void Begin();
		double Stop();
	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> start_time_;
};

void StopWatch::Begin() {
	start_time_ = std::chrono::high_resolution_clock::now();
}

double StopWatch::Stop() {
	auto end_time = std::chrono::high_resolution_clock::now();

	auto start = std::chrono::time_point_cast<std::chrono::seconds>(start_time_).time_since_epoch().count();
	auto end = std::chrono::time_point_cast<std::chrono::seconds>(end_time).time_since_epoch().count();

	double duration =  static_cast<double>(end - start);
	return duration;
}
#endif // !STOPWATCH_H
