#include "ImageGenerator.h"
#include <filesystem>

namespace simG
{
	ImageGenerator::ImageGenerator(
		const std::string& templateDir,
		const std::string& wallpaperDir,
		const std::string& saveDir,
		int numberImages,
		const AugmentationParams& params
	)
		: template_dir_(templateDir), wallpaper_dir_(wallpaperDir), save_dir_(saveDir), target_images_(numberImages), aug_params_(params)
	{
	}

	cv::Mat ImageGenerator::generate()
	{
		cv::Mat img;
		return img;
	}

	bool ImageGenerator::is_generator_valid() const
	{
		return is_valid_;
	}

	bool ImageGenerator::has_finished() const
	{
		if (number_generated_ == target_images_)
		{
			return true;
		}
		return false;
	}

	std::vector<std::string> ImageGenerator::aggregatePaths(const std::string& dir) const
	{
		std::vector<std::string> entries;
		for (const auto& entry : std::filesystem::recursive_directory_iterator(dir))
		{
			entries.push_back(entry.path().string());
		}
		return entries;
	}

	void ImageGenerator::iterate() const
	{
	}

	void ImageGenerator::augment() const
	{
	}

	void ImageGenerator::compose() const
	{
	}
}