#pragma once
#include <opencv2/opencv.hpp>
#include <string>
namespace simG
{
	// ToDo: https://www.youtube.com/watch?v=bvTu1b1AhOg
	class ForegroundExtractor
	{
	public:
		ForegroundExtractor() = default;
		~ForegroundExtractor() = default;

		static cv::Mat extract(const cv::Mat& inputImg, const cv::Rect& bbox);
		static cv::Mat extractDNN(const cv::Mat& inputImg, const std::string& modelWeigths, const std::string& modelConfig, const std::string& categoryfilter);
	};
}
