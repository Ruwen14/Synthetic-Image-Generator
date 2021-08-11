#pragma once
#include <string>
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
}