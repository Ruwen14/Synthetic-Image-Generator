#include "imageenhancer.h"
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>
#include <iostream>

namespace simG
{
	void ImageEnhancer::rescale(cv::Mat& img, std::pair<int, int> dim) const
	{
		cv::Size mat_size = img.size();
		int interpolation_flag;

		if ((dim.first > mat_size.width) && (dim.second > mat_size.height))
		{
			interpolation_flag = cv::INTER_CUBIC;
		}
		else
		{
			interpolation_flag = cv::INTER_LINEAR;
		}

		cv::resize(img, img, cv::Size(dim.first, dim.second), 0, 0, interpolation_flag);
	}

	void ImageEnhancer::rotate(double degree) const
	{
	}

	void ImageEnhancer::flip(cv::Mat& img, FlipMode fm) const
	{
		switch (fm)
		{
		case FlipMode::VERTICAL_FLIP:
			cv::flip(img, img, 0);
			break;
		case FlipMode::HORIZONTAL_FLIP:
			cv::flip(img, img, 1);
			break;
		case FlipMode::HORIZONTAL_AND_VERTICAL_FLIP:
			cv::flip(img, img, -1);
			break;
		default:
			break;
		}
	}

	void ImageEnhancer::shiftBrightness(int factor) const
	{
	}
}