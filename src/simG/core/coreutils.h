#pragma once
#include "types.h"
#include "../../../external/clipper/clipper.hpp"
#include <opencv2/opencv.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/core/mat.hpp>
#include <vector>

namespace simG {
	using CLPoly = ClipperLib::Path;
	using CLPolys = ClipperLib::Paths;

	namespace coreutils
	{
		void findContours()
		{
			//cv::findContours(CV_CHAIN_APPROX_TC89_L1)
		}

		void overlayMatrix(cv::Mat& dstMat, const cv::Mat& inputMat, cv::Point location) {}

		void simplifyContour(const std::vector<cv::Point>& srcCntr, std::vector<cv::Point>& dstCntr, float tolerance) {}

		// Check https://github.com/milkbread/Visvalingam-Wyatt
		void visvalingamCurveSimplification(const CVContour& srcCntr, CVContour& dstContr, int targetSize)
		{
			dstContr.reserve(targetSize);
			for (size_t i = 1; i < srcCntr.size() - 1; i++)
			{
				std::cout << "Triangle" << i - 1 << ", " << i << ", " << i + 1 << "\n";
				//	double area_i =
				//		0.5 * ((srcCntr[i - 1].x * srcCntr[i].y) + (srcCntr[i].x * srcCntr[i + 1].y)
				//			+ (srcCntr[i + 1].x * srcCntr[i - 1].y)
				//			- (srcCntr[i - 1].x * srcCntr[i + 1].y)
				//			- (srcCntr[i].x * srcCntr[i - 1].y)
				//			- (srcCntr[i + 1].x * srcCntr[i].y));
			}
		}

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

		void contourToMask(const CVContour& cntr, cv::Mat& mask)
		{
			CVContours cntrs{ cntr };
			cv::drawContours(mask, cntrs, -1, cv::Scalar(255, 255, 255), cv::FILLED);
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

		bool doContoursIntersect() { return true; }

		bool pointInContourRobustButSlow(const CVContour& cntr, cv::Point p)
		{
			return cv::pointPolygonTest(cntr, p, false) == 1.0;
		}

		struct RDPApproxParams
		{
			double epsilon;
			double arcLength;
			double optimalTolerance;
		};

		// finds params for Ramer–Douglas–Peucker algorithm so we can approx a polygon with a fixed size.
		RDPApproxParams optimalRDPParams(const simG::CVContour& cntr, size_t approxSize)
		{
			auto n_iter = 0;
			auto max_iter = 100;

			double lb = 0.;
			double  ub = 1.;

			while (true)
			{
				simG::CVContour dstCntr;

				n_iter += 1;

				auto k = (lb + ub) / 2.;
				auto arclen = cv::arcLength(cntr, true);
				auto eps = k * arclen;

				if (n_iter > max_iter)
				{
					return { eps, arclen, k };
				}

				cv::approxPolyDP(cntr, dstCntr, eps, true);

				if (dstCntr.size() > approxSize)
				{
					lb = (lb + ub) / 2.;
				}
				else if (dstCntr.size() < approxSize)
				{
					ub = (lb + ub) / 2.;
				}
				else
				{
					return { eps, arclen, k };
				}
			}
		}

		bool pointInContourMaybeFasterButNotTested(cv::Point point, const CVContour& cntr)
		{
			int pX = point.x;
			int pY = point.y;
			int i, j, inside = 0;
			int cntrSize = cntr.size();
			for (i = 0, j = cntrSize - 1; i < cntrSize; j = i++)
			{
				if (((cntr[i].y > pY) != (cntr[j].y > pY)) && (pX < (cntr[j].x - cntr[i].x) * (pY - cntr[i].y) / (cntr[j].y - cntr[i].y) + cntr[i].x))
				{
					inside = !inside;
				}
			}
			return inside;
		}

		bool isContourInside(const CVContour& inside, const CVContour& outside)
		{
			return pointInContourMaybeFasterButNotTested(inside[0], outside);
			//return cv::pointPolygonTest(outside, inside[0], false) == 1.0;
		}

		bool fastContourIntersect2(const CVContour& inside, const CVContour& outside)
		{
			for (const auto& point : inside)
			{
				if (pointInContourMaybeFasterButNotTested(point, outside))
				{
					return true;
				}
			}
			return false;

			//for (const auto& insideP : inside)
			//{
			//	if (pointInContourRobustButSlow(outside, insideP));
			//	{
			//		return true;
			//	}
			//}
			//return false;
		}

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

			if (intersection.size() > 1)
			{
				std::sort(intersection.begin(), intersection.end(),
					[](const ClipperLib::Path& a, ClipperLib::Path& b) { return a.size() > b.size(); });
			}

			return intersection[0];
		}

		ClipperLib::Path clipPolygon(const ClipperLib::Path& srcPoly, const ClipperLib::Path& diffPoly)
		{
			ClipperLib::Paths solution;
			ClipperLib::Clipper c;

			c.AddPath(srcPoly, ClipperLib::ptSubject, true);
			c.AddPath(diffPoly, ClipperLib::ptClip, true);
			c.Execute(ClipperLib::ctDifference, solution, ClipperLib::pftNonZero, ClipperLib::pftNonZero);

			if (solution.size() > 1)
			{
				std::sort(solution.begin(), solution.end(),
					[](const ClipperLib::Path& a, ClipperLib::Path& b) { return a.size() > b.size(); });
			}

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