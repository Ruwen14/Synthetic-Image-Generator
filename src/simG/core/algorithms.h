#pragma once
#include <opencv2/core/types.hpp>
#include <opencv2/core/mat.hpp>

namespace simG
{
	namespace algorithms
	{
		/** @brief Overlays/Composes two matrices together.
		*@param dstMat:  destination Matrix. Will be mutated!
		*@param inputMat:  input Matrix.
		*@param location:  x/y-coordinates where overlay will happen.
		*/
		void composeMatrices(cv::Mat& dstMat, const cv::Mat& inputMat, cv::Point2i location);

		/** @brief Checks if two Arrays/Vectors of Points intersect.
		*@param refContour:  Vector 1.
		*@param queryContour:  Vector 2.
		*@return true if intersect; otherwise false
		*/
		bool fastContourIntersect(const std::vector<cv::Point>& refContour, const std::vector<cv::Point>& queryContour);

		cv::Mat generateZeroMatrix(int width, int height);
	}


}
