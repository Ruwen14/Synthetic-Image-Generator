// Synthethic-Image-Generator.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#pragma once
#include "imggenerator2d.h"
#include "annotators/CocoISAnnotator.h"
//#include "annotator.h"
//#include "imageenhancer.h"
#include <iostream>
#include "utils/Random.h"
//#include <filesystem>
//#include <string>
#include <chrono>
#include <opencv2/opencv.hpp>
//#include "Utils/Directory.h"
//#include "Utils/Random.h"
#include "Exceptions.h"

using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;


void batchResize()
{
	simG::Directory InputDir("./src");
	for (const auto& file : InputDir.entries)
	{
		std::string file_path = InputDir.relativeFilePath(file);
	}
}


enum class GeneratorInit { SUCCESS = 0, FAILED = 1, UNKNOWN_ERROR = 2 };


GeneratorInit doStuff()
{
	return GeneratorInit::SUCCESS;
}

int main()
{
	simG::ImageGenerator2D::AugmentationParams params;

	// Defaults
	//params.randomnoise = false;
	//params.BackgroundAugs.axisflipping.do_random_flip = true;
	//params.BackgroundAugs.axisflipping.include_no_flip = true;
	//params.BackgroundAugs.brightness.do_shift = true;
	//params.BackgroundAugs.brightness.brightness_range = { -20.5, 20.5 };
	//params.BackgroundAugs.crop.do_crop = true;
	//params.BackgroundAugs.crop.target_dim = { 1024, 576 };
	//params.BackgroundAugs.crop.keep_aspect = true;

	//params.MaskAugs.rotation.do_rotate = true;
	//params.MaskAugs.rotation.rotation_range = { 0, 360 };
	//params.MaskAugs.brightness.do_shift = true;
	//params.MaskAugs.brightness.brightness_range = { -20.5, 20.5 };

	simG::ImageGenerator2D generator(R"(C:\Users\ruwen\Desktop\SyntheticDataGenerator_Bachelor\Dataset\input\templates\transportation\car)",
		R"(C:\Users\ruwen\Desktop\Learning_CPP\Synthethic-Image-Generator\Test)", 500, 5, params);

	cv::Mat img = cv::imread(R"(C:\Users\ruwen\Desktop\Learning_CPP\Synthethic-Image-Generator\Test\background0.jpg)", cv::IMREAD_COLOR);

	simG::ImageAugmenter augmenter;


	if (doStuff() == GeneratorInit::SUCCESS)
	{
		simG::print("ja");
	}

	//auto result = generator.generate();
	simG::Directory D("./src");

	auto start = std::chrono::high_resolution_clock::now();

	//auto b = D.absoluteFilePath("ja");
	
	//cv::resize(img, img, cv::Size(1024, 576), 0, 0, cv::INTER_LINEAR);

	//cv::imshow("Windows", img);
	//cv::waitKey(0);

	//auto result = generator.generate();

	//while (!generator.hasFinished())
	//{
	//	generator.generate();
	//	//simG::print(ImgaGenerator.image_count);
	//}

	//try
	//{
	//	simG::ImageGenerator2D ImgaGenerator("Stuff", "Stuff", 3000, 5,params);
	//}
	//catch (const std::exception& e)
	//{
	//	std::cerr << e.what() << "\n";
	//}

	auto end = std::chrono::high_resolution_clock::now();
	duration<double, std::milli> ms_double = end - start;
	std::cout << ms_double.count() << "ms\n";

	//if (!ImgaGenerator.is_generator_valid())
	//{
	//	exit(1);
	//}

	//while (!ImgaGenerator.has_finished())
	//{
	//	ImgaGenerator.generate();
	//	ImgaGenerator.image_count;
	//}




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