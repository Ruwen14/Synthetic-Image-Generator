#include "imggenerator.h"
#include "../utils/Random.h"
#include <omp.h>

namespace simG
{
	class Compose
	{
	};

	ImageGenerator::ImageGenerator(
		const std::string& maskDir,
		const std::string& backgroundDir,
		const AugmentationParams& params,
		annotators::AbstractAnnotator* imageAnnotator
	)
		: maskDir_(maskDir), bckgrDir_(backgroundDir), numberObjects_{ 1, 5 }, augparams_(params), annotator_(imageAnnotator)
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
		const AugmentationParams& params
	)
		: ImageGenerator(maskDir, backgroundDir, params, nullptr)
	{
	}

	simG::ImageCompound ImageGenerator::forward()
	{
		cv::Mat bckgr_sample = cv::imread(bckgrDir_.relativeFilePath(bckgrDir_.cycleEntry()), cv::IMREAD_COLOR);
		augmentBackground(bckgr_sample);

		//for (int i = 0; i < numberObjects; i++)
		//{
		//}

		//cv::imshow("Window", bckgr_sample);

		//cv::waitKey(0);

		//++image_count;
		//return bckgr_sample;
		return  {};
	}

	void ImageGenerator::generate(int targetNumber, annotators::AbstractAnnotator* annotator /*= nullptr*/)
	{
		if (annotator == nullptr)
		{
			std::cout << "Hi\n";
		}

		if (this->numWorkers_ > 1)
		{
			std::cout << "[INFO]: Running Loop with Multithreading[enabled::<" << this->numWorkers_ << ">workers]" << "\n";
			runParallel_();
		}
		else
		{
			std::cerr << "[INFO]: Running Loop with Multithreading[disabled]" << ". (Optional) enable it with <setThreading(...)>." << "\n";
			runSequential_();
		}
	}

	void ImageGenerator::setInput(const Directory& maskdir, const Directory& bckdir)
	{

	}

	void ImageGenerator::setOutput(const Directory& out)
	{

	}

	void ImageGenerator::addTransforms(const transforms::Sequential& transforms, TransformTarget target)
	{
		switch (target)
		{
		case APPLY_ON_MASK:
			this->maskTransforms_ = transforms;
			break;
		case APPLY_ON_BCKGROUND:
			this->bckgrTransforms_ = transforms;
			break;
		case APPLY_ON_RESULT:
			this->resultTransforms_ = transforms;
			break;
		default:
			break;
		}
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

			this->numWorkers_ = static_cast<int>(ThreadingStatus::DISABLE_THREADING);
		}
		else if (tStatus == ThreadingStatus::ADJUST_TO_CPU)
		{
			this->numWorkers_ = omp_get_num_procs();
		}
		else
		{
			this->numWorkers_ = static_cast<int>(tStatus);
		}
	}

	void ImageGenerator::setNumberObjects(int lower, int upper)
	{
		if ((lower < 1) || (upper < 1))
		{
			std::cout << "lower or upper are not allowed to be smaller than 1\n";
			return;
		}

		if (lower > upper)
		{
			std::cout << "lower is not allowed to be bigger than upper\n";
			return;
		}

		this->numberObjects_.lower = lower;
		this->numberObjects_.upper = upper;
	}

	bool ImageGenerator::isThreadingEnabled() const
	{
		if (this->numWorkers_ > 1) {
			return true;
		}
		return false;
	}

	void ImageGenerator::runSequential_()
	{
		return;
		//while (!hasFinished())
		//{
		//	cv::Mat composit = forward();
		//}
	}

	void ImageGenerator::runParallel_()
	{
		return;
		//list of entries = getResizedVec(Dir.entries)
		//# omp parallel for
		//for (int i = 0; i < list of entries.size; i++)
		//{
		//	forward
		// annoate(imagecompound)
		//}
	}

	void ImageGenerator::preprocess(cv::Mat& mask, cv::Mat& bckgrnd) const
{
		// TODO: augment masks and background
	}

	void ImageGenerator::postprocess(cv::Mat& result) const
{
		// TODO: augment resultimage

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