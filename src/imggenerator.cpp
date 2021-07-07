#include "imggenerator.h"
#include "utils/Random.h"
#include <omp.h>


namespace simG
{
	ImageGenerator::ImageGenerator(
		const std::string& maskDir,
		const std::string& backgroundDir,
		int numberImages,
		int objectsPerImage,
		const AugmentationParams& params,
		AbstractAnnotator* imageAnnotator
	)
		: MaskDir_(maskDir), BckgrndDir_(backgroundDir), target_images_(numberImages), obj_per_image_(objectsPerImage), augparams_(params), annotator_(imageAnnotator)
	{
		//if (MaskDir_.isEmpty())
		//{
		//	throw InvalidDirectoryException(maskDir);
		//}
		//if (BckgrndDir_.isEmpty())
		//{
		//	throw InvalidDirectoryException(backgroundDir);
		//}
//
	}

	ImageGenerator::ImageGenerator(
		const std::string& maskDir,
		const std::string& backgroundDir,
		int numberImages,
		int objectsPerImage,
		const AugmentationParams& params
	)
		: ImageGenerator(maskDir, backgroundDir, numberImages, objectsPerImage, params, nullptr)
	{
	}

	cv::Mat ImageGenerator::forward()
	{
		cv::Mat bckgr_sample = cv::imread(BckgrndDir_.relativeFilePath(BckgrndDir_.cycleEntry()), cv::IMREAD_COLOR);
		augmentBackground(bckgr_sample);

		for (int i = 0; i < obj_per_image_; i++)
		{
		}

		//cv::imshow("Window", bckgr_sample);

		//cv::waitKey(0);

		++image_count;
		return bckgr_sample;
	}

	void ImageGenerator::forwardloop()
	{
		if (this->num_workers_ > 1)
		{
			std::cout << "[INFO]: Running Loop with Multithreading[enabled::<" << this->num_workers_ << ">workers]" << "\n";
			runParallel_();
		}
		else
		{
			std::cerr << "[INFO]: Running Loop with Multithreading[disabled]" << ". (Optional) enable it with <setThreading(...)>." << "\n";
			runSequential_();
		}




	}

	bool ImageGenerator::hasFinished() const
	{
		return this->image_count == this->target_images_;
	}

	void ImageGenerator::setThreading(ThreadingStatus tStatus)
	{
		if (OPENMP_IS_AVAIL == 0 && tStatus != ThreadingStatus::DISABLE_THREADING)
		{
			std::string warningText =
				"[WARNING]: Oops. It looks like you tried to enable threading, but forgot to set the '/openmp' flag during compilation. \n"
				"	   >> Threading was therefore disabled for the rest of the program. \n"
				"	   -- \n"
				"	   If you still want to use threading add '-openmp' to your compilation commands or \n"
				"	   select C/C++->Language, and change 'OpenMP Support' to 'Yes' if using Visual Studio 2019. \n"
				"	   -- \n"
				"	   Otherwise do 'setThreading(simG::ThreadingStatus::DISABLE_THREADING)' to disable the warning. \n";
			std::cout << warningText << "\n";

			this->num_workers_ == static_cast<int>(ThreadingStatus::DISABLE_THREADING);
		}
		else if (tStatus == ThreadingStatus::ADJUST_TO_CPU)
		{
			this->num_workers_ = omp_get_num_procs();
		}
		else
		{
			this->num_workers_ = static_cast<int>(tStatus);
		}
	}

	void ImageGenerator::runSequential_()
	{
		return;
		while (!hasFinished())
		{
			cv::Mat composit = forward();
		}
	}

	void ImageGenerator::runParallel_()
	{
		return;
		//list of entries = getResizedVec(Dir.entries)
		//# omp parallel for
		//for (int i = 0; i < list of entries.size; i++)
		//{
		//}
	}

	void ImageGenerator::augmentMask(cv::Mat& maskSample) const
	{
		const auto& mask_params = this->augparams_.MaskAugs;
	}

	void ImageGenerator::augmentBackground(cv::Mat& backgrSample) const
	{
		const auto& background_parms = this->augparams_.BackgroundAugs;

		//this->enhancer_.rescale(backgrSample, augparams_.rescaling.dimensions);
		if (background_parms.crop.do_crop)
		{
			backgrSample = this->augmenter_.randomCrop(
				backgrSample,
				background_parms.crop.target_dim,
				background_parms.crop.keep_aspect);
		}

		// AUGMENTATION: FLIP-IMAGE
		if (background_parms.axisflipping.do_random_flip)
		{
			this->augmenter_.randomFlip(
				backgrSample,
				background_parms.axisflipping.include_no_flip);
		}

		// AUGMENTATION: CHANGE BRIGTHNESS
		if (background_parms.brightness.do_shift)
		{
			this->augmenter_.randomBrightness(
				backgrSample,
				background_parms.brightness.brightness_range);
		}

		// Add rotation 180°
	}

	void ImageGenerator::compose(const cv::Mat& srcComp1, const cv::Mat& srcComp2, cv::Mat& dst) const
	{
	}

	bool ImageGenerator::overlap() const
	{
	}

}