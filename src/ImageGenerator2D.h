#pragma once
#include "ImageAugmenter.h"
#include "Annotators/AbstractAnnotator.h"
#include "Utils/Directory.h"
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <opencv2/core/mat.hpp>
#include "Exceptions.h"

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
		};

		struct ImageAugmentationOptions
		{
			bool randomnoise = false;
			BackgroundAugmentations BackgroundAugs;
			MaskAugmentations MaskAugs;
		};
	} // end of namespace internal_

	class ImageGenerator2D
	{
	public:
		typedef internal_::ImageAugmentationOptions AugmentationParams;

		ImageGenerator2D(
			const std::string& maskDir,
			const std::string& backgroundDir,
			int numberImages,
			int objectsPerImage,
			const AugmentationParams& params,
			AbstractAnnotator* imageAnnotator
		);

		ImageGenerator2D(
			const std::string& maskDir,
			const std::string& backgroundDir,
			int numberImages,
			int objectsPerImage,
			const AugmentationParams& params
		);

		~ImageGenerator2D() = default;

		cv::Mat generate();
		bool isValid() const;
		bool hasFinished() const;

		std::stringstream error_log;
		int image_count = 0;

	private:
		void augmentMask(cv::Mat& maskSample) const;
		void augmentBackground(cv::Mat& backgrSample) const;
		void compose(const cv::Mat& srcComp1, const cv::Mat& srcComp2, cv::Mat& dst) const;
		bool overlap() const;

		Directory MaskDir_;
		Directory BckgrndDir_;
		bool valid_flag_ = true;
		int target_images_;
		int obj_per_image_;
		AugmentationParams augparams_;
		ImageAugmenter augmenter_;
		AbstractAnnotator* annotator_;
	};
}
