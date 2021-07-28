#include "Random.h"

namespace simG {
	const unsigned int Random::seed{ std::random_device{}() };
	std::mt19937 Random::MTEngine_{ seed };

	int Random::uniformInt(int min, int max)
	{
		return std::uniform_int_distribution<int>{min, max}(MTEngine_);
	}

	double Random::uniformDouble(double min, double max)
	{
		return std::uniform_real_distribution<double>{min, max}(MTEngine_);
	}

	bool Random::randomProb(double probability)
	{
		return uniformDouble(0, 1.0) < probability;
	}

	int ThreadSafeRandom::uniformInt(int min, int max)
	{
		static thread_local auto seed = std::random_device{}();
		static thread_local std::mt19937 gen(seed);
		std::uniform_int_distribution<int> dist(min, max);

		return dist(gen);
	}

	double ThreadSafeRandom::uniformDouble(double min, double max)
	{
		static thread_local auto seed = std::random_device{}();
		static thread_local std::mt19937 gen(seed);
		std::uniform_real_distribution<double> dist(min, max);

		return dist(gen);
	}

	bool ThreadSafeRandom::randomProb(double probability)
	{
		return uniformDouble(0, 1.0) < probability;
	}
}

// #ToDo thread local
//#include <random>
//float randFloat(float low, float high) {
//	thread_local static std::random_device rd;
//	thread_local static std::mt19937 rng(rd());
//	thread_local std::uniform_real_distribution<float> urd;
//	return urd(rng, decltype(urd)::param_type{ low,high });
//}