// Synthethic-Image-Generator.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#pragma once
#include "ImageGenerator2D.h"
//#include "annotator.h"
//#include "imageenhancer.h"
#include <iostream>
#include "Utils/Random.h"
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

// paths must outlive all even compose and stuff or be copied
//
class Test
{
public:
	Test(const std::string& path) : d(path) {}

	simG::Directory d;
};
template<typename T>
struct Range
{
	T lower;
	T upper;
};








//	cv::Size src_size = src.size();
//	cv::Mat dest;
//	cv::Point2f rotation_cntr((src.cols-1) / 2., (src.rows-1) / 2.);
//	cv::Mat rotation_mat = cv::getRotationMatrix2D(rotation_cntr, angle, 1.0);
//
//	auto abs_cos = abs(rotation_mat.at<int>(0, 0));
//	auto abs_sin = abs(rotation_mat.at<int>(0, 1));
//
//	auto bound_w = int(src_size.height * abs_sin + src_size.width * abs_cos);
//	auto bound_h = int(src_size.height * abs_cos + src_size.width * abs_sin);
//
//	rotation_mat.at<int>(0, 2) += bound_w / 2 - ((src.cols - 1) / 2.);
//	rotation_mat.at<int>(1, 2) += bound_h / 2 - ((src.rows - 1) / 2.);
//
//
//
//
//	cv::warpAffine(src, dest, rotation_mat, cv::Size(bound_w, bound_h));
//	return dest;
//}

int main()
{
	//
	//
	//	//std::unique_ptr<simG::AbstractGenerator> generator = std::make_unique<simG::ImageGenerator2D>("stuff", "stuff", 3000, params);
	//
	//	//auto iter = std::filesystem::directory_iterator(R"(C:\Users\ruwen\Desktop\iav_Werkstudent\Dataset\Datasat_keep_aspect\train\imags)");
	//	//for (const auto& entry : std::filesystem::directory_iterator(R"(C:\Users\ruwen\Desktop\iav_Werkstudent\Dataset\Datasat_keep_aspect\train\imags)"))
	//	//{
	//	//}
	//
	//	//bool b = std::filesystem::is_directory(R"(C:\Users\ruwen\Desktop\iav_Werkstudent\Dataset\Datasat_keep_aspect\train\images)");
	//
	//	simG::Directory OutputDir("../Synthethic-Image-Generator");
	//
	simG::ImageGenerator2D::AugmentationParams params;


	simG::ImageGenerator2D generator(R"(C:\Users\ruwen\Desktop\SyntheticDataGenerator_Bachelor\Dataset\input\templates\transportation\car)",
		R"(C:\Users\ruwen\Desktop\Learning_CPP\Synthethic-Image-Generator\Test)", 500, 5, params);

	cv::Mat img = cv::imread(R"(C:\Users\ruwen\Desktop\Learning_CPP\Synthethic-Image-Generator\Test\t_img_klein0.png)", cv::IMREAD_COLOR);

	auto start = std::chrono::high_resolution_clock::now();

	//simG::print(r.upper);

	//simG::ImageEnhancer enhancer;

	//img = enhancer.rotate(img, 90);


	
	//simG::print(img.size());
	auto result = generator.generate();

	//cv::imshow("Windows", result);
	//cv::waitKey(0);

	//while (!ImgaGenerator.has_finished())
	//{
	//	ImgaGenerator.generate();
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