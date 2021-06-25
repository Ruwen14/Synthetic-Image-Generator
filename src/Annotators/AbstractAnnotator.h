#pragma once
#include <string>
#include <opencv2/core/mat.hpp>
#include <fstream>
#include <iostream>

namespace simG
{
	typedef std::ofstream AnnotationFile;
	struct SynImgMetaInfo
	{
	};

	struct SynImg
	{
		cv::Mat result;
		cv::Mat mask_component;
		cv::Mat bgrnd_component;
	};

	class AbstractAnnotator
	{
	public:

		AbstractAnnotator(const std::string& annofile_savepath);
		virtual ~AbstractAnnotator() { std::cout << "AbstractAnnotator destroyed" << "\n"; }

		virtual void annotateImg(const SynImg& synth_img, const SynImgMetaInfo& meta) = 0;
		virtual void close();

	protected:
		AnnotationFile anno_file_;
	};
}
