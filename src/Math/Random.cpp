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
}