#pragma once
#include <vector>
#include <string>
#include "Math/Random.h"

namespace simG
{
	std::vector<std::string> listdir(const std::string& dir);

	bool ispath(const std::string& path);

	template<typename dyT>
	const dyT& infinite_next(std::vector<dyT>& vec, int& iter)
	{
		if (iter == vec.size() - 1)
		{
			iter = -1;
			// No const ref, because we need to shuffle.
			Random::shuffleList(vec);
		}
		++iter;
		return vec[iter];
	}
}




