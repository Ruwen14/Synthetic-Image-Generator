#include "foregroundextractor.h"

namespace simG
{
	cv::Mat ForegroundExtractor::extract(const cv::Mat& inputImg, const cv::Rect& bbox)
	{
		cv::Mat foreground;
		//cv::grabCut()
		return foreground;
	}

	cv::Mat ForegroundExtractor::extractDNN(const cv::Mat& inputImg, const std::string& modelWeigths, const std::string& modelConfig, const std::string& categoryfilter)
	{
		cv::Mat foreground;
		//cv::dnn::readNet()

		//cv::grabCut()
		cv::Rect outline;
		return extract(inputImg, outline);
	}
}