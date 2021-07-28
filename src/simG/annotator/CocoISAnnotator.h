#pragma once
#include "AbstractAnnotator.h"
namespace simG
{
	// Annotates in the Coco-Instance-Segmentation-Format
	class CocoISAnnotator : public AbstractAnnotator
	{
	public:
		CocoISAnnotator(const std::string& annofile_savepath);
		~CocoISAnnotator() { std::cout << "CocoIsAnnotator destroyed" << "\n"; }

		void annotateImg(const std::vector<cv::Mat>& masks, const ImgMetaInfo& meta) override;
	};
}