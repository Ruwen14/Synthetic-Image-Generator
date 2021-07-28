#pragma once
#include <string>
#include <iostream>
#include <opencv2/core/mat.hpp>

namespace simG
{
	enum class FlipMode { VERTICAL_FLIP = 0, HORIZONTAL_FLIP = 1, HORIZONTAL_AND_VERTICAL_FLIP = -1, NO_FLIP = 2 };
	enum class RotationMode { ROTATE_90_CLCKWISE = 0, ROTATE_180 = 1, ROTATE_90_CNTRCLCKWISE = 2 };
	enum class ResizeMode { NEAREST = 0, LINEAR = 1, CUBIC = 2, AREA = 3, FASTEST = 4, AUTO = 5 };

	struct Dimensions
	{
		int width;
		int height;
	};

	template<typename T>
	struct Range
	{
		T lower;
		T upper;
	};

	class ImageAugmenter
	{
	public:
		ImageAugmenter() = default;
		~ImageAugmenter() = default;

		void rescale(cv::Mat& img, Dimensions dim, ResizeMode resMode, bool keepAspectRatio = false) const;

		cv::Mat rotate(const cv::Mat& img, double degree) const;
		cv::Mat rotate(const cv::Mat& img, RotationMode rm) const;
		cv::Mat randomRotate(const cv::Mat& img, Range<int> degreeRange) const;

		void flip(cv::Mat& img, FlipMode fm) const;
		void randomFlip(cv::Mat& img, bool includeNoFlip = true) const;

		void shiftBrightness(cv::Mat& img, double beta, double alpha = 1) const;
		void randomBrightness(cv::Mat& img, Range<double> brightnessRange) const;

		cv::Mat crop(const cv::Mat& img, const cv::Rect& roi) const;
		cv::Mat randomCrop(const cv::Mat& img, const Dimensions& maxCropDim, bool keepAspectRatio = true) const;

		// Expensive. Up to 10ms per image.
		cv::Mat addGaussianNoise(const cv::Mat& img, double mean = 0.0, double stdDev = 10.0) const;

	public:
		Dimensions calcPreservedAspectRatio(const Dimensions& srcDim, const Dimensions& targetDim) const;

		int default_interp = 0;
	};
}