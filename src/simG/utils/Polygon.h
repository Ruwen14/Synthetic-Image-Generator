#pragma once
#include <vector>
#include <opencv2/core/types.hpp>
#include "../../../external/clipper/clipper.hpp"

namespace simG::geometry
{
	typedef ClipperLib::Path PointVector;

	class Polygon
	{
	public:
		Polygon(const PointVector& points, bool closed = true, bool simplify = true);
		Polygon(const std::vector<cv::Point>& points, bool closed = true, bool simplify = true);

		double area();
		size_t size();

		void move(int dx, int dy);
		void scale(double factor);
		void simplify(double tolerance);

		bool contains(const Polygon& poly);
		bool contains(int px, int py);
		bool intersect(const Polygon& poly);
		bool isclosed();

		Polygon intersection(const Polygon& poly);
		Polygon clip(const Polygon& srcPoly, const Polygon& diffPoly);

		inline PointVector& data() { return m_points; }

	protected:
		PointVector m_points;
		bool m_closed;
	};
}
