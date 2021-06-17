#pragma once
#include <opencv2/core/mat.hpp>

namespace simG {
	class AbstractGenerator
	{
	public:
		AbstractGenerator() = default;
		virtual ~AbstractGenerator() = default;

		virtual cv::Mat generate() = 0;
		virtual bool is_generator_valid() const = 0;
		virtual bool has_finished() const = 0;
	};
}