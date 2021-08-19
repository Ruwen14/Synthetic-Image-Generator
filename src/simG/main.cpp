// Synthethic-Image-Generator.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "../simG.h"
#include "utils/Timer.h"
#include "core/algorithms.h"

#include "../../external/rapidjson/document.h"
#include "../../external/rapidjson/writer.h"
#include "../../external/rapidjson/prettywriter.h"

#include "../../external/rapidjson/stringbuffer.h"
#include "../../external/rapidjson/filereadstream.h"
#include <../../external/rapidjson/istreamwrapper.h>
#include "utils/datasetviz.h"

#include <iostream>
#include <chrono>
#include <variant>
#include <cstdio>
#include <optional>

#include <ctime>

using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;

using simG::print;
//#ifndef SIMG_EXPORT
//# if (defined _WIN32 || defined WINCE || defined __CYGWIN__)
//#   define SIMG_EXPORT __declspec(dllexport)
//# elif defined __GNUC__ && __GNUC__ >= 4 && (defined(CVAPI_EXPORTS) || defined(__APPLE__))
//#   define SIMG_EXPORT __attribute__ ((visibility ("default")))
//# endif
//#endif

// From https://github.com/TheCherno/Hazel/blob/master/Hazel/src/Hazel/Core/PlatformDetection.h
// Platform detection using predefined macros
//#ifdef _WIN32
//	/* Windows x64/x86 */
//#ifdef _WIN64
//	/* Windows x64  */
//#define HZ_PLATFORM_WINDOWS
//#else
//	/* Windows x86 */
//#error "x86 Builds are not supported!"
//#endif
//#elif defined(__APPLE__) || defined(__MACH__)
//#include <TargetConditionals.h>
///* TARGET_OS_MAC exists on all the platforms
// * so we must check all of them (in this order)
// * to ensure that we're running on MAC
// * and not some other Apple platform */
//#if TARGET_IPHONE_SIMULATOR == 1
//#error "IOS simulator is not supported!"
//#elif TARGET_OS_IPHONE == 1
//#define HZ_PLATFORM_IOS
//#error "IOS is not supported!"
//#elif TARGET_OS_MAC == 1
//#define HZ_PLATFORM_MACOS
//#error "MacOS is not supported!"
//#else
//#error "Unknown Apple platform!"
//#endif
// /* We also have to check __ANDROID__ before __linux__
//  * since android is based on the linux kernel
//  * it has __linux__ defined */
//#elif defined(__ANDROID__)
//#define HZ_PLATFORM_ANDROID
//#error "Android is not supported!"
//#elif defined(__linux__)
//#define HZ_PLATFORM_LINUX
//#error "Linux is not supported!"
//#else
//	/* Unknown compiler/platform */
//#error "Unknown platform!"
//#endif // End of platform detection

namespace sa = simG::annotators;


void checkSpeed()
{

	simG::viz::COCOVizualizer coco(R"(C:\Users\ruwen\Desktop\iav_Werkstudent\Dataset\Datasat_keep_aspect\train\synthetic_train_annotations.json)");

	std::vector<double> testVector{ 634.97, 417.61, 632.15, 413.01, 627.55, 409.12, 622.6, 406.65, 616.95, 404.53, 611.29, 403.82, 605.28, 403.82, 599.63, 404.18, 593.62, 405.59, 587.61, 406.65, 581.25, 407.71, 575.95, 408.42, 570.65, 409.48, 564.99, 409.48, 559.34, 409.48, 553.68, 407.71, 548.73, 405.59, 543.08, 404.18, 539.54, 408.42, 537.07, 413.37, 531.42, 413.37, 525.05, 414.07, 519.4, 414.07, 513.74, 414.07, 507.73, 414.43, 501.37, 414.43, 495.72, 414.78, 489.36, 414.78, 482.64, 414.78, 477.34, 415.13, 472.04, 416.19, 466.38, 416.9, 460.73, 417.61, 454.72, 417.96, 449.42, 418.31, 444.11, 419.73, 438.81, 420.43, 433.51, 421.85, 425.74, 423.97, 421.49, 427.86, 417.25, 435.28, 416.55, 443.76, 417.61, 455.78, 417.61, 467.44, 417.96, 477.34, 420.43, 489.0, 422.91, 499.61, 425.74, 508.79, 429.62, 514.1, 437.05, 517.98, 445.18, 519.4, 465.32, 520.81, 472.39, 520.81, 478.05, 521.17, 484.41, 521.17, 491.12, 520.81, 497.13, 520.81, 503.49, 520.46, 508.79, 520.11, 514.45, 519.04, 520.11, 518.69, 526.47, 517.63, 531.77, 517.28, 537.07, 516.92, 542.73, 516.57, 555.1, 515.51, 551.56, 520.11, 549.79, 523.29, 549.79, 525.05, 555.1, 525.76, 557.92, 523.29, 559.69, 519.04, 561.46, 516.57, 572.06, 514.45, 592.21, 515.16, 604.93, 513.39, 621.54, 510.92, 628.61, 508.09, 636.74, 499.61, 639.57, 492.54, 641.34, 481.58, 641.34, 465.67, 640.63, 451.54, 638.15, 433.16,
	634.97, 417.61, 632.15, 413.01, 627.55, 409.12, 622.6, 406.65, 616.95, 404.53, 611.29, 403.82, 605.28, 403.82, 599.63, 404.18, 593.62, 405.59, 587.61, 406.65, 581.25, 407.71, 575.95, 408.42, 570.65, 409.48, 564.99, 409.48, 559.34, 409.48, 553.68, 407.71, 548.73, 405.59, 543.08, 404.18, 539.54, 408.42, 537.07, 413.37, 531.42, 413.37, 525.05, 414.07, 519.4, 414.07, 513.74, 414.07, 507.73, 414.43, 501.37, 414.43, 495.72, 414.78, 489.36, 414.78, 482.64, 414.78, 477.34, 415.13, 472.04, 416.19, 466.38, 416.9, 460.73, 417.61, 454.72, 417.96, 449.42, 418.31, 444.11, 419.73, 438.81, 420.43, 433.51, 421.85, 425.74, 423.97, 421.49, 427.86, 417.25, 435.28, 416.55, 443.76, 417.61, 455.78, 417.61, 467.44, 417.96, 477.34, 420.43, 489.0, 422.91, 499.61, 425.74, 508.79, 429.62, 514.1, 437.05, 517.98, 445.18, 519.4, 465.32, 520.81, 472.39, 520.81, 478.05, 521.17, 484.41, 521.17, 491.12, 520.81, 497.13, 520.81, 503.49, 520.46, 508.79, 520.11, 514.45, 519.04, 520.11, 518.69, 526.47, 517.63, 531.77, 517.28, 537.07, 516.92, 542.73, 516.57, 555.1, 515.51, 551.56, 520.11, 549.79, 523.29, 549.79, 525.05, 555.1, 525.76, 557.92, 523.29, 559.69, 519.04, 561.46, 516.57, 572.06, 514.45, 592.21, 515.16, 604.93, 513.39, 621.54, 510.92, 628.61, 508.09, 636.74, 499.61, 639.57, 492.54, 641.34, 481.58, 641.34, 465.67, 640.63, 451.54, 638.15, 433.16,
	634.97, 417.61, 632.15, 413.01, 627.55, 409.12, 622.6, 406.65, 616.95, 404.53, 611.29, 403.82, 605.28, 403.82, 599.63, 404.18, 593.62, 405.59, 587.61, 406.65, 581.25, 407.71, 575.95, 408.42, 570.65, 409.48, 564.99, 409.48, 559.34, 409.48, 553.68, 407.71, 548.73, 405.59, 543.08, 404.18, 539.54, 408.42, 537.07, 413.37, 531.42, 413.37, 525.05, 414.07, 519.4, 414.07, 513.74, 414.07, 507.73, 414.43, 501.37, 414.43, 495.72, 414.78, 489.36, 414.78, 482.64, 414.78, 477.34, 415.13, 472.04, 416.19, 466.38, 416.9, 460.73, 417.61, 454.72, 417.96, 449.42, 418.31, 444.11, 419.73, 438.81, 420.43, 433.51, 421.85, 425.74, 423.97, 421.49, 427.86, 417.25, 435.28, 416.55, 443.76, 417.61, 455.78, 417.61, 467.44, 417.96, 477.34, 420.43, 489.0, 422.91, 499.61, 425.74, 508.79, 429.62, 514.1, 437.05, 517.98, 445.18, 519.4, 465.32, 520.81, 472.39, 520.81, 478.05, 521.17, 484.41, 521.17, 491.12, 520.81, 497.13, 520.81, 503.49, 520.46, 508.79, 520.11, 514.45, 519.04, 520.11, 518.69, 526.47, 517.63, 531.77, 517.28, 537.07, 516.92, 542.73, 516.57, 555.1, 515.51, 551.56, 520.11, 549.79, 523.29, 549.79, 525.05, 555.1, 525.76, 557.92, 523.29, 559.69, 519.04, 561.46, 516.57, 572.06, 514.45, 592.21, 515.16, 604.93, 513.39, 621.54, 510.92, 628.61, 508.09, 636.74, 499.61, 639.57, 492.54, 641.34, 481.58, 641.34, 465.67, 640.63, 451.54, 638.15, 433.16,
	634.97, 417.61, 632.15, 413.01, 627.55, 409.12, 622.6, 406.65, 616.95, 404.53, 611.29, 403.82, 605.28, 403.82, 599.63, 404.18, 593.62, 405.59, 587.61, 406.65, 581.25, 407.71, 575.95, 408.42, 570.65, 409.48, 564.99, 409.48, 559.34, 409.48, 553.68, 407.71, 548.73, 405.59, 543.08, 404.18, 539.54, 408.42, 537.07, 413.37, 531.42, 413.37, 525.05, 414.07, 519.4, 414.07, 513.74, 414.07, 507.73, 414.43, 501.37, 414.43, 495.72, 414.78, 489.36, 414.78, 482.64, 414.78, 477.34, 415.13, 472.04, 416.19, 466.38, 416.9, 460.73, 417.61, 454.72, 417.96, 449.42, 418.31, 444.11, 419.73, 438.81, 420.43, 433.51, 421.85, 425.74, 423.97, 421.49, 427.86, 417.25, 435.28, 416.55, 443.76, 417.61, 455.78, 417.61, 467.44, 417.96, 477.34, 420.43, 489.0, 422.91, 499.61, 425.74, 508.79, 429.62, 514.1, 437.05, 517.98, 445.18, 519.4, 465.32, 520.81, 472.39, 520.81, 478.05, 521.17, 484.41, 521.17, 491.12, 520.81, 497.13, 520.81, 503.49, 520.46, 508.79, 520.11, 514.45, 519.04, 520.11, 518.69, 526.47, 517.63, 531.77, 517.28, 537.07, 516.92, 542.73, 516.57, 555.1, 515.51, 551.56, 520.11, 549.79, 523.29, 549.79, 525.05, 555.1, 525.76, 557.92, 523.29, 559.69, 519.04, 561.46, 516.57, 572.06, 514.45, 592.21, 515.16, 604.93, 513.39, 621.54, 510.92, 628.61, 508.09, 636.74, 499.61, 639.57, 492.54, 641.34, 481.58, 641.34, 465.67, 640.63, 451.54, 638.15, 433.16 };

	print(testVector.size());

	std::vector<cv::Point> dstCntr;
	auto srcCntr = coco.translateListToContour(testVector);
	print(srcCntr.size());
	auto start = std::chrono::high_resolution_clock::now();
	simG::algorithms::simplifyContour(srcCntr, dstCntr, 0.00003);
	auto end = std::chrono::high_resolution_clock::now();
	duration<double, std::milli> ms_double = end - start;
	std::cout << ms_double.count() << "ms\n";
	print(dstCntr.size());
}


int main()
{	
	simG::viz::COCOVizualizer coco(R"(C:\Users\ruwen\Desktop\iav_Werkstudent\Dataset\Datasat_keep_aspect\train\synthetic_train_annotations.json)");

	auto st = simG::Timer::startTimer();

	simG::Directory imgDir(R"(C:\Users\ruwen\Desktop\iav_Werkstudent\Dataset\Datasat_keep_aspect\train\images)");
	for (const auto& path : imgDir.entries)
	{
		auto result = coco.showAnnotations(imgDir.absoluteFilePath(path));
		cv::imshow("Current annotation", result);
		cv::waitKey(0);
	}

	simG::Timer::timeElapsed(st);

	exit(0);
	sa::COCOAnnotator annotator("hi", sa::COCOAnnotator::KEYPOINTS);
	//std::variant<int, simG::ImageGenerator> vec;
	// TODO: Use RapidXML for parsing XML; it's header-only and pretty fast. But PugiXML is more convenient and still fast

	//TEST_RAPIDJSON();
	//yoloDetect();
	// ToDo translate Contours https://medium.com/analytics-vidhya/tutorial-how-to-scale-and-rotate-contours-in-opencv-using-python-f48be59c35a2
	// And https://github.com/danielthank/jetson-robot/blob/0db11c6d63df76bbc0a35439181ae3e83351a650/alphabet/letter_detector.py
	//test_overlay();

	//atuo back = Mat::zeros(Size(image.cols, image.rows), CV_8UC1);

	//config_.RandomRotation({ 0, 360 });
	//config_.RandomCrop({ 1024, 576 }, true);
	//config_.RandomNoise(0.0, 10.0);
	//config_.RandomHorizontalFlip(0.5);
	//config_.RandomVerticalFlip(0.5);
	//config_.RandomBrightnessShift({ -20.5, 20.5 });
	simG::ImageGenerator::AugmentationParams params;
	//simG::TransformsList transforms{
	//	AddTransform<simG::RandomBrightness>({-20.5, 20.5})
	//};

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

	//simG::transforms::Sequential maskTransforms; //<- Random rotation; radomscale flips, noise
	//simG::transforms::Sequential BckgrTransforms; <-resize, noise, crop; check that random scale is not smaller than resize
	//simG::transforms::Sequential BckgrTransforms;
	//simG::Directory maskDir;
	//simG::Directory maskDir;

	//ToDO add Placement Ruleset
	simG::ImageGenerator generator(R"(C:\Users\ruwen\Desktop\iav_Werkstudent\Dataset\Datasat_keep_aspect\train\images)",
		R"(C:\Users\ruwen\Desktop\Learning_CPP\Synthethic-Image-Generator\Test)", params);
	//MultithreadGenerator mGen(R"(C:\Users\ruwen\Desktop\SyntheticDataGenerator_Bachelor\Dataset\input\templates\transportation\car)");

	auto test_img = cv::imread(R"(C:\Users\ruwen\Desktop\Learning_CPP\Synthethic-Image-Generator\resources\t_00000001.jpg)");

	auto start = std::chrono::high_resolution_clock::now();
	//
	//simG::ImageAugmenter augmenter;
	//auto start = std::chrono::high_resolution_clock::now();

	//overlayImage(back, back, { 0,0 });
	//cv::imshow("", back);
	//cv::waitKey(0);
	//
	// ToDo: check out https://github.com/jbohnslav/opencv_transforms/blob/master/opencv_transforms/transforms.py
	//simG::transforms::RandomRotation layer({ 60,60 });
	//simG::transforms::Sequential transforms({
	//	simG::transforms::RandomScale({1.5, 1.5}),
	//	simG::transforms::RandomCrop({550, 100}, true),
	//	simG::transforms::Resize({500, 128}, false),
	//	simG::transforms::RandomBrightness({-150, -150}),
	//	simG::transforms::GaussianBlur(1.0),
	//	//	//simG::transforms::GaussianBlur(1.0),
	//	//	//simG::transforms::RandomGaussNoise(1.0),
	//	//	//simG::transforms::RandomVerticalFlip(1.0),
	//	//	//simG::transforms::RandomHorizontalFlip(1.0),
	//	//	////put annotator as transform like in https://github.com/LinkedAi/flip

	//	// //  //simG::transforms::RandomRotation90(1.0),
	//	// //  //simG::transforms::RandomRotation180(1.0),
	//	// //  //simG::transforms::RandomRotation270(1.0),
	//	// //  //simG::transforms::RandomRotation({30, 30})
	//	});

	simG::transforms::Sequential2 trans({
		//simG::transforms::RandomScale({1.5, 1.5}),
		simG::transforms::RandomBrightness({-150, -150}),

		simG::transforms::RandomCrop({550, 100}, true),
		simG::transforms::Resize({500, 128}, true),
		//simG::transforms::GaussianBlur(1.0),
		//simG::transforms::RandomGaussNoise(1.0),
		//simG::transforms::RandomVerticalFlip(1.0),
		//simG::transforms::RandomHorizontalFlip(1.0),
		////put annotator as transform like in https://github.com/LinkedAi/flip

	   //simG::transforms::RandomRotation90(1.0),
	   //simG::transforms::RandomRotation180(1.0),
	   //simG::transforms::RandomRotation270(1.0),
	   //simG::transforms::RandomRotation({30, 30})
		});

	//cv::Mat dst;
	////trans.apply(test_img, dst);
	////trans.add(rb);
	//trans.measureTransforms(test_img, dst);
	//simG::print(dst.size());

	//transforms.apply(test_img.clone(), dst2);
	//cv::imshow("Original", test_img);

	//cv::imshow("dst1", dst);
	//cv::imshow("dst2", dst2);

	//cv::waitKey(0);
	//std::vector<std::variant<simG::transforms::RandomRotation>> vars{
	//	//simG::transforms::RandomRotation({60,60})
	//};

	//auto c = transforms;

	//simG::transforms::Sequential maskTransforms({
	//	//simG::transforms::RandomScale({0.5, 1.5}),
	//	// simG::transforms::RandomCrop({550, 100}, true),
	//	//simG::transforms::Resize({600, 400}, true),
	//	//simG::transforms::RandomBrightness({-50, 50}),
	//	//simG::transforms::GaussianBlur(1.0),
	//	});

	//generator.setThreading(simG::ThreadingStatus::ADJUST_TO_CPU);
	//simG::print(generator.isThreadingEnabled());
	//generator.addTransforms(transforms, simG::APPLY_ON_BCKGROUND);

	//simG::print(generator.bckgrTransforms_.count());

	//auto b = static_cast<simG::transforms::RandomScale>(generator.m_ImageTransformations[0].first.transforms_[0]);

	//simG::print(generator.m_ImageTransformations[0].first.transforms_[0]->getType());

	//generator.addTransforms(transforms, simG::APPLY_ON_MASK);

	//std::vector<std::pair<simG::transforms::Sequential, simG::TransformTarget>> transformsCont;
	//transformsCont.emplace_back(transforms, simG::APPLY_ON_BCKGROUND);

	//transformss.emplace_back(transforms, simG::APPLY_ON_RESULT);

	//for (const auto& tr :imageTransforms)
	//{
	//	if (tr.target == simG::APPLY_ON_BCKGROUND)
	//	{
	//		simG::print("Ja");
	//	}

	//cv::Mat dst;
	//transforms.apply(test_img, dst);
	//cv::imshow("", dst);
	//cv::waitKey(0);

	//simG::Directory dir(R"(C:\Users\ruwen\Desktop\iav_Werkstudent\Dataset\Datasat_keep_aspect\train\images)");

//	std::vector<cv::Mat> img_array;
//	img_array.reserve(10);
////
//////#pragma omp parallel for num_threads(4)
//	for (int i = 0; i < 1; i++)
//	{
//		//std::cout << omp_get_thread_num() << "\n";
//		cv::Mat dst;
//		cv::Mat src;
//		test_img.copyTo(src);
//		transforms.apply(src, dst);
//		img_array.push_back(dst);
//	}
	//simG::subplot(img_array, 4);

	//generator.setThreading(simG::ThreadingStatus::ADJUST_TO_CPU);
	//auto dst = rbright(test_img);

	//mGen.generateThreaded();

	//auto img = cv::imread("./resources/t_00000001.jpg");

	//#pragma omp parallel for num_threads(4)
	//	for (int i = 0; i < 1000; i++)
	//	{
	//		std::cout << omp_get_thread_num() << "\n";
	//	}
		//simG::print(generator.num_workers_);

		//simG::print(cache);
		//simG::print(c);

		//simG::Directory testDir(R"(C:\Users\ruwen\Desktop\SyntheticDataGenerator_Bachelor\Dataset\input\templates\transportation\car)");
		//for (int i = 0; i < v.size(); i++)
		//{
		////	std::cout << "Without Thread" << b[i] << "\n";
		////	std::cout << "With    Thread" << v[i] << "\n";

		//}

		//simG::print(omp_get_num_procs());

		//mGen.generateThreaded();

		//simG::Directory input_dir(R"(C:\Users\ruwen\Desktop\SyntheticDataGenerator_Bachelor\Dataset\input\templates\transportation\car)");

		//load_images(input_dir);
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
	std::string warningText =
		"[WARNING]: Oops. It looks like you tried to enable threading, but forgot to set the '/openmp' flag during compilation. \n"
		"	   >> Threading was therefore disabled for the rest of the program. \n"
		"	   -- \n"
		"	   If you still want to use threading add '-openmp' to your compilation commands or \n"
		"	   select C/C.++->Language, and change 'OpenMP Support' to 'Yes' if using Visual Studio 2019. \n"
		"	   -- \n"
		"	   Otherwise do 'setThreading(simG::ThreadingStatus::DISABLE_THREADING)' to disable the warning. \n";
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