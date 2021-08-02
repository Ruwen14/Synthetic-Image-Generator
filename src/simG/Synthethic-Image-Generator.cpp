// Synthethic-Image-Generator.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "../simG.h"
#include "utils/Timer.h"
#include "core/algorithms.h"

#include <iostream>
#include <chrono>
#include <variant>

using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;

//#ifndef SIMG_EXPORT
//# if (defined _WIN32 || defined WINCE || defined __CYGWIN__)
//#   define SIMG_EXPORT __declspec(dllexport)
//# elif defined __GNUC__ && __GNUC__ >= 4 && (defined(CVAPI_EXPORTS) || defined(__APPLE__))
//#   define SIMG_EXPORT __attribute__ ((visibility ("default")))
//# endif
//#endif

//void load_images(simG::Directory dir)
//{
//	std::vector<cv::Mat>img_lst;
//	std::mutex mtx;
//
//	int count = 0;
//
//#pragma omp parallel for
//	for (int i = 0; i < dir.entries.size(); i++)
//	{
//		simG::print(omp_get_num_threads());
//
//		cv::Mat img = cv::imread(dir.absoluteFilePath(dir.entries[i]));
//		++count;
//		//mtx.lock();
//		//img_lst.push_back(img);
//		//mtx.unlock();
//	}
//	simG::print(count);
//	//mtx.lock();
//	//std::cout << "INFO: Loaded " << img_lst.size() << std::endl;
//	//mtx.unlock();
//}
//
//void load_images_single(simG::Directory dir) {
//	std::vector<cv::Mat>img_lst;
//	std::mutex mtx;
//
//	int count = 0;
//
//	for (int i = 0; i < dir.entries.size(); i++)
//	{
//		cv::Mat img = cv::imread(dir.absoluteFilePath(dir.entries[i]));
//		++count;
//		//mtx.lock();
//		//img_lst.push_back(img);
//		//mtx.unlock();
//	}
//	simG::print(count);
//	//mtx.lock();
//	//std::cout << "INFO: Loaded " << img_lst.size() << std::endl;
//	//mtx.unlock();
//}
//
//class MultithreadGenerator
//{
//public:
//	MultithreadGenerator(const std::string& dirName) :m_Dir(dirName) { };
//	~MultithreadGenerator() = default;
//
//	void generate();
//	void generateThreaded(int workers = omp_get_num_procs());
//
//	int m_frameCount = 0;
//
//public:
//	simG::Directory m_Dir;
//};
//
//void MultithreadGenerator::generate()
//{
//	cv::Mat img;
//	for (int i = 0; i < this->m_Dir.entries.size(); i++)
//	{
//		img = cv::imread(m_Dir.absoluteFilePath(m_Dir.entries[i]));
//		++m_frameCount;
//	}
//}
//
//void MultithreadGenerator::generateThreaded(int workers /*= omp_get_num_procs()*/)
//{
//	cv::Mat img;
//#pragma omp parallel for num_threads(workers)
//	for (int i = 0; i < this->m_Dir.entries.size(); i++)
//	{
//		img = cv::imread(m_Dir.absoluteFilePath(m_Dir.entries[i]));
//
//#pragma omp critical
//		++m_frameCount;
//	}
//}
//

//struct CropSettings
//{
//	int b;
//};
//
//template<typename T>
//struct Range
//{
//	T lower;
//	T upper;
//};
//
//struct Dimensions
//{
//	int width;
//	int height;
//};
//
//enum class Status { ENABLED, DISABLED };
//enum class ApplyOn { TEMPLATE, BACKGROUND, TEMPLATE_AND_BACKGROUND };
//class AugmentationConfig
//{
//public:
//
//	AugmentationConfig() = default;
//	~AugmentationConfig() = default;
//	void RandomCrop(Dimensions targetDim, bool keepAspectRatio);
//	void RandomRotation(Range<int> degrees);
//	void RandomHorizontalFlip(double chance = 0.5);
//	void RandomVerticalFlip(double chance = 0.5);
//	void RandomBrightnessShift(Range<double> range);
//	void RandomColorJitter(double brightness, double hue);
//	void RandomNoise(double mean = 0.0, double stdDev = 10.0);
//	void RandomGaussianBlur(std::pair<int, int> kernelSize, std::pair<int, int> sigma);
//
//	void RandomApply(std::variant<int> transforms);
//
//	void Normalize();
//
//	//void RandomBrightnessBckgr(Range<double> shiftRange);
//	//void RandomBrightnessMask(Range<double> shiftRange);
//
//	//void RandomAxisFlipMask(bool includeNoFlip = true);
//	//void RandomAxisFlipBckgr(bool includeNoFlip = true);
//
//	void getConfigObject();
//
//	std::string dump();
//};
//
//class AugRandomCrop
//{
//public:
//	AugRandomCrop(int b) : b_(b) {}
//	~AugRandomCrop() = default;
//
//	void operator()(const std::string& out);
//
//private:
//	int b_;
//};
//
//void AugRandomCrop::operator()(const std::string& out)
//{
//	std::cout << out << b_ << "\n";
//}

cv::Mat overlay(const cv::Mat& mask, const cv::Mat& back)
{
	cv::Mat dst;
	cv::addWeighted(back, 0.4, mask, 0.1, 0, dst);

	return dst;
}

/**
 * @brief Draws a transparent image over a frame Mat.
 *
 * @param frame the frame where the transparent image will be drawn
 * @param transp the Mat image with transparency, read from a PNG image, with the IMREAD_UNCHANGED flag
 * @param xPos x position of the frame image where the image will start.
 * @param yPos y position of the frame image where the image will start.
 */
void drawTransparency(cv::Mat frame, cv::Mat transp, int xPos, int yPos) {
	cv::Mat mask;
	std::vector<cv::Mat> layers;

	cv::split(transp, layers); // seperate channels
	cv::Mat rgb[3] = { layers[0],layers[1],layers[2] };
	mask = layers[3]; // png's alpha channel used as mask
	cv::merge(rgb, 3, transp);  // put together the RGB channels, now transp insn't transparent
	transp.copyTo(frame.rowRange(yPos, yPos + transp.rows).colRange(xPos, xPos + transp.cols), mask);
}

void test()
{/*create two input arrays: matrices*/
	/*also populate matrices with zeros*/
	cv::Mat input1 = cv::Mat::zeros(cv::Size(400, 400), CV_8UC3);
	cv::Mat input2 = cv::Mat::zeros(cv::Size(400, 400), CV_8UC3);

	/*draw a circle on input1*/
	cv::circle(input1, cv::Point(200, 200), 100.0, cv::Scalar(0, 0, 255), 1, 8);

	/*display input1*/
	imshow("input1", input1);

	/*draw a circle on input2*/
	cv::circle(input2, cv::Point(150, 200), 100.0, cv::Scalar(255, 0, 0), 1, 8);

	/*display input2*/
	imshow("input2", input2);

	/*stores the output*/
	cv::Mat output;

	/*compute the bitwise and of input arrays*/
	/*and store them in output array*/
	bitwise_and(input1, input2, output);

	/*display the output*/
	imshow("bitwise_and", output);

	/*wait for the use to press any */
	/*key to exit frames*/
	cv::waitKey(0);

	/*return 0 to caller to indicate*/
	/*successful termination of program*/
}

void overlayImage(cv::Mat& background, const cv::Mat& mask, cv::Point2i location)
{
	//background.copyTo(output);

	auto mask_channels = mask.channels();
	auto background_channels = background.channels();

	// start at the row indicated by location, or at row 0 if location.y is negative.
	for (int y = std::max(location.y, 0); y < background.rows; ++y)
	{
		int fY = y - location.y; // because of the translation

		// we are done of we have processed all rows of the foreground image.
		if (fY >= mask.rows)
			break;

		// start at the column indicated by location,

		// or at column 0 if location.x is negative.
		for (int x = std::max(location.x, 0); x < background.cols; ++x)
		{
			int fX = x - location.x; // because of the translation.

			// we are done with this row if the column is outside of the foreground image.
			if (fX >= mask.cols)
				break;

			// determine the opacity of the foregrond pixel, using its fourth (alpha) channel.
			double opacity = (static_cast<double>(mask.data[fY * mask.step + fX * mask_channels + 3])) / 255.;

			// and now combine the background and foreground pixel, using the opacity,

			// but only if opacity > 0.
			for (int c = 0; opacity > 0 && c < background_channels; ++c)
			{
				unsigned char foregroundPx = mask.data[fY * mask.step + fX * mask_channels + c];
				unsigned char backgroundPx = background.data[y * background.step + x * background_channels + c];
				background.data[y * background.step + background_channels * x + c] = backgroundPx * (1. - opacity) + foregroundPx * opacity;
			}
		}
	}
}

struct Pos
{
	int x;
	int y;
};

Pos getRandomPos(const cv::Size& backSize, const cv::Size& maskSize)
{
	int max_x = backSize.width - maskSize.width;
	int max_y = backSize.height - maskSize.height;

	return { simG::Random::uniformInt(0, max_x), simG::Random::uniformInt(0, max_y) };
}

using CvContours = std::vector<std::vector<cv::Point>>;

void overlay1(const cv::Mat& mask, const cv::Size& backsize, CvContours& cntrs)
{
	cv::Mat tmp = cv::Mat::zeros(cv::Size(backsize.width, backsize.height), CV_8UC1);

	overlayImage(tmp, mask, { 10, 10 });

	cntrs.reserve(1);

	cv::findContours(tmp, cntrs, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
}

void overlay2(const cv::Mat& mask, const cv::Size& backsize, CvContours& cntrs)
{
	cv::Mat tmp = cv::Mat::zeros(cv::Size(backsize.width, backsize.height), CV_8UC1);

	overlayImage(tmp, mask, { 124, 170 });

	cntrs.reserve(1);
	//std::vector<cv::Point> contrs;
	cv::findContours(tmp, cntrs, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
	if (cntrs.size() != 1)
	{
		std::cout << "Size Alarn\n";
		exit(-1);
	}
}

bool doMaskIntersect(const CvContours& prevContour, const CvContours& currContour)
{
	cv::Mat check1, check2, result;

	cv::drawContours(check1, prevContour, 0, 1);
	cv::drawContours(check2, currContour, 0, 1);

	cv::bitwise_and(check1, check2, result);
	cv::imshow("ss", check1);
	cv::waitKey(0);
	return false;
}

bool ccw(const cv::Point& A, const cv::Point& B, const cv::Point& C)
{
	return ((C.y - A.y) * (B.x - A.x)) > ((B.y - A.y) * (C.x - A.x));
}

bool fastContourIntersectInt(const std::vector<cv::Point>& refContour, const std::vector<cv::Point>& queryContour)
{
	for (size_t rIdx = 0; rIdx < refContour.size() - 1; rIdx++)
	{
		auto A = refContour[rIdx];
		auto B = refContour[rIdx + 1];

		for (size_t qIdx = 0; qIdx < queryContour.size() - 1; qIdx++)
		{
			auto C = queryContour[qIdx];
			auto D = queryContour[qIdx + 1];
			if ((ccw(A, C, D) != ccw(B, C, D)) && (ccw(A, B, C) != ccw(A, B, D)))
			{
				return true;
			}
		}
	}
	return false;
}

void test_overlay()
{
	auto mask = cv::imread(R"(C:\Users\ruwen\Desktop\Learning_CPP\Synthethic-Image-Generator\Test\t_img_klein0.png)", -1);
	auto back = cv::imread(R"(C:\Users\ruwen\Desktop\Learning_CPP\Synthethic-Image-Generator\Test\background0.jpg)");

	auto pos = getRandomPos(back.size(), mask.size());
	CvContours cntrs1;
	CvContours cntrs2;

	overlay1(mask, back.size(), cntrs1);
	overlay2(mask, back.size(), cntrs2);
	auto start = std::chrono::high_resolution_clock::now();

	//bool res = fastContourIntersectInt(cntrs1[0], cntrs2[0]);
	bool res = simG::algorithms::fastContourIntersect(cntrs1[0], cntrs2[0]);
	auto end = std::chrono::high_resolution_clock::now();
	duration<double, std::milli> ms_double = end - start;
	std::cout << ms_double.count() << "ms\n";
	simG::print(res);
	//doMaskIntersect(cntrs1, cntrs2);

	//cv::Mat tmp = cv::Mat::zeros(cv::Size(back.cols, back.rows), CV_8UC1);

	//overlayImage(tmp, mask, { 10, 10 });

	//CvContours contrs;
	//cv::findContours(tmp, contrs, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

	cv::drawContours(back, cntrs1, 0, cv::Scalar(0, 255, 0), 1);
	cv::drawContours(back, cntrs2, 0, cv::Scalar(0, 255, 0), 1);

	//simG::print(tmp.channels());
	//auto max_x = back.size().width - mask.size().width

	//auto end = std::chrono::high_resolution_clock::now();
	//duration<double, std::milli> ms_double = end - start;
	//std::cout << ms_double.count() << "ms\n";

	cv::imshow("", back);
	cv::waitKey(0);
	exit(0);
}

int main()
{
	test_overlay();
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

	simG::ImageGenerator generator(R"(C:\Users\ruwen\Desktop\SyntheticDataGenerator_Bachelor\Dataset\input\templates\transportation\car)",
		R"(C:\Users\ruwen\Desktop\Learning_CPP\Synthethic-Image-Generator\Test)", params);
	//MultithreadGenerator mGen(R"(C:\Users\ruwen\Desktop\SyntheticDataGenerator_Bachelor\Dataset\input\templates\transportation\car)");

	auto test_img = cv::imread(R"(C:\Users\ruwen\Desktop\Learning_CPP\Synthethic-Image-Generator\resources\t_00000001.jpg)");

	//
	//simG::ImageAugmenter augmenter;
	auto start = std::chrono::high_resolution_clock::now();

	//overlayImage(back, back, { 0,0 });
	//cv::imshow("", back);
	//cv::waitKey(0);

	//simG::transforms::RandomRotation layer({ 60,60 });
	//simG::transforms::Sequential transforms({
	//	//simG::transforms::RandomScale({0.5, 1.5}),
	//	//simG::transforms::RandomCrop({550, 100}, true),
	//	//simG::transforms::Resize({600, 400}, true),
	//	//simG::transforms::RandomBrightness({-50, 50}),
	//	//simG::transforms::GaussianBlur(1.0),
	//	//simG::transforms::RandomGaussNoise(1.0),
	//	//simG::transforms::RandomVerticalFlip(1.0),
	//	//simG::transforms::RandomHorizontalFlip(1.0),
	//	 //put annotator as transform like in https://github.com/LinkedAi/flip

	//	//simG::transforms::RandomRotation90(1),
	//	//simG::transforms::RandomRotation180(1.0),
	//	//simG::transforms::RandomRotation270(1.0),
	//	simG::transforms::RandomRotation({60,60})
	//	});

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
		"	   select C/C++->Language, and change 'OpenMP Support' to 'Yes' if using Visual Studio 2019. \n"
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