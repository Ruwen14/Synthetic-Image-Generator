#pragma once
#include "../simG.h"
#include "core/algorithms.h"
#include "../../external/rapidjson/document.h"
#include "../../external/rapidjson/writer.h"
#include "../../external/rapidjson/prettywriter.h"

#include "../../external/rapidjson/stringbuffer.h"
#include "../../external/rapidjson/filereadstream.h"
#include <../../external/rapidjson/istreamwrapper.h>

#include <iostream>
#include <chrono>
#include <variant>
#include <cstdio>
#include <optional>

using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;



void ADD_COCO()
{
	// COCO STRUCTURE
	//{
	//	"info": {...},
	//		"licenses" : [...] ,
	//		"images" : [...] ,
	//		"annotations" : [...] ,
	//		"categories" : [...] ,
	//}

	std::vector testVector{ 634.97, 417.61, 632.15, 413.01, 627.55, 409.12, 622.6, 406.65, 616.95, 404.53, 611.29, 403.82, 605.28, 403.82, 599.63, 404.18, 593.62, 405.59, 587.61, 406.65, 581.25, 407.71, 575.95, 408.42, 570.65, 409.48, 564.99, 409.48, 559.34, 409.48, 553.68, 407.71, 548.73, 405.59, 543.08, 404.18, 539.54, 408.42, 537.07, 413.37, 531.42, 413.37, 525.05, 414.07, 519.4, 414.07, 513.74, 414.07, 507.73, 414.43, 501.37, 414.43, 495.72, 414.78, 489.36, 414.78, 482.64, 414.78, 477.34, 415.13, 472.04, 416.19, 466.38, 416.9, 460.73, 417.61, 454.72, 417.96, 449.42, 418.31, 444.11, 419.73, 438.81, 420.43, 433.51, 421.85, 425.74, 423.97, 421.49, 427.86, 417.25, 435.28, 416.55, 443.76, 417.61, 455.78, 417.61, 467.44, 417.96, 477.34, 420.43, 489.0, 422.91, 499.61, 425.74, 508.79, 429.62, 514.1, 437.05, 517.98, 445.18, 519.4, 465.32, 520.81, 472.39, 520.81, 478.05, 521.17, 484.41, 521.17, 491.12, 520.81, 497.13, 520.81, 503.49, 520.46, 508.79, 520.11, 514.45, 519.04, 520.11, 518.69, 526.47, 517.63, 531.77, 517.28, 537.07, 516.92, 542.73, 516.57, 555.1, 515.51, 551.56, 520.11, 549.79, 523.29, 549.79, 525.05, 555.1, 525.76, 557.92, 523.29, 559.69, 519.04, 561.46, 516.57, 572.06, 514.45, 592.21, 515.16, 604.93, 513.39, 621.54, 510.92, 628.61, 508.09, 636.74, 499.61, 639.57, 492.54, 641.34, 481.58, 641.34, 465.67, 640.63, 451.54, 638.15, 433.16 };

	auto start = std::chrono::high_resolution_clock::now();

	//auto start = std::chrono::high_resolution_clock::now();

	rapidjson::Document rootObj;
	// define the document as an object rather than an array
	rootObj.SetObject();
	rapidjson::Document::AllocatorType& allocator = rootObj.GetAllocator();

	rapidjson::Value AnnotationFieldsArray(rapidjson::kArrayType);
	rapidjson::Value Bbox(rapidjson::kArrayType);
	//rootObj.AddMember("annotations", AnnotationsArray, allocator);

	// **************************************************
	// apppend to "annotations": []
	// **************************************************
	rapidjson::Value SingleAnnotationFieldObject(rapidjson::kObjectType);
	SingleAnnotationFieldObject.AddMember("id", 15, allocator);
	SingleAnnotationFieldObject.AddMember("image_id", 3, allocator);
	SingleAnnotationFieldObject.AddMember("category_id", 1, allocator);
	SingleAnnotationFieldObject.AddMember("iscrowd", 0, allocator);
	SingleAnnotationFieldObject.AddMember("area", 22859.0, allocator);
	Bbox.PushBack(416.55, allocator).PushBack(403.82, allocator).PushBack(224.79, allocator).PushBack(121.94, allocator);
	SingleAnnotationFieldObject.AddMember("bbox", Bbox, allocator);
	rapidjson::Value SegmentationArray(rapidjson::kArrayType);
	for (const auto& el : testVector)
	{
		SegmentationArray.PushBack(el, allocator);
	}
	rapidjson::Value DummyArrayBrackets(rapidjson::kArrayType);
	DummyArrayBrackets.PushBack(SegmentationArray, allocator);
	SingleAnnotationFieldObject.AddMember("segmentation", DummyArrayBrackets, allocator);

	//rootObj["annotations"].PushBack(SingleAnnotationObject, allocator);;Evertime we add someting to list of annotations
	AnnotationFieldsArray.PushBack(SingleAnnotationFieldObject, allocator);
	rootObj.AddMember("annotations", AnnotationFieldsArray, allocator);

	// **************************************************
	// apppend to "images": []
	// **************************************************
	rapidjson::Value ImageFieldsArray(rapidjson::kArrayType);
	rapidjson::Value SingleImageFieldObject(rapidjson::kObjectType);

	SingleImageFieldObject.AddMember("id", 15, allocator);
	SingleImageFieldObject.AddMember("width", 640, allocator);
	SingleImageFieldObject.AddMember("height", 427, allocator);
	SingleImageFieldObject.AddMember("file_name", "000000037777.jpg", allocator);
	SingleImageFieldObject.AddMember("coco_url", "", allocator);
	SingleImageFieldObject.AddMember("date_captured", "", allocator);
	SingleImageFieldObject.AddMember("flickr_url", "", allocator);
	SingleImageFieldObject.AddMember("license", 0, allocator);

	ImageFieldsArray.PushBack(SingleImageFieldObject, allocator);

	rootObj.AddMember("images", ImageFieldsArray, allocator);

	// **************************************************
	// apppend to "images": []
	// **************************************************
	rapidjson::Value CategoryFieldsArray(rapidjson::kArrayType);

	//rapidjson::StringBuffer strbuf;
	//rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(strbuf);
	//rootObj.Accept(writer);

	//std::cout << strbuf.GetString() << std::endl;

	auto end = std::chrono::high_resolution_clock::now();
	duration<double, std::milli> ms_double = end - start;
	std::cout << ms_double.count() << "ms\n";
}

void TEST_RAPIDJSON()
{
	auto start = std::chrono::high_resolution_clock::now();

	// BUILD DICT
	rapidjson::Document rootObj;

	// define the document as an object rather than an array
	rootObj.SetObject();

	rapidjson::Value array(rapidjson::kArrayType);

	// must pass an allocator when the object may need to allocate memory
	rapidjson::Document::AllocatorType& allocator = rootObj.GetAllocator();

	// chain methods as rapidjson provides a fluent interface when modifying its objects
	array.PushBack("hello", allocator).PushBack(3, allocator);//"array":["hello","world"]

	std::string b = "hi";
	double asd = 3.414123;
	rapidjson::Value bString(rapidjson::kObjectType);
	rapidjson::Value doubleD(3.414123);
	bString.SetString(b.c_str(), allocator);
	rootObj.AddMember("Name", bString, allocator);
	rootObj.AddMember("Rollnumer", asd, allocator);
	rootObj.AddMember("array", array, allocator);

	// create a rapidjson object type
	rapidjson::Value object(rapidjson::kObjectType);
	object.AddMember("Math", "50", allocator);
	object.AddMember("Science", "70", allocator);
	object.AddMember("English", "50", allocator);
	object.AddMember("Social Science", "70", allocator);
	rootObj.AddMember("Marks", object, allocator);
	//	fromScratch["object"]["hello"] = "Yourname";

	rapidjson::StringBuffer strbuf;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(strbuf);
	rootObj.Accept(writer);

	std::cout << strbuf.GetString() << std::endl;

	auto end = std::chrono::high_resolution_clock::now();
	duration<double, std::milli> ms_double = end - start;
	std::cout << ms_double.count() << "ms\n";
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

	simG::algorithms::composeMatrices(tmp, mask, { 500, 250 });

	cntrs.reserve(1);

	cv::findContours(tmp, cntrs, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
}

void overlay2(const cv::Mat& mask, const cv::Size& backsize, CvContours& cntrs)
{
	cv::Mat tmp = cv::Mat::zeros(cv::Size(backsize.width, backsize.height), CV_8UC1);

	simG::algorithms::composeMatrices(tmp, mask, { 10, 10 });

	cntrs.reserve(1);
	//std::vector<cv::Point> contrs;
	cv::findContours(tmp, cntrs, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
	if (cntrs.size() != 1)
	{
		std::cout << "Size Alarn\n";
		exit(-1);
	}
}
void test_overlay()
{
	auto mask = cv::imread(R"(C:\Users\ruwen\Desktop\Learning_CPP\Synthethic-Image-Generator\Test\t_img_klein0.png)", -1);
	auto back = cv::imread(R"(C:\Users\ruwen\Desktop\Learning_CPP\Synthethic-Image-Generator\Test\background0.jpg)");

	auto pos = getRandomPos(back.size(), mask.size());
	CvContours cntrs1;
	CvContours cntrs2;
	//auto start = std::chrono::high_resolution_clock::now();

	overlay1(mask, back.size(), cntrs1);
	overlay2(mask, back.size(), cntrs2);

	//bool res = fastContourIntersectInt(cntrs1[0], cntrs2[0]);
	bool res = simG::algorithms::fastContourIntersect(cntrs1[0], cntrs2[0]);
	auto start = std::chrono::high_resolution_clock::now();

	int dist_x = 500 - 10;
	int dist_y = 250 - 10;

	cv::Point dist = cv::Point(10, 10) - cv::Point(500, 250);

	//simG::print(dist_x);
	//simG::print(dist_y);

	//simG::algorithms::translateContour(cntrs2[0], dist_x, dist_y);
	//simG::algorithms::rotateContour(cntrs1[0], 0.5);
	std::vector<cv::Point> rese;
	//simG::print(0.03* cv::arcLength(cntrs2[0], true));
	simG::algorithms::simplifyContour(cntrs2[0], rese, 0.0001);
	simG::print(cntrs2[0].size());

	simG::print(rese.size());

	auto end = std::chrono::high_resolution_clock::now();
	duration<double, std::milli> ms_double = end - start;
	std::cout << ms_double.count() << "ms\n";
	//simG::print(res);
	//doMaskIntersect(cntrs1, cntrs2);

	//cv::Mat tmp = cv::Mat::zeros(cv::Size(back.cols, back.rows), CV_8UC1);

	//overlayImage(tmp, mask, { 10, 10 });

	//CvContours contrs;
	//cv::findContours(tmp, contrs, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

	cntrs2[0] = rese;
	cv::drawContours(back, cntrs1, 0, cv::Scalar(0, 255, 0), 1);
	cv::drawContours(back, cntrs2, 0, cv::Scalar(0, 0, 255), 1);

	//simG::print(tmp.channels());
	//auto max_x = back.size().width - mask.size().width

	//auto end = std::chrono::high_resolution_clock::now();
	//duration<double, std::milli> ms_double = end - start;
	//std::cout << ms_double.count() << "ms\n";

	cv::imshow("", back);
	cv::waitKey(0);
	exit(0);
}

void load_images(simG::Directory dir)
{
	std::vector<cv::Mat>img_lst;
	std::mutex mtx;

	int count = 0;

#pragma omp parallel for
	for (int i = 0; i < dir.entries.size(); i++)
	{
		simG::print(omp_get_num_threads());

		cv::Mat img = cv::imread(dir.absoluteFilePath(dir.entries[i]));
		++count;
		//mtx.lock();
		//img_lst.push_back(img);
		//mtx.unlock();
	}
	simG::print(count);
	//mtx.lock();
	//std::cout << "INFO: Loaded " << img_lst.size() << std::endl;
	//mtx.unlock();
}

void load_images_single(simG::Directory dir) {
	std::vector<cv::Mat>img_lst;
	std::mutex mtx;

	int count = 0;

	for (int i = 0; i < dir.entries.size(); i++)
	{
		cv::Mat img = cv::imread(dir.absoluteFilePath(dir.entries[i]));
		++count;
		//mtx.lock();
		//img_lst.push_back(img);
		//mtx.unlock();
	}
	simG::print(count);
	//mtx.lock();
	//std::cout << "INFO: Loaded " << img_lst.size() << std::endl;
	//mtx.unlock();
}

std::vector<std::string> getClassNames(const std::string& path)
{
	std::vector<std::string> classes;
	std::ifstream input(path);
	std::string line;
	while (std::getline(input, line)) classes.push_back(line);

	return classes;
}

std::vector<std::string> getOutputNames(const cv::dnn::Net& net)
{
	std::vector<std::string> names;
	auto outLayers = net.getUnconnectedOutLayers();
	auto layersNames = net.getLayerNames();

	names.resize(outLayers.size());
	for (size_t i = 0; i < outLayers.size(); ++i)
	{
		names[i] = layersNames[outLayers[i] - 1];
	}

	return names;
}

struct LidarPoint { // single lidar point in space
	double x, y, z, r; // x,y,z in [m], r is point reflectivity
};

struct BoundingBox { // bounding box around a classified object (contains both 2D and 3D data)
	int boxID; // unique identifier for this bounding box
	int trackID; // unique identifier for the track to which this bounding box belongs

	cv::Rect roi; // 2D region-of-interest in image coordinates
	int classID; // ID based on class file provided to YOLO framework
	double confidence; // classification trust

	std::vector<LidarPoint> lidarPoints; // Lidar 3D points which project into 2D image roi
	std::vector<cv::KeyPoint> keypoints; // keypoints enclosed by 2D roi
	std::vector<cv::DMatch> kptMatches; // keypoint matches enclosed by 2D roi
};

// Remove the bounding boxes with low confidence using non-maxima suppression
void postprocess(const cv::Mat& img, const std::vector<cv::Mat>& netOutput, const std::vector<std::string>& classes, float confThreshold = 0.2, float nmsThreshold = 0.4)
{
	std::vector<int> classIds;
	std::vector<float> confidences;
	std::vector<cv::Rect> boxes;

	std::cout << "netOutput size:" << netOutput.size() << "\n";
	for (const auto& i : netOutput)
	{
		auto* data = (float*)i.data;
		for (int j = 0; j < i.rows; ++j, data += i.cols)
		{
			cv::Mat scores = i.row(j).colRange(5, i.cols);
			cv::Point classId;
			double confidence;

			// Get the value and location of the maximum score
			cv::minMaxLoc(scores, nullptr, &confidence, nullptr, &classId);
			if (confidence > confThreshold)
			{
				cv::Rect box; int cx, cy;
				cx = (int)(data[0] * img.cols);
				cy = (int)(data[1] * img.rows);
				box.width = (int)(data[2] * img.cols);
				box.height = (int)(data[3] * img.rows);
				box.x = cx - box.width / 2; // left
				box.y = cy - box.height / 2; // top
				std::cout << "box.x: " << box.x << " , ";
				std::cout << "box.y: " << box.y << "\n";
				boxes.push_back(box);
				classIds.push_back(classId.x);
				confidences.push_back((float)confidence);
			}
		}
	}
	std::cout << "# of bounding box: " << boxes.size() << "\n";
	// perform non-maxima suppression
	std::vector<int> indices;
	cv::dnn::NMSBoxes(boxes, confidences, confThreshold, nmsThreshold, indices);
	std::vector<BoundingBox> bBoxes;
	for (int& indice : indices)
	{
		BoundingBox bBox;
		bBox.roi = boxes[indice];
		bBox.classID = classIds[indice];
		bBox.confidence = confidences[indice];
		bBox.boxID = (int)bBoxes.size(); // zero-based unique identifier for this bounding box

		bBoxes.push_back(bBox);
	}

	// show results
	cv::Mat visImg = img.clone();
	for (auto& bBoxe : bBoxes)
	{
		// Draw rectangle displaying the bounding box
		int top, left, width, height;
		top = bBoxe.roi.y;
		left = bBoxe.roi.x;
		width = bBoxe.roi.width;
		height = bBoxe.roi.height;
		cv::rectangle(visImg, cv::Point(left, top), cv::Point(left + width, top + height), cv::Scalar(0, 255, 0), 2);

		std::string label = cv::format("%.2f", bBoxe.confidence);
		label = classes[(bBoxe.classID)] + ":" + label;

		// Display label at the top of the bounding box
		int baseLine;
		cv::Size labelSize = getTextSize(label, cv::FONT_ITALIC, 0.5, 1, &baseLine);
		top = std::max(top, labelSize.height);
		rectangle(visImg, cv::Point(left, top - round(1.1 * labelSize.height)), cv::Point(left + round(1.1 * labelSize.width), top + baseLine), cv::Scalar(255, 255, 255), cv::FILLED);
		cv::putText(visImg, label, cv::Point(left, top), cv::FONT_ITALIC, 0.5, cv::Scalar(0, 0, 0), 1);
	}

	std::string windowName = "Object classification";
	cv::namedWindow(windowName, 1);
	cv::imshow(windowName, visImg);
	cv::waitKey(0); // wait for key to be pressed
}

void yoloDetect()
{
	cv::Mat img = cv::imread(R"(C:\Users\ruwen\Desktop\Learning_CPP\Synthethic-Image-Generator\resources\trees.jpg)");
	auto classes = getClassNames(R"(C:\Users\ruwen\Desktop\Learning_CPP\Synthethic-Image-Generator\resources\weights\yolo_openimages_offical\openimages.names)");

	//cv::dnn::Net net = cv::dnn::readNet(
	//	R"(C:\Users\ruwen\Desktop\Learning_CPP\Synthethic-Image-Generator\resources\weights\inception_openimages\saved_model.pb)",
	//	R"(C:\Users\ruwen\Desktop\Learning_CPP\Synthethic-Image-Generator\resources\weights\inception_openimages\config.pbtxt)");

	cv::dnn::Net net;
	exit(0);

	net.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
	net.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);
	auto names = getOutputNames(net);

	cv::Mat blob;
	cv::dnn::blobFromImage(img, blob, 1 / 255.0, cv::Size(600, 600), cv::Scalar(0, 0, 0), true, false);

	net.setInput(blob);
	std::vector<cv::Mat> outs;
	net.forward(outs, names);

	postprocess(img, outs, classes, 0.1, 0.5);

	std::vector<double> layersTimes;
	double freq = cv::getTickFrequency() / 1000;
	double t = net.getPerfProfile(layersTimes) / freq;
	std::string label = cv::format("Inference time for a frame : %.2f ms", t);
	simG::print(label);
}
