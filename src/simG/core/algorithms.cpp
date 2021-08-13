#include "algorithms.h"
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <random>

namespace simG
{
	namespace algorithms
	{
		void composeMatrices(cv::Mat& dstMat, const cv::Mat& inputMat, cv::Point2i location)
		{
			auto mask_channels = inputMat.channels();
			auto background_channels = dstMat.channels();

			// start at the row indicated by location, or at row 0 if location.y is negative.
			for (int y = std::max(location.y, 0); y < dstMat.rows; ++y)
			{
				int fY = y - location.y; // because of the translation

				// we are done of we have processed all rows of the foreground image.
				if (fY >= inputMat.rows)
					break;

				// start at the column indicated by location,

				// or at column 0 if location.x is negative.
				for (int x = std::max(location.x, 0); x < dstMat.cols; ++x)
				{
					int fX = x - location.x; // because of the translation.

					// we are done with this row if the column is outside of the foreground image.
					if (fX >= inputMat.cols)
						break;

					// determine the opacity of the foregrond pixel, using its fourth (alpha) channel.
					double opacity = (static_cast<double>(inputMat.data[fY * inputMat.step + fX * mask_channels + 3])) / 255.;

					// and now combine the background and foreground pixel, using the opacity,

					// but only if opacity > 0.
					for (int c = 0; opacity > 0 && c < background_channels; ++c)
					{
						unsigned char foregroundPx = inputMat.data[fY * inputMat.step + fX * mask_channels + c];
						unsigned char backgroundPx = dstMat.data[y * dstMat.step + x * background_channels + c];
						dstMat.data[y * dstMat.step + background_channels * x + c] = backgroundPx * (1. - opacity) + foregroundPx * opacity;
					}
				}
			}
		}

		bool ccw(const cv::Point& A, const cv::Point& B, const cv::Point& C)
		{
			return ((C.y - A.y) * (B.x - A.x)) > ((B.y - A.y) * (C.x - A.x));
		}

		bool fastContourIntersect(const std::vector<cv::Point>& refContour, const std::vector<cv::Point>& queryContour)
		{
			for (size_t rIdx = 0; rIdx < refContour.size() - 1; rIdx++)
			{
				auto A = refContour[rIdx];
				auto B = refContour[rIdx + 1];

				for (size_t qIdx = 0; qIdx < queryContour.size() - 1; qIdx++)
				{
					auto C = queryContour[qIdx];
					auto D = queryContour[qIdx + 1];
					if ((ccw(A, C, D) != ccw(B, C, D)) && (ccw(A, B, C) != ccw(A, B, D)))
					{
						return true;
					}
				}
			}
			return false;
		}

		cv::Mat generateZeroMatrix(int width, int height)
		{
			return cv::Mat::zeros(cv::Size(width, height), CV_8UC1);
		}

		void translateContour(std::vector<cv::Point>& contour, int dx, int dy)
		{
			for (auto& point : contour)
			{
				point.x += dx;
				point.y += dy;
			}
		}

		void simplifyContour(const std::vector<cv::Point>& srcCntr, std::vector<cv::Point>& dstCntr, float tolerance)
		{
			auto eps = tolerance * cv::arcLength(srcCntr, true);
			//std::cout << eps <<"\n";
			cv::approxPolyDP(srcCntr, dstCntr, 1, true);
		}

		std::string pseudoUniqueIdentifier(size_t length)
		{
			static auto& chrs = "0123456789"
				"abcdefghijklmnopqrstuvwxyz"
				"ABCDEFGHIJKLMNOPQRSTUVWXYZ";

			static thread_local  std::mt19937 rg{ std::random_device{}() };
			static thread_local  std::uniform_int_distribution<std::string::size_type> pick(0, sizeof(chrs) - 2);

			std::string s;

			s.reserve(length);

			while (length--)
				s += chrs[pick(rg)];

			return s;
		}

	}
}