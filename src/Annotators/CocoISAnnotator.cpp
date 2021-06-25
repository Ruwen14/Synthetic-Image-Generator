#include "CocoISAnnotator.h"

namespace simG
{
	CocoISAnnotator::CocoISAnnotator(const std::string& annofile_savepath)
		: AbstractAnnotator(annofile_savepath)
	{
	}

	void CocoISAnnotator::annotateImg(const SynImg& synth_img, const SynImgMetaInfo& meta)
	{
		anno_file_ << "CocoIsAnnotor has written";
	}
}