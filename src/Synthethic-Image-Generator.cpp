// Synthethic-Image-Generator.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "Math/Random.h"
#include "Generators/ImageGenerator.h"
#include "imageenhancer.h"
#include <iostream>
#include <filesystem>
#include <string>
#include <chrono>
#include <opencv2/opencv.hpp>
#include "utils.h"

using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;


// paths must outlive all even compose and stuff or be copied
const std::string& next(std::vector<std::string>& paths, int& iter)
{
	if (iter == paths.size() - 1)
	{
		simG::Random::shuffleList(paths);
		iter = -1;
	}

	++iter;
	return paths[iter];
}


#include <iostream>

int main()
{
	simG::ImageGenerator::AugmentationParams params;



	//simG::ImageGenerator ImgaGenerator("stuff", "stuff", "stuff", 3000, params);

	//simG::AbstractGenerator* generator = new simG::ImageGenerator("stuff", "stuff", "stuff", 3000, params);






	auto start = std::chrono::high_resolution_clock::now();
	

	auto end = std::chrono::high_resolution_clock::now();
	duration<double, std::milli> ms_double = end - start;
	std::cout << ms_double.count() << "ms\n";


}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
