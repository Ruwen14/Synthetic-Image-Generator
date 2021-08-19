#include "datasetviz.h"
#include "Timer.h"

#include <../../external/rapidjson/istreamwrapper.h>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <fstream>
#include <iostream>

namespace simG::viz
{
	cv::Scalar DatasetVisualizer::getRandomColor() const
	{
		cv::Scalar rndColor(
			simG::Random::uniformInt(0, 255),
			simG::Random::uniformInt(0, 255),
			simG::Random::uniformInt(0, 255)
		);
		return rndColor;
	}

	void DatasetVisualizer::drawBBox(cv::Mat& dst, cv::Point upperleft, cv::Point bottomright, cv::Scalar color) const
	{
		cv::rectangle(dst, upperleft, bottomright, color, linewidth_);
	}

	void DatasetVisualizer::drawCategoryTag(cv::Mat& dst, const std::string& category, cv::Point pos, cv::Scalar color) const
	{
		auto textsize = cv::getTextSize(category, fontFace_, fontScale_, fontFace_, 0);

		cv::rectangle(dst, pos - cv::Point(1, 0), pos + cv::Point(textsize.width, -textsize.height), color, -1);
		cv::putText(dst, category, pos - cv::Point(0, 2), fontFace_, fontScale_, cv::Scalar(255, 255, 255), 1, cv::LINE_AA);
	}

	void DatasetVisualizer::drawSegmentationMask(cv::Mat& dst, const std::vector<cv::Point>& cntr, cv::Scalar color) const
	{
		std::vector<std::vector<cv::Point>> cntrs{ cntr };
		cv::drawContours(dst, cntrs, -1, color, cv::FILLED, 16);
	}

	COCOVizualizer::COCOVizualizer(const std::string& annotationfile)
	{
		auto start = Timer::startTimer();
		std::ifstream file(annotationfile);
		rapidjson::IStreamWrapper wrapper(file);
		json_.ParseStream(wrapper);
		std::cout << "Loading annotations in memory... ";
		Timer::timeElapsed(start);
	}

	cv::Mat COCOVizualizer::showAnnotations(const std::string& filepath) const
	{
		auto filename = filepath.substr(filepath.find_last_of("/\\") + 1);
		int imageid = getImageId(filename);

		std::cout << "Loading Image <" << filename << ">... ";
		auto loadingtime = simG::Timer::startTimer();
		cv::Mat dst = cv::imread(filepath);
		simG::Timer::timeElapsed(loadingtime);

		//std::cout << "Query Annotations for ID <" << imageid << ">... ";
		//auto querytime = simG::Timer::startTimer();
		//auto annotationsToDraw = queryAnnotations(imageid);
		//simG::Timer::timeElapsed(querytime);


		auto drawingtime = simG::Timer::startTimer();

		for (rapidjson::Value::ConstValueIterator itr = json_["annotations"].Begin(); itr != json_["annotations"].End(); ++itr)
		{
			rapidjson::Value id_(imageid);
			if ((*itr)["image_id"] == id_)
			{
				auto color = getRandomColor();

				cv::Point upperleft(static_cast<int>((*itr)["bbox"][0].GetDouble()), static_cast<int>((*itr)["bbox"][1].GetDouble()));
				cv::Point bottomright(upperleft + cv::Point(static_cast<int>((*itr)["bbox"][2].GetDouble()), static_cast<int>((*itr)["bbox"][3].GetDouble())));


				drawBBox(dst, upperleft, bottomright, color);
				std::cout << "Category <" << getCategoryById((*itr)["category_id"].GetInt()) << "> @ " << upperleft << "\n";

				drawCategoryTag(dst, getCategoryById((*itr)["category_id"].GetInt()), upperleft, color);

				if ((*itr).HasMember("segmentation")) {
					std::vector<double> seglist;
					size_t size = (*itr)["segmentation"][0].GetArray().Size();
					seglist.reserve(size);

					for (rapidjson::Value::ConstValueIterator segitr = (*itr)["segmentation"][0].Begin(); segitr != (*itr)["segmentation"][0].End(); ++segitr) {
						seglist.push_back((*segitr).GetDouble());
					}

					drawSegmentationMask(dst, translateListToContour(seglist), color);
				}
			}
		}
		std::cout << "Loading and drawing Annotations for ID <" << imageid << ">... ";
		simG::Timer::timeElapsed(drawingtime);
		std::cout << "------------------------------\n";
		return dst;

		//std::cout << "Draw Annotations for ID <" << imageid << ">... ";
		//auto drawingtime = simG::Timer::startTimer();

		//for (const auto& annotation : annotationsToDraw)
		//{
		//	auto color = getRandomColor();

		//	cv::Point upperleft(static_cast<int>(annotation.bbox[0]), static_cast<int>(annotation.bbox[1]));
		//	cv::Point bottomright(upperleft + cv::Point(static_cast<int>(annotation.bbox[2]), static_cast<int>(annotation.bbox[3])));

		//	drawBBox(dst, upperleft, bottomright, color);
		//	drawCategoryTag(dst, annotation.category, upperleft, color);
		//	drawSegmentationMask(dst, translateListToContour(annotation.segmentations), color);
		//}

		//simG::Timer::timeElapsed(drawingtime);
		//std::cout << "------------------------------\n";
		//return dst;
	}

	int COCOVizualizer::getImageId(const std::string fname) const
	{
		rapidjson::Value filen;
		filen.SetString(fname.data(), static_cast<rapidjson::SizeType>(fname.size()));

		for (rapidjson::Value::ConstValueIterator itr = json_["images"].Begin(); itr != json_["images"].End(); ++itr)
		{
			if (filen == (*itr)["file_name"])
			{
				return (*itr)["id"].GetInt();
			}
		}
		return -1;
	}

	std::string COCOVizualizer::getCategoryById(int catId) const
	{
		for (rapidjson::Value::ConstValueIterator itr = json_["categories"].Begin(); itr != json_["categories"].End(); ++itr)
		{
			if ((*itr)["id"] == catId)
			{
				return static_cast<std::string>(((*itr)["name"].GetString()));
			}
		}
		return "";
	}

	//std::vector<COCOVizualizer::Annotations> COCOVizualizer::queryAnnotations(int imageId) const
	//{
	//	std::vector<COCOVizualizer::Annotations> query;
	//	for (rapidjson::Value::ConstValueIterator itr = json_["annotations"].Begin(); itr != json_["annotations"].End(); ++itr)
	//	{
	//		rapidjson::Value id_(imageId);
	//		if ((*itr)["image_id"] == id_)
	//		{
	//			std::vector<double> seglist;
	//			size_t size = (*itr)["segmentation"][0].GetArray().Size();
	//			seglist.reserve(size);
	//			for (rapidjson::Value::ConstValueIterator segitr = (*itr)["segmentation"][0].Begin(); segitr != (*itr)["segmentation"][0].End(); ++segitr)
	//			{
	//				seglist.push_back((*segitr).GetDouble());
	//			}

	//			query.push_back({
	//				{
	//					(*itr)["bbox"][0].GetDouble(),
	//					(*itr)["bbox"][1].GetDouble(),
	//					(*itr)["bbox"][2].GetDouble(),
	//					(*itr)["bbox"][3].GetDouble()
	//				},
	//				seglist,
	//				getCategoryById((*itr)["category_id"].GetInt()) });
	//		}
	//	}
	//	return query;
	//}

	std::vector<cv::Point> COCOVizualizer::translateListToContour(const std::vector<double>& src) const
	{
		if (src.size() % 2 != 0)
		{
			std::cout << "NOT EVEN ERROR" << "\n";
			exit(0);
		}
		std::vector<cv::Point> dstCntrs;
		dstCntrs.reserve(src.size() / 2);

		int cdx = 0;
		int ndx = 1;
		for (size_t i = 0; i < src.size() / 2; i++)
		{
			cv::Point p(static_cast<int>(src[cdx]), static_cast<int>(src[ndx]));
			dstCntrs.push_back(p);
			cdx += 2;
			ndx += 2;
		}
		return dstCntrs;
	}
}