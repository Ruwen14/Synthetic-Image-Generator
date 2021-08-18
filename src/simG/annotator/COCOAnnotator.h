#pragma once
#include "AbstractAnnotator.h"
namespace simG::annotators
{
	//enum COCOType { OBJ_DET, KEYPOINTS, PANOPTIC, INSTANCE_SEG };

	// Annotates in the Coco-Instance-Segmentation-Format
	class COCOAnnotator : public AbstractAnnotator
	{
	public:
		enum Format { OBJ_DET, KEYPOINTS, PANOPTIC, INSTANCE_SEG };

		COCOAnnotator(const std::string& annofile_savepath, Format f);
		~COCOAnnotator() { std::cout << "CocoIsAnnotator destroyed" << "\n"; }

		void annotateImg(const std::vector<cv::Mat>& masks, const AnnotationDictionary& annotationDict) override;
	};
}
