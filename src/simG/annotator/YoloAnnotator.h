#pragma once
#include "AbstractAnnotator.h"
namespace simG
{
	class YoloAnnotator : public AbstractAnnotator
	{
		YoloAnnotator() = default;
		//needs virtual destructor
		~YoloAnnotator() = default;
	};
}
