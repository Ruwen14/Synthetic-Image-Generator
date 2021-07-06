#include "imggenerator2d.h"
#include "utils/Random.h"
#include <filesystem>
#include <opencv2/highgui.hpp>
#include <omp.h>

namespace simG
{
	ImageGenerator2D::ImageGenerator2D(
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

	ImageGenerator2D::ImageGenerator2D(
		const std::string& maskDir,
		const std::string& backgroundDir,
		int numberImages,
		int objectsPerImage,
		const AugmentationParams& params
	)
		: ImageGenerator2D(maskDir, backgroundDir, numberImages, objectsPerImage, params, nullptr)
	{
	}


	
	cv::Mat ImageGenerator2D::forward()
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

	void ImageGenerator2D::forwardloop()
	{
		if (this->num_workers_ > 1)
		{
			std::cout << "[INFO]: Running Loop with Multithreading[enabled::<" << this->num_workers_ << ">workers]" << "\n";
			runSequential_();
		}
		else
		{
			std::cerr << "[INFO]: Running Loop with Multithreading[disabled]" << ". (Optional) enable it with <setThreading(...)>." << "\n";
			runParallel_();
		}

		
	}


	bool ImageGenerator2D::hasFinished() const
	{
		return this->image_count == this->target_images_;
	}

	void ImageGenerator2D::setThreading(ThreadingStatus tStatus)
	{
		if (tStatus == ThreadingStatus::ADJUST_TO_CPU)
		{
			this->num_workers_ = omp_get_num_procs();
		}
		else
		{
			this->num_workers_ = static_cast<int>(tStatus);
		}
	}

	void ImageGenerator2D::runSequential_()
	{
		while (!hasFinished())
		{
			cv::Mat composit = forward();
		}

	}

	void ImageGenerator2D::runParallel_()
	{
		//list of entries = getResizedVec(Dir.entries)
		//# omp parallel for 
		//for (int i = 0; i < list of entries.size; i++)
		//{

		//}
	}

	void ImageGenerator2D::augmentMask(cv::Mat& maskSample) const
	{
		const auto& mask_params = this->augparams_.MaskAugs;
	}

	void ImageGenerator2D::augmentBackground(cv::Mat& backgrSample) const
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

	void ImageGenerator2D::compose(const cv::Mat& srcComp1, const cv::Mat& srcComp2, cv::Mat& dst) const
	{
	}

	bool ImageGenerator2D::overlap() const
	{
	}
}