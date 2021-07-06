#pragma once
#include <vector>
#include <iterator>
#include<random>

namespace simG {
	// https://stackoverflow.com/questions/19036141/vary-range-of-uniform-int-distribution
	// Light Wrapper around mersenne-twister-engine do have an unbiased and non deterministic random-number-generetor. srand()/rand() to much bias
	// Uses C++17.
	// Internally dependent on Mersenne-Twister-Generator and std::random_device in <random>
	class Random
	{
	public:
		static const unsigned int seed;

		static int uniformInt(int min, int max);

		static double uniformDouble(double min, double max);

		static bool randomProb(double probability);

		template<typename T> static void shuffleList(std::vector<T>& list)
		{
			std::shuffle(list.begin(), list.end(), MTEngine_);
		}

		template<typename T> static std::vector<T> randomSamples(const std::vector<T>& vec, int number_samples = 1)
		{
			std::vector<T> samples;
			std::sample(vec.begin(), vec.end(), std::back_inserter(samples), number_samples, MTEngine_);
			return samples;
		}

	private:
		static std::mt19937 MTEngine_;
	};

	class ThreadSafeRandom
	{
	public:
		static int uniformInt(int min, int max);

		static double uniformDouble(double min, double max);

		static bool randomProb(double probability);

		// Lock list when shuffling, otherwise not threadsafe
		template<typename T> static void shuffleList(std::vector<T>& list)
		{
			static thread_local auto seed = std::random_device{}();
			static thread_local std::mt19937 gen(seed);
			std::shuffle(list.begin(), list.end(), gen);
		}

		template<typename T> static std::vector<T> randomSamples(const std::vector<T>& vec, int number_samples = 1)
		{
			static thread_local auto seed = std::random_device{}();
			static thread_local std::mt19937 gen(seed);

			std::vector<T> samples;
			std::sample(vec.begin(), vec.end(), std::back_inserter(samples), number_samples, gen);
			return samples;
		}


	};




}
