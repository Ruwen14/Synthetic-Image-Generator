#include "imgaugmenter.h"
#include "Utils/Random.h"

#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>
#include <iostream>

namespace simG
{
	void ImageAugmenter::rescale(cv::Mat& img, Dimensions dim, ResizeMode resMode, bool keepAspectRatio /*= false*/) const
	{
		int res_mode;
		cv::Size img_size = img.size();
		// if image has the target-dim already, simply return
		if (img_size.width == dim.width && img_size.height == dim.height)
		{
			return;
		}

		if (keepAspectRatio)
		{
			dim = calcPreservedAspectRatio({ img_size.width, img_size.height }, dim);
		}

		switch (resMode)
		{
		case ResizeMode::NEAREST:
			res_mode = 0;
			break;
		case ResizeMode::LINEAR:
			res_mode = 1;
			break;
		case ResizeMode::CUBIC:
			res_mode = 2;
			break;
		case ResizeMode::AREA:
			res_mode = 3;
			break;
		case ResizeMode::FASTEST:
			res_mode = 0;
			break;
		case ResizeMode::AUTO:
			//  Shrinking --> Area is best
			if (img_size.width > dim.width && img_size.height > dim.height)
			{
				res_mode = 3; // Same as AREA
			}
			// Enlarging --> Linear fast and good enough
			else
			{
				res_mode = 1;
			}
			break;
		default:
			res_mode = 1;
			break;
		}

		//std::cout << dim.width << "\n" << dim.height << "\n";
		cv::resize(img, img, cv::Size(dim.width, dim.height), 0, 0, res_mode);
	}

	// ToDO entferne nervigen schwarzen Rand.
	cv::Mat ImageAugmenter::rotate(const cv::Mat& img, double degree) const
	{
		cv::Mat dst;
		if (degree == 0)
		{
			return img;
		}
		// get rotation matrix for rotating the image around its center in pixel coordinates
		cv::Point2f image_center(static_cast<float>(img.cols / 2.0), static_cast<float>(img.rows / 2.0));
		cv::Mat rotation_matrix = cv::getRotationMatrix2D(image_center, degree, 1.0);

		// determine bounding rectangle
		cv::Rect2f bbox = cv::RotatedRect(cv::Point2f(), img.size(), static_cast<float>(degree)).boundingRect2f();

		// adjust transformation matrix
		rotation_matrix.at<double>(0, 2) += bbox.width / 2.0 - img.cols / 2.0;
		rotation_matrix.at<double>(1, 2) += bbox.height / 2.0 - img.rows / 2.0;

		// transform
		cv::warpAffine(img, dst, rotation_matrix, bbox.size());

		return dst;
	}

	cv::Mat ImageAugmenter::rotate(const cv::Mat& img, RotationMode rm) const
	{
		cv::Mat dst;
		switch (rm)
		{
		case RotationMode::ROTATE_90_CLCKWISE:
			cv::rotate(img, dst, 0);
			break;
		case RotationMode::ROTATE_180:
			cv::rotate(img, dst, 1);
			break;
		case RotationMode::ROTATE_90_CNTRCLCKWISE:
			cv::rotate(img, dst, 2);
			break;
		}
		return dst;
	}

	cv::Mat ImageAugmenter::randomRotate(const cv::Mat& img, Range<int> degreeRange) const
	{
		double degree = Random::uniformInt(degreeRange.lower, degreeRange.upper);
		return rotate(img, degree);
	}

	void ImageAugmenter::flip(cv::Mat& img, FlipMode fm) const
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
		case FlipMode::NO_FLIP:
			return;
		default:
			break;
		}
	}

	void ImageAugmenter::randomFlip(cv::Mat& img, bool includeNoFlip /*= true*/) const
	{
		FlipMode flip_mode;
		// Random Flip Mode
		if (includeNoFlip)
		{
			flip_mode = static_cast<FlipMode>(Random::uniformInt(-1, 2));
		}
		else
		{
			flip_mode = static_cast<FlipMode>(Random::uniformInt(-1, 1));
		}

		flip(img, flip_mode);
	}

	void ImageAugmenter::shiftBrightness(cv::Mat& img, double beta, double alpha /*= 1*/) const
	{
		img.convertTo(img, -1, alpha, beta);
	}

	void ImageAugmenter::randomBrightness(cv::Mat& img, Range<double> brightnessRange) const
	{
		double beta = Random::uniformDouble(brightnessRange.lower, brightnessRange.upper);
		if (beta == 0)
		{
			return;
		}
		shiftBrightness(img, beta, 1.0);
	}

	cv::Mat ImageAugmenter::crop(const cv::Mat& img, const cv::Rect& roi) const
	{
		cv::Mat cropped_img = img(roi);
		return cropped_img;
	}

	cv::Mat ImageAugmenter::randomCrop(const cv::Mat& img, const Dimensions& maxCropDim, bool keepAspectRatio /*= true*/) const
	{
		cv::Size img_size = img.size();
		Dimensions crop_dim;

		if (keepAspectRatio)
		{
			crop_dim = calcPreservedAspectRatio({ img_size.width, img_size.height }, maxCropDim);
		}
		else
		{
			crop_dim = maxCropDim;
		}

		assert(img_size.width >= crop_dim.width && img_size.height >= crop_dim.height);

		int max_x = img_size.width - crop_dim.width;
		int max_y = img_size.height - crop_dim.height;

		int crop_x = Random::uniformInt(0, max_x);
		int crop_y = Random::uniformInt(0, max_y);

		return crop(img, cv::Rect(crop_x, crop_y, crop_dim.width, crop_dim.height));
	}

	cv::Mat ImageAugmenter::addGaussianNoise(const cv::Mat& img, double mean /*= 0.0*/, double stdDev /*= 10.0*/) const
	{
		cv::Mat mSrc_16SC;
		cv::Mat dst;
		cv::Mat mGaussian_noise = cv::Mat(img.size(), CV_16SC3);
		randn(mGaussian_noise, cv::Scalar::all(mean), cv::Scalar::all(stdDev));

		img.convertTo(mSrc_16SC, CV_16SC3);
		cv::addWeighted(mSrc_16SC, 1.0, mGaussian_noise, 1.0, 0.0, mSrc_16SC);
		mSrc_16SC.convertTo(dst, img.type());

		return dst;
	}

	Dimensions ImageAugmenter::calcPreservedAspectRatio(const Dimensions& srcDim, const Dimensions& targetDim) const
	{
		Dimensions new_dim;

		if (srcDim.width == targetDim.width && srcDim.height == targetDim.height)
		{
			return { srcDim.width, srcDim.height };
		}

		float aspect_ratio = (float)srcDim.width / (float)srcDim.height;
		if (targetDim.width >= targetDim.height)
		{
			new_dim.width = targetDim.width;
			new_dim.height = (int)(targetDim.width / aspect_ratio);
		}
		else
		{
			new_dim.height = targetDim.height;
			new_dim.width = (int)(targetDim.height * aspect_ratio);
		}

		return new_dim;
	}
} // End of namespace simG