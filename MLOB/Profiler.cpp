#include "Profiler.h"

#include <iostream>

Profiler::Profiler()
{
	d_data.reserve(INITIAL_CAPACITY);
}

uint32_t Profiler::start(const std::string& funcName, uint32_t parentId)
{
	Profiler& profiler = Profiler::instance();
	profiler.d_data.emplace_back(ProfileData{ funcName, 0.0 ,std::chrono::steady_clock::now(), {}, parentId});
	profiler.d_eventCount++;
	return profiler.d_eventCount - 1;
}

void Profiler::stop(const uint32_t eventId)
{
	Profiler& profiler = Profiler::instance();
	auto& data = profiler.d_data[eventId];

	data.d_endTime = std::chrono::steady_clock::now();
	data.d_duration = std::chrono::duration<double, std::nano>(data.d_endTime - data.d_startTime).count();
}

void Profiler::report()
{
	Profiler& profiler = Profiler::instance();
	profiler.reportHelper(-1, 0);
}

void Profiler::reportHelper(uint32_t parentId, int depth)
{
	Profiler& profiler = Profiler::instance();
	for (const auto& data : profiler.d_data)
	{
		if (data.d_parentId == parentId)
		{
			std::cout << std::string(depth, '\t') << "Function: " << data.d_name
				<< ", Duration: " << data.d_duration << " ns\n";
			reportHelper(&data - &profiler.d_data[0], depth + 1); // Recurse for child functions
		}
	}
}


