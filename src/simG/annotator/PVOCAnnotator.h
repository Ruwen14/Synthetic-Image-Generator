#pragma once
#include "AbstractAnnotator.h"

namespace simG::annotators
{
	enum class PVOCType { OBJ_DET, SEM_SEG };

	class PVOCAnnotator : public AbstractAnnotator
	{
	public:
		PVOCAnnotator() = default;
		//needs virtual destructor
		~PVOCAnnotator() = default;

		void annotateImg(const std::vector<cv::Mat>& masks, const AnnotationDictionary& annotationDict) override;

		void close() override;

	};
}


