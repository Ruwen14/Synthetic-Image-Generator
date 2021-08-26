#pragma once
#include "../../../external/clipper/clipper.hpp"

#include <opencv2/core/types.hpp>
#include <opencv2/core/mat.hpp>
#include <vector>

namespace simG::internals
{

	void overlayMat(cv::Mat& dstMat, const cv::Mat& inputMat, cv::Point location) {}

	void simplifyContour(const std::vector<cv::Point>& srcCntr, std::vector<cv::Point>& dstCntr, float tolerance) {}

	void moveCntr(std::vector<cv::Point>& contour, int dx, int dy) {}

	ClipperLib::Path cvContourToClipperPoly(const std::vector<cv::Point>& cntr)
	{
		ClipperLib::Path clipperpoly;
		clipperpoly.reserve(cntr.size());
		for (const auto& point : cntr)
		{
			clipperpoly.emplace_back(point.x, point.y);
		}
		return clipperpoly;
	}

	bool contourIntersect() {}

	double polyIntersectArea(const ClipperLib::Path& poly1, const ClipperLib::Path& poly2)
	{
		ClipperLib::Paths intersection;
		ClipperLib::Clipper c;

		c.AddPath(poly1, ClipperLib::ptSubject, true);
		c.AddPath(poly2, ClipperLib::ptClip, true);
		c.Execute(ClipperLib::ctIntersection, intersection, ClipperLib::pftNonZero, ClipperLib::pftNonZero);
		if (intersection.size() != 0)
		{
			return ClipperLib::Area(intersection[0]);
		}
		return 0.0;
	}






	template <typename T>
	class Cycle
	{
	public:
		Cycle(const std::vector<T>& vec)
			: entries_(vec), curidx_(-1) {}

		void reset()
		{
			curidx_ = -1;
		}

		bool reachedEnd() const
		{
			return curidx_ == entries_.size() - 1;
		}

		T next()
		{
			if (reachedEnd())
				reset();

			++curidx_;
			return entries_[curidx_];
		};

	protected:
		std::vector<T> entries_;
		int curidx_;
	};
}
