#pragma once
#include "imgaugmenter.h"
#include "../annotator/AbstractAnnotator.h"
#include "../utils/Directory.h"
#include "transforms.h"

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <opencv2/opencv.hpp>

#ifdef _OPENMP
#define OPENMP_IS_AVAIL 1
#else
#define OPENMP_IS_AVAIL 0
#endif

namespace simG
{
	namespace internal_
	{
		struct RescaleOptions
		{
			bool do_rescale = true;
			Dimensions dimensions{ 1024,576 };
			int interpolation_mode = 0; // TODO cv InterplationFlags
		};

		struct CropOptions
		{
			bool do_crop = true;
			bool keep_aspect = true;
			Dimensions target_dim{ 800, 400 };
		};

		struct RotationOptions
		{
			bool do_rotate = true;
			Range<int> rotation_range{ 0, 360 };
		};

		struct BrightnessOptions
		{
			bool do_shift = true;
			Range<double> brightness_range{ -200, 200.5 };
		};

		struct AxisFlippingOptions
		{
			bool do_random_flip = true;
			// introduces the possibility that the image is not flipped --> random[no_flip, vertical...]
			bool include_no_flip = true;
		};

		struct BackgroundAugmentations
		{
			CropOptions crop;
			BrightnessOptions brightness;
			AxisFlippingOptions axisflipping;
		};

		struct MaskAugmentations
		{
			BrightnessOptions brightness;
			RotationOptions rotation;
			AxisFlippingOptions axisflipping;
		};

		struct ImageAugmentationOptions
		{
			bool randomnoise = false;
			BackgroundAugmentations BackgroundAugs;
			MaskAugmentations MaskAugs;
		};

		class ImageGenerator_iterator
		{
		public:
		protected:
		private:
		};
	} // end of namespace internal_

	enum class ThreadingStatus {
		ADJUST_TO_CPU = 0, DISABLE_THREADING = 1, NUM_THREADS_2 = 2,
		NUM_THREADS_3 = 3, NUM_THREADS_4 = 4, NUM_THREADS_5 = 5,
		NUM_THREADS_6 = 6, NUM_THREADS_7 = 7, NUM_THREADS_8 = 8
	};

	enum TransformTarget {
		APPLY_ON_MASK = 0, APPLY_ON_BCKGROUND = 1, APPLY_ON_RESULT = 2
	};

	struct ImageCompound
	{
		cv::Mat base;
		std::vector<cv::Mat> objects;
		// vector<categories>
		// keypoints ...?
		// savepath??
		// name
	};

	class ImageGenerator
	{
	public:
		typedef internal_::ImageAugmentationOptions AugmentationParams;
		using iterator = internal_::ImageGenerator_iterator;

		ImageGenerator(
			const std::string& maskDir,
			const std::string& backgroundDir,
			const AugmentationParams& params,
			annotators::AbstractAnnotator* imageAnnotator
		);

		ImageGenerator(
			const std::string& maskDir,
			const std::string& backgroundDir,
			const AugmentationParams& params
		);

		ImageGenerator(
			const Directory& maskDir,
			const Directory& bckgndDir,
			ThreadingStatus tStatus,
			annotators::AbstractAnnotator* imageAnnotator
		);

		~ImageGenerator() = default;

		ImageCompound forward(); // return struct of image and annotation (string representation) auto [img, anno]
		//maybe make so truncutate objects as well
		void generate(int targetNumber, annotators::AbstractAnnotator* annotator = nullptr); //Rename to apply(dir, dir, outdir)
		void setInput(const Directory& maskdir, const Directory& bckdir);
		void setOutput(const Directory& out);
		void addTransforms(const transforms::Sequential& transforms, TransformTarget target);
		void setThreading(ThreadingStatus tStatus);
		void setNumberObjects(int lower, int upper);
		bool isThreadingEnabled() const;

	public:
		void runSequential_();
		void runParallel_();
		void preprocess(cv::Mat& mask, cv::Mat& bckgrnd) const;
		void postprocess(cv::Mat& result) const;
		void augmentMask(cv::Mat& maskSample) const;
		void augmentBackground(cv::Mat& backgrSample) const;
		void compose(const cv::Mat& srcComp1, const cv::Mat& srcComp2, cv::Mat& dst) const;
		bool overlap() const;

		Directory maskDir_;
		Directory bckgrDir_;
		simG::Range<int> numberObjects_;
		int numWorkers_ = static_cast<int>(ThreadingStatus::DISABLE_THREADING);

		transforms::Sequential maskTransforms_;
		transforms::Sequential bckgrTransforms_;
		transforms::Sequential resultTransforms_;

		AugmentationParams augparams_;
		ImageAugmenter augmenter_;
		annotators::AbstractAnnotator* annotator_;
	};
}
