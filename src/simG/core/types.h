#pragma once
#include <string>
#include <vector>
#include <opencv2/core/types.hpp>

namespace simG
{
	struct Pos
	{
		int x;
		int y;
	};

	struct BoundingBox
	{
		int x;
		int y;
		int w;
		int h;
	};

	struct Dims
	{
		int width;
		int height;
	};

	template<typename T>
	struct Rang
	{
		T lower;
		T upper;
	};

	using Rectangle = cv::Rect;
	using CVContour = std::vector<cv::Point>;
	using CVContours = std::vector<std::vector<cv::Point>>;

	class ContentMargins {
	public:
		ContentMargins() : leftF(0.0), topF(0.0), rightF(0.0), bottomF(0.0) {}
		ContentMargins(double leftFactor, double topFactor, double rightFactor, double bottomFactor)
			:leftF(leftFactor), topF(topFactor), rightF(rightF), bottomF(bottomFactor)
		{}

		Rectangle getBoundsRelativeTo(const Rectangle& contentbounds) {
			auto upperleftX = contentbounds.x + static_cast<int>(leftF * static_cast<double>(contentbounds.width));
			auto upperleftY = contentbounds.y + static_cast<int>(topF * static_cast<double>(contentbounds.height));
			auto bottomrightX = contentbounds.height + static_cast<int>(rightF * static_cast<double>(contentbounds.width));
			auto bottomrightY = contentbounds.height + static_cast<int>(bottomF * static_cast<double>(contentbounds.height));

			return { upperleftX, upperleftY, bottomrightX, bottomrightY };
		}

	public:
		double leftF;
		double topF;
		double rightF;
		double bottomF;
	};
}