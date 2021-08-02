#include "Timer.h"
#include <iostream>

namespace simG
{
	namespace Timer
	{
		std::chrono::time_point<std::chrono::high_resolution_clock> startTimer()
		{
			return std::chrono::high_resolution_clock::now();
		}

		void timeElapsed(const std::chrono::time_point<std::chrono::high_resolution_clock>& measured)
		{
			auto end = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double, std::milli> ms_double = end - measured;
			std::cout << "[Finished in " << ms_double.count() << "ms]\n";
		}

		ScopeTimer::ScopeTimer(const std::string& scopename)
			: m_scopeName(scopename), m_start(std::chrono::high_resolution_clock::now())
		{
		}

		ScopeTimer::ScopeTimer()
			: ScopeTimer("Current Scope")
		{
		}

		ScopeTimer::~ScopeTimer()
		{
			auto end = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double, std::milli> ms_double = end - this->m_start;
			std::cout << "[" << m_scopeName << " took " << ms_double.count() << "ms]\n";
		}
	}
}