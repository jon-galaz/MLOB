#pragma once

#include <string>
#include <vector>
#include <chrono>

struct ProfileData
{
	std::string d_name;
	double d_duration;
	std::chrono::steady_clock::time_point d_startTime;
	std::chrono::steady_clock::time_point d_endTime;
	uint32_t d_parentId;
};

class Profiler
{
public:
	static Profiler& instance()
	{
		static Profiler instance;
		return instance;
	}
	static uint32_t start(const std::string& funcName, uint32_t parentId = -1);
	static void stop(const uint32_t eventId);
	static void report();

	Profiler(const Profiler&) = delete;
	Profiler& operator=(const Profiler&) = delete;

private:
	Profiler();
	void reportHelper(uint32_t parentId, int depth);

	std::vector<ProfileData> d_data;
	static constexpr uint8_t INITIAL_CAPACITY = static_cast<uint8_t>(1000);
	uint32_t d_eventCount = 0;
};