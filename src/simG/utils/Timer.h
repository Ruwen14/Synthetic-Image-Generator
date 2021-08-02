#pragma once
#include <string>
#include <chrono>

//using std::chrono::duration_cast;
using std::chrono::duration;

namespace simG
{
	namespace Timer
	{
		std::chrono::time_point<std::chrono::high_resolution_clock> startTimer();
		void timeElapsed(const std::chrono::time_point<std::chrono::high_resolution_clock>& measured);

		class ScopeTimer
		{
		public:
			ScopeTimer(const std::string& scopename);
			ScopeTimer();
			~ScopeTimer();

		private:
			std::chrono::time_point<std::chrono::high_resolution_clock> m_start;
			std::string m_scopeName;
		};

		class Timer
		{
		public:
			Timer()
			{
				Reset();
			}

			void Reset()
			{
				m_Start = std::chrono::high_resolution_clock::now();
			}

			float Elapsed()
			{
				return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - m_Start).count() * 0.001f * 0.001f * 0.001f;
			}

			float ElapsedMillis()
			{
				return Elapsed() * 1000.0f;
			}

		private:
			std::chrono::time_point<std::chrono::high_resolution_clock> m_Start;
		};
	}
}
