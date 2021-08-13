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

		void translateContour(std::vector<cv::Point>& contour, int dx, int dy);

		void simplifyContour(const std::vector<cv::Point>& srcCntr, std::vector<cv::Point>& dstCntr, float tolerance);

		// from https://stackoverflow.com/questions/440133/how-do-i-create-a-random-alpha-numeric-string-in-c
		std::string pseudoUniqueIdentifier(size_t length);

		template<typename T>
		std::vector<T> resizeVec(const std::vector<T>& srcVec, const std::size_t tSize)
		{
			std::vector<T> dstVec;
			dstVec = srcVec;

			if (tSize == srcVec.size())
			{
				return dstVec;
			}

			if (tSize < srcVec.size())
			{
				dstVec.resize(tSize);
				return dstVec;
			}

			dstVec.reserve(tSize);
			while (dstVec.size() < tSize)
			{
				if ((dstVec.size() + srcVec.size()) <= tSize)
				{
					dstVec.insert(dstVec.end(), srcVec.begin(), srcVec.end());
				}
				else
				{
					std::vector<T> slicedVec = std::vector<T>(
						srcVec.begin(),
						srcVec.begin() + (tSize - dstVec.size())
						);
					dstVec.insert(dstVec.end(), slicedVec.begin(), slicedVec.end());
				}
			}

			return dstVec;
		}
	}
}
