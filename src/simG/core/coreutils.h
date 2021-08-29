#pragma once
#include "types.h"
#include "../../../external/clipper/clipper.hpp"

#include <opencv2/core/types.hpp>
#include <opencv2/core/mat.hpp>
#include <vector>

namespace simG {
	using CLPoly = ClipperLib::Path;
	using CLPolys = ClipperLib::Paths;

	namespace coreutils
	{
		void overlayMatrix(cv::Mat& dstMat, const cv::Mat& inputMat, cv::Point location) {}

		void simplifyContour(const std::vector<cv::Point>& srcCntr, std::vector<cv::Point>& dstCntr, float tolerance) {}

		void moveContour(CVContour& cntr, int dx, int dy) {}

		void scaleContour(CVContour& cntrs, float scale)
		{
			auto M = cv::moments(cntrs);
			auto cx = static_cast<int>(M.m10 / M.m00);
			auto cy = static_cast<int>(M.m01 / M.m00);

			for (auto& point : cntrs)
			{
				cv::Point centerP(cx, cy);
				point -= centerP;
				point.x *= scale;
				point.y *= scale;
				point += centerP;
			}
		}

		void contourToMask(cv::Mat& dst, const CVContour& cntr)
		{
			CVContours cntrs{ cntr };
			cv::drawContours(dst, cntrs, -1, cv::Scalar(255, 255, 255), cv::FILLED);
		}

		std::vector<int> flattenContours(const CVContour& cntr)
		{
			std::vector<int> flatten;
			flatten.reserve(cntr.size() * 2);
			for (const auto& point : cntr)
			{
				flatten.push_back(point.x);
				flatten.push_back(point.y);
			}
			return flatten;
		}

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

		std::vector<cv::Point> clipperPolyToCvContour(const ClipperLib::Path& poly)
		{
			std::vector<cv::Point> cvcntr;
			cvcntr.reserve(poly.size());
			for (const auto& point : poly)
			{
				cvcntr.emplace_back(point.X, point.Y);
			}
			return cvcntr;
		}

		bool doContoursIntersect() {}

		double polygonIntersectArea(const CLPoly& poly1, const CLPoly& poly2)
		{
			ClipperLib::Paths intersection;
			ClipperLib::Clipper c;

			c.AddPath(poly1, ClipperLib::ptSubject, true);
			c.AddPath(poly2, ClipperLib::ptClip, true);
			c.Execute(ClipperLib::ctIntersection, intersection, ClipperLib::pftNonZero, ClipperLib::pftNonZero);
			if (!intersection.empty())
			{
				return ClipperLib::Area(intersection[0]);
			}
			return 0.0;
		}

		double polygonArea(const ClipperLib::Path& poly)
		{
			return ClipperLib::Area(poly);
		}

		double polygonArea(std::vector<cv::Point>& polygon, int b)
		{
			return polygonArea(cvContourToClipperPoly(polygon));
		}

		ClipperLib::Path getPolygonIntersection(const ClipperLib::Path& poly1, const ClipperLib::Path& poly2)
		{
			ClipperLib::Paths intersection;
			ClipperLib::Clipper c;

			c.AddPath(poly1, ClipperLib::ptSubject, true);
			c.AddPath(poly2, ClipperLib::ptClip, true);
			c.Execute(ClipperLib::ctIntersection, intersection, ClipperLib::pftNonZero, ClipperLib::pftNonZero);

			return intersection[0];
		}

		ClipperLib::Path clipPolygon(const ClipperLib::Path& srcPoly, const ClipperLib::Path& diffPoly)
		{
			ClipperLib::Paths solution;
			ClipperLib::Clipper c;

			c.AddPath(srcPoly, ClipperLib::ptSubject, true);
			c.AddPath(diffPoly, ClipperLib::ptClip, true);
			c.Execute(ClipperLib::ctDifference, solution, ClipperLib::pftNonZero, ClipperLib::pftNonZero);

			return solution[0];
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
}