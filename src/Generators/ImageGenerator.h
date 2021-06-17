#pragma once
#include "AbstractGenerator.h"
#include "../imageenhancer.h"
#include <iostream>
#include <string>
#include <vector>
#include <sstream>

namespace simG
{
	namespace internal_
	{
		struct FloatRange
		{
			float lower;
			float upper;
		};

		struct RescaleOptions
		{
			bool do_rescale = true;
			float probability = 0.5;
			std::pair<int, int> dimensions{ 1280,960 };
			int interpolation_mode = 0; // TODO cv InterplationFlags
		};

		struct RotationOptions
		{
			bool do_rotate = true;
			float probability = 1.0;
			FloatRange rotation_range{ -0.2f, 0.2f };
		};

		struct BrightnessOptions
		{
			bool do_shift = true;
			float probability = 1.0;
			FloatRange brightness_range{ 0.75f, 1.25f };
		};

		struct AxisFlippingOptions
		{
			bool do_hozizontal_flip = true;
			bool do_vertical_flip = true;
			float horizontal_flip_prob = 0.5;
			float vetical_flip_prob = 0.5;
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

	class ImageGenerator : public AbstractGenerator
	{
	public:
		typedef internal_::ImageAugmentationOptions AugmentationParams;

		ImageGenerator(
			const std::string& templateDir,
			const std::string& wallpaperDir,
			const std::string& saveDir,
			int numberImages,
			const AugmentationParams& params
		);
		~ImageGenerator() = default;

		cv::Mat generate() override;
		bool is_generator_valid() const override;
		bool has_finished() const override;

		std::stringstream error_log;

	public:
		std::vector<std::string> aggregatePaths(const std::string& dir) const;
		void iterate() const;
		void augment() const;
		void compose() const;
		std::string template_dir_;
		std::string wallpaper_dir_;
		std::string save_dir_;
		bool is_valid_ = true;
		int target_images_;
		int number_generated_ = 0;
		AugmentationParams aug_params_;
		ImageEnhancer ImgEnhancer_;
	};
}
