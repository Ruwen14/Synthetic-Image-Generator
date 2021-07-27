#pragma once
#include <iostream>
#include <vector>
#include <string>



namespace simG
{
	template<typename T>
	std::vector<T> getResizedVec(const std::vector<T>& srcVec, const std::size_t tSize)
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



	/**
	* @brief Shows composite image from the given images
	* @param imgArray Vector of Images.
	* @param windowHeight The height of the new composite image to be formed.
	* @param nRows Number of rows of images. (Number of columns will be calculated
	*              depending on the value of total number of images).
	*/
	void subplot(const std::vector<cv::Mat>& imgArray, int nRows)
	{
		int windowHeight = 960;
		int N = imgArray.size();
		nRows = nRows > N ? N : nRows;
		int edgeThickness = 10;
		int imagesPerRow = ceil(double(N) / nRows);
		int resizeHeight = floor(2.0 * ((floor(double(windowHeight - edgeThickness) / nRows)) / 2.0)) - edgeThickness;
		int maxRowLength = 0;

		std::vector<int> resizeWidth;
		for (int i = 0; i < N;) {
			int thisRowLen = 0;
			for (int k = 0; k < imagesPerRow; k++) {
				double aspectRatio = double(imgArray[i].cols) / imgArray[i].rows;
				auto temp = int(ceil(resizeHeight * aspectRatio));
				resizeWidth.push_back(temp);
				thisRowLen += temp;
				if (++i == N) break;
			}
			if ((thisRowLen + edgeThickness * (imagesPerRow + 1)) > maxRowLength) {
				maxRowLength = thisRowLen + edgeThickness * (imagesPerRow + 1);
			}
		}
		int windowWidth = maxRowLength;
		cv::Mat canvasImage(windowHeight, windowWidth, CV_8UC3, cv::Scalar(0, 0, 0));

		for (int k = 0, i = 0; i < nRows; i++) {
			int y = i * resizeHeight + (i + 1) * edgeThickness;
			int x_end = edgeThickness;
			for (int j = 0; j < imagesPerRow && k < N; k++, j++) {
				int x = x_end;
				cv::Rect roi(x, y, resizeWidth[k], resizeHeight);
				cv::Size s = canvasImage(roi).size();
				// change the number of channels to three
				cv::Mat target_ROI(s, CV_8UC3);
				if (imgArray[k].channels() != canvasImage.channels()) {
					if (imgArray[k].channels() == 1) {
						cv::cvtColor(imgArray[k], target_ROI, cv::COLOR_GRAY2BGR);
					}
				}
				else {
					imgArray[k].copyTo(target_ROI);
				}
				cv::resize(target_ROI, target_ROI, s);
				if (target_ROI.type() != canvasImage.type()) {
					target_ROI.convertTo(target_ROI, canvasImage.type());
				}
				target_ROI.copyTo(canvasImage(roi));
				x_end += resizeWidth[k] + edgeThickness;
			}
		}
		std::string windowsTitle = 
			"Subplot (" + std::to_string(nRows) + ", " 
			+ std::to_string(imagesPerRow) + ")";
		cv::imshow(windowsTitle, canvasImage);
		cv::waitKey(0);
	}
}

template <typename T>
std::ostream& operator<< (std::ostream& out, const std::vector<T>& v) {
	out << "[";
	for (const auto& ele : v)
	{
		out << ele << ", ";
	}
	out << "\b\b]";
	return out;
}
