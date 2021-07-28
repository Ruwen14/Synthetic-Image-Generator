#include "CocoISAnnotator.h"

namespace simG
{
	CocoISAnnotator::CocoISAnnotator(const std::string& annofile_savepath)
		: AbstractAnnotator(annofile_savepath)
	{
	}

	void CocoISAnnotator::annotateImg(const std::vector<cv::Mat>& masks, const ImgMetaInfo& meta)
	{
		anno_file_ << "CocoIsAnnotor has written";
	}
}