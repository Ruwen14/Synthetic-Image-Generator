#pragma once
#include "imgaugmenter.h"
#include "annotators/AbstractAnnotator.h"
#include "utils/Directory.h"

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

	class ImageGenerator
	{
	public:
		typedef internal_::ImageAugmentationOptions AugmentationParams;
		using iterator = internal_::ImageGenerator_iterator;

		ImageGenerator(
			const std::string& maskDir,
			const std::string& backgroundDir,
			int numberImages,
			int objectsPerImage,
			const AugmentationParams& params,
			AbstractAnnotator* imageAnnotator
		);

		ImageGenerator(
			const std::string& maskDir,
			const std::string& backgroundDir,
			int numberImages,
			int objectsPerImage,
			const AugmentationParams& params
		);

		~ImageGenerator() = default;

		cv::Mat forward();
		void run();
		bool hasFinished() const;
		void setThreading(ThreadingStatus tStatus);

		int image_count = 0;

	protected:
		void runSequential_();
		void runParallel_();
		void augmentMask(cv::Mat& maskSample) const;
		void augmentBackground(cv::Mat& backgrSample) const;
		void compose(const cv::Mat& srcComp1, const cv::Mat& srcComp2, cv::Mat& dst) const;
		bool overlap() const;

		Directory MaskDir_;
		Directory BckgrndDir_;
		bool valid_flag_ = true;
		int target_images_;
		int obj_per_image_;
		int num_workers_ = static_cast<int>(ThreadingStatus::DISABLE_THREADING);
		AugmentationParams augparams_;
		ImageAugmenter augmenter_;
		AbstractAnnotator* annotator_;

	private:
		void begin() const;
		void end() const;
	};
}
