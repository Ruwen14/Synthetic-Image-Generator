#pragma once
#include <string>
#include <opencv2/core/mat.hpp>
#include <opencv2/imgcodecs.hpp>

namespace simG
{
	enum class FlipMode { VERTICAL_FLIP = 0, HORIZONTAL_FLIP = 1, HORIZONTAL_AND_VERTICAL_FLIP = -1 };

	class ImageEnhancer
	{
	public:
		ImageEnhancer() = default;
		~ImageEnhancer() = default;

		void rescale(cv::Mat& img, std::pair<int, int> dim) const;
		cv::Mat rotate(const cv::Mat& img, double degree) const;
		void flip(cv::Mat& img, FlipMode fm) const;
		void shiftBrightness(cv::Mat& img, double beta, double alpha = 1) const;
	private:
		int default_interp = 0;
	};
}