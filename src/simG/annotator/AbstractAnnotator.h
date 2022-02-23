#pragma once
#include <string>
#include <variant>
#include <map>
#include <opencv2/core/mat.hpp>
#include <opencv2/core/types.hpp>
#include <fstream>
#include <iostream>

namespace simG::annotators
{
	typedef std::ofstream AnnotationFile;
	// ofstream has enough buffer. Tested with reading and writing 500MB
	using AnnotationDictionaryValues = std::variant<int, double, std::string, std::vector<cv::Point>>;
	using AnnotationDictionary = std::map<std::string, AnnotationDictionaryValues>;

	struct ObjectAnnotation
	{
		std::string category;
		std::vector<cv::Point> segpoints;
		std::array<int, 4> bbox;
		int catid;
	};
	struct AnnotationDetails
	{
		std::vector<ObjectAnnotation> objannots;
		std::string filename;
		int imgid;
		int channels;
		cv::Size imgdim;
	};

	struct SynImg
	{
		cv::Mat result;
		cv::Mat mask_component;
		cv::Mat bgrnd_component;
	};

	// ToDO see all formats: https://remo.ai/docs/annotation-formats/
	// https://blog.roboflow.com/how-to-convert-annotations-from-voc-xml-to-coco-json/
	class AbstractAnnotator
	{
	public:

		AbstractAnnotator(const std::string& annofile_savepath);
		virtual ~AbstractAnnotator() { std::cout << "AbstractAnnotator destroyed" << "\n"; }

		// ToDo: uSE Struct with all kinds of infos :)
		virtual void forward(const std::vector<cv::Mat>& masks, const AnnotationDictionary& annotationDict) = 0;
		virtual void close();

		/** @brief Setter method to enable annotation-clipping for overlapping object-instances.
		Following annotations will be clipped: segmentation-polygons, segmentation-masks.
		 @param clip(boolean) true if received annotations should be clipped, false if not. False by default.
		*/
		void setClipAnns(bool clip);

		/** @brief Getter-method to check if annotator will use clipped annotations.
		@return true or false
		*/
		bool isClipAnns();

	protected:
		AnnotationFile anno_file_;
		bool m_clipAnns = false;
	};

	//struct BBox
	//{
	//public:
	//	int x;
	//	int y;
	//	int w;
	//	int h;
	//};

	//using ListCategories = std::vector<std::string>;
	//using ListBBoxes = std::vector<BBox>;
	//using ListSegmentations = std::vector<std::vector<float>>;
	//using ListIds = std::vector<int>;
	//using AnnoDictValues = std::variant<int, double, bool, std::string, ListIds, ListCategories, ListBBoxes, ListSegmentations>;
	//using Pydict = std::map < std::string, AnnoDictValues>;

	//void doStuff(Pydict& annotationDict)
	//{
	//	auto segmentations = std::get<std::vector<std::vector<float>>>(annotationDict["segmentation"]);

	//	for (const auto& seg : segmentations)
	//	{
	//		//simG::print(seg);
	//	}

	//	auto bboxes = std::get<ListBBoxes>(annotationDict["bbox"]);

	//	for (const auto& bbox : bboxes)
	//	{
	//		//simG::print(bbox.x);
	//	}
	//}

//	void TEST_DICT()
//	{
//		auto start = std::chrono::high_resolution_clock::now();
//		//simG::Timer::Timer t1;
//		//simG::Timer::ScopeTimer s1;
//
//		Pydict myDict;
//		myDict["file_name"] = "000000093437.jpg";
//		myDict["id"] = 1;
//		myDict["width"] = 361;
//		myDict["height"] = 640;
//		myDict["area"] = 2420.0;
//		myDict["iscrowd"] = 0;
//		myDict["image_id"] = 1;
//
//		ListBBoxes bboxes;
//		bboxes.push_back(
//			BBox{ 10, 10, 350, 200 }
//		);
//		bboxes.push_back(
//			BBox{ 10, 10, 350, 200 }
//		);
//		bboxes.push_back(
//			BBox{ 10, 10, 350, 200 }
//		);
//		bboxes.push_back(
//			BBox{ 10, 10, 350, 200 }
//		);
//		bboxes.push_back(
//			BBox{ 10, 10, 350, 200 }
//		);
//		bboxes.push_back(
//			BBox{ 10, 10, 350, 200 }
//		);
//		myDict["bbox"] = bboxes;
//		myDict["category_id"] = 62;
//
//		ListSegmentations segs;
//		segs.push_back(
//			std::vector<float>{ 184.05, 440.78, 192.11, 445.71, 202.41, 445.71, 213.6, 437.2, 226.14, 428.25, 232.41, 423.32, 243.16, 423.32, 255.69, 428.25, 264.65, 434.96, 268.68, 443.02, 270.92, 452.87, 269.13, 466.31, 264.65, 468.99, 263.31, 487.8, 260.17, 500.34, 257.93, 507.05, 258.83, 511.98, 253.9, 516.01, 251.66, 510.19, 251.66, 489.59, 251.22, 473.47, 249.79, 494.67, 246.2, 494.43, 244.04, 489.4, 239.24, 477.16, 238.76, 473.56, 235.16, 470.2, 236.84, 477.4, 240.44, 487.24, 236.6, 489.16, 234.2, 486.28, 225.08, 472.36, 222.44, 461.32, 219.32, 447.64, 219.08, 446.92, 203.24, 455.56, 187.65, 454.36, 179.73, 446.44, 179.73, 440.2 }
//		);
//		segs.push_back(
//			std::vector<float>{ 184.05, 440.78, 192.11, 445.71, 202.41, 445.71, 213.6, 437.2, 226.14, 428.25, 232.41, 423.32, 243.16, 423.32, 255.69, 428.25, 264.65, 434.96, 268.68, 443.02, 270.92, 452.87, 269.13, 466.31, 264.65, 468.99, 263.31, 487.8, 260.17, 500.34, 257.93, 507.05, 258.83, 511.98, 253.9, 516.01, 251.66, 510.19, 251.66, 489.59, 251.22, 473.47, 249.79, 494.67, 246.2, 494.43, 244.04, 489.4, 239.24, 477.16, 238.76, 473.56, 235.16, 470.2, 236.84, 477.4, 240.44, 487.24, 236.6, 489.16, 234.2, 486.28, 225.08, 472.36, 222.44, 461.32, 219.32, 447.64, 219.08, 446.92, 203.24, 455.56, 187.65, 454.36, 179.73, 446.44, 179.73, 440.2 }
//		);
//		segs.push_back(
//			std::vector<float>{ 184.05, 440.78, 192.11, 445.71, 202.41, 445.71, 213.6, 437.2, 226.14, 428.25, 232.41, 423.32, 243.16, 423.32, 255.69, 428.25, 264.65, 434.96, 268.68, 443.02, 270.92, 452.87, 269.13, 466.31, 264.65, 468.99, 263.31, 487.8, 260.17, 500.34, 257.93, 507.05, 258.83, 511.98, 253.9, 516.01, 251.66, 510.19, 251.66, 489.59, 251.22, 473.47, 249.79, 494.67, 246.2, 494.43, 244.04, 489.4, 239.24, 477.16, 238.76, 473.56, 235.16, 470.2, 236.84, 477.4, 240.44, 487.24, 236.6, 489.16, 234.2, 486.28, 225.08, 472.36, 222.44, 461.32, 219.32, 447.64, 219.08, 446.92, 203.24, 455.56, 187.65, 454.36, 179.73, 446.44, 179.73, 440.2 }
//		);
//		segs.push_back(
//			std::vector<float>{ 184.05, 440.78, 192.11, 445.71, 202.41, 445.71, 213.6, 437.2, 226.14, 428.25, 232.41, 423.32, 243.16, 423.32, 255.69, 428.25, 264.65, 434.96, 268.68, 443.02, 270.92, 452.87, 269.13, 466.31, 264.65, 468.99, 263.31, 487.8, 260.17, 500.34, 257.93, 507.05, 258.83, 511.98, 253.9, 516.01, 251.66, 510.19, 251.66, 489.59, 251.22, 473.47, 249.79, 494.67, 246.2, 494.43, 244.04, 489.4, 239.24, 477.16, 238.76, 473.56, 235.16, 470.2, 236.84, 477.4, 240.44, 487.24, 236.6, 489.16, 234.2, 486.28, 225.08, 472.36, 222.44, 461.32, 219.32, 447.64, 219.08, 446.92, 203.24, 455.56, 187.65, 454.36, 179.73, 446.44, 179.73, 440.2 }
//		);
//		segs.push_back(
//			std::vector<float>{ 184.05, 440.78, 192.11, 445.71, 202.41, 445.71, 213.6, 437.2, 226.14, 428.25, 232.41, 423.32, 243.16, 423.32, 255.69, 428.25, 264.65, 434.96, 268.68, 443.02, 270.92, 452.87, 269.13, 466.31, 264.65, 468.99, 263.31, 487.8, 260.17, 500.34, 257.93, 507.05, 258.83, 511.98, 253.9, 516.01, 251.66, 510.19, 251.66, 489.59, 251.22, 473.47, 249.79, 494.67, 246.2, 494.43, 244.04, 489.4, 239.24, 477.16, 238.76, 473.56, 235.16, 470.2, 236.84, 477.4, 240.44, 487.24, 236.6, 489.16, 234.2, 486.28, 225.08, 472.36, 222.44, 461.32, 219.32, 447.64, 219.08, 446.92, 203.24, 455.56, 187.65, 454.36, 179.73, 446.44, 179.73, 440.2 }
//		);
//
//		myDict["classes"] = ListCategories{ "car", "plane", "bike", "person", "bird" };
//		myDict["segmentation"] = segs;
//		doStuff(myDict);
//
//		//std::this_thread::sleep_for(std::chrono::milliseconds(100));
//
//		//simG::print(t1.ElapsedMillis());
//
//		auto end = std::chrono::high_resolution_clock::now();
//		duration<double, std::milli> ms_double = end - start;
//		std::cout << ms_double.count() << "ms\n";
//	}
}
