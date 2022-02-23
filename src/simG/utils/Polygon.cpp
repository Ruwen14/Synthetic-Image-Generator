#include "Polygon.h"

namespace simG::geometry
{
	Polygon::Polygon(const PointVector& points, bool closed /*= true*/, bool simplify /*= true*/)
	{
		//https://stackoverflow.com/questions/23929800/is-stdmove-really-needed-on-initialization-list-of-constructor-for-heavy-membe
		// : m_points(std::move(points))??  Only if often passed an rvalue(temporary)
		if (simplify)
		{
			//simplyfy
		}
	}

	Polygon::Polygon(const std::vector<cv::Point>& points, bool closed /*= true*/, bool simplify /*= true*/)
	{
		// : m_points(std::move(points))??
		if (simplify)
		{
			//simplyfy
		}
	}

	double Polygon::area()
	{
	}

	size_t Polygon::size()
	{
		return m_points.size();
	}

	void Polygon::move(int dx, int dy)
	{
	}

	void Polygon::scale(double factor)
	{
	}

	void Polygon::simplify(double tolerance)
	{
	}

	bool Polygon::contains(const Polygon& poly)
	{
	}

	bool Polygon::contains(int px, int py)
	{
	}

	bool Polygon::intersect(const Polygon& poly)
	{
	}

	bool Polygon::isclosed()
	{
	}

	Polygon Polygon::intersection(const Polygon& poly)
	{
		//https://stackoverflow.com/questions/12953127/what-are-copy-elision-and-return-value-optimization
		//https://en.cppreference.com/w/cpp/language/move_constructor
		//return Polygon for NRVO
	}

	Polygon Polygon::clip(const Polygon& srcPoly, const Polygon& diffPoly)
	{
		//https://stackoverflow.com/questions/12953127/what-are-copy-elision-and-return-value-optimization
		//https://en.cppreference.com/w/cpp/language/move_constructor
		//return Polygon for NRVO
	}
}