#pragma once
#include "imageenhancer.h"
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
		template<typename T>
		struct Range
		{
			T lower;
			T upper;
		};

		struct RescaleOptions
		{
			bool do_rescale = true;
			std::pair<int, int> dimensions{ 1024,576 };
			int interpolation_mode = 0; // TODO cv InterplationFlags
		};

		struct RotationOptions
		{
			bool do_rotate = true;
			Range<int> rotation_range{ 0, 360 };
		};

		struct BrightnessOptions
		{
			bool do_shift = true;
			Range<double> brightness_range{ -20.5, 20.5 };
		};

		struct AxisFlippingOptions
		{
			float flip_prob = 1.0;
			bool do_hozizontal_flip = true;
			bool do_vertical_flip = true;
		};

		struct ImageAugmentationOptions
		{
			bool random_noise = true;
			RotationOptions Rotation;
			RescaleOptions Rescaling;
			BrightnessOptions ShiftBrightness;
			AxisFlippingOptions AxisFlipping;
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
		bool is_valid() const;
		bool has_finished() const;

		std::stringstream error_log;
		int image_count = 0;

	private:
		void augment_mask(cv::Mat& mask) const;
		void augment_bckground(cv::Mat& bckr) const;
		void compose() const;
		bool overlap() const;

		Directory MaskDir_;
		Directory BckgrndDir_;
		bool valid_flag_ = true;
		int target_images_;
		int obj_per_image_;
		AugmentationParams AugParams_;
		ImageEnhancer ImgEnhancer_;
		AbstractAnnotator* Annotator_;
	};
}
