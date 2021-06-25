#include "imageenhancer.h"
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>
#include <iostream>

namespace simG
{
	void ImageEnhancer::rescale(cv::Mat& img, std::pair<int, int> dim) const
	{
		cv::Size img_size = img.size();
		// if image has the target-dim already, simply return
		if (img_size.width == dim.first && img_size.height == dim.second)
		{
			return;
		}
		cv::resize(img, img, cv::Size(dim.first, dim.second), 0, 0, cv::INTER_LINEAR);
	}

	cv::Mat ImageEnhancer::rotate(const cv::Mat& img, double degree) const
	{
		// get rotation matrix for rotating the image around its center in pixel coordinates
		cv::Point2f image_center(img.cols / 2.0, img.rows / 2.0);
		cv::Mat rotation_matrix = cv::getRotationMatrix2D(image_center, degree, 1.0);

		// determine bounding rectangle
		cv::Rect2f bbox = cv::RotatedRect(cv::Point2f(), img.size(), degree).boundingRect2f();

		// adjust transformation matrix
		rotation_matrix.at<double>(0, 2) += bbox.width / 2.0 - img.cols / 2.0;
		rotation_matrix.at<double>(1, 2) += bbox.height / 2.0 - img.rows / 2.0;

		// transform
		cv::Mat dst;
		cv::warpAffine(img, dst, rotation_matrix, bbox.size());

		return dst;
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

	void ImageEnhancer::shiftBrightness(cv::Mat& img, double beta, double alpha /*= 1*/) const
	{
		img.convertTo(img, -1, alpha, beta);
	}
}