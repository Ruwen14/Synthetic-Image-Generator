#pragma once
#include <iostream>
#include <vector>
#include <string>

namespace simG
{
	template<typename T>
	std::vector<T> getResizedVec(const std::vector<T>& srcVec, const std::size_t tSize)
	{
		std::vector<T> dstVec;
		dstVec = srcVec;

		if (tSize == srcVec.size())
		{
			return dstVec;
		}

		if (tSize < srcVec.size())
		{
			dstVec.resize(tSize);
			return dstVec;
		}

		dstVec.reserve(tSize);
		while (dstVec.size() < tSize)
		{
			if ((dstVec.size() + srcVec.size()) <= tSize)
			{
				dstVec.insert(dstVec.end(), srcVec.begin(), srcVec.end());
			}
			else
			{
				std::vector<T> slicedVec = std::vector<T>(
					srcVec.begin(),
					srcVec.begin() + (tSize - dstVec.size())
					);
				dstVec.insert(dstVec.end(), slicedVec.begin(), slicedVec.end());
			}
		}

		return dstVec;
	}
}

template <typename T>
std::ostream& operator<< (std::ostream& out, const std::vector<T>& v) {
	out << "[";
	for (const auto& ele : v)
	{
		out << ele << ", ";
	}
	out << "\b\b]";
	return out;
}