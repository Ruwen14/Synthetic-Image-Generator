#pragma once
#include "AbstractAnnotator.h"
namespace simG::annotators
{
	class YoloAnnotator : public AbstractAnnotator
	{
	public:
		YoloAnnotator() = default;
		//needs virtual destructor
		~YoloAnnotator() = default;
	};
}
