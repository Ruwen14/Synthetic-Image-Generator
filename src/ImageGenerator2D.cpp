#include "ImageGenerator2D.h"
#include "Utils/Random.h"
#include <filesystem>
#include <opencv2/highgui.hpp>

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
		: MaskDir_(maskDir), BckgrndDir_(backgroundDir), target_images_(numberImages), obj_per_image_(objectsPerImage), AugParams_(params), Annotator_(imageAnnotator)
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

	cv::Mat ImageGenerator2D::generate()
	{
		cv::Mat bckgr_sample = cv::imread(BckgrndDir_.relativeFilePath(BckgrndDir_.cycleEntry()), cv::IMREAD_COLOR);
		augment_bckground(bckgr_sample);

		for (int i = 0; i < obj_per_image_; i++)
		{
		}

		//cv::imshow("Window", bckgr_sample);

		//cv::waitKey(0);

		++image_count;
		return bckgr_sample;
	}

	bool ImageGenerator2D::is_valid() const
	{
		return this->valid_flag_;
	}

	bool ImageGenerator2D::has_finished() const
	{
		return this->image_count == this->target_images_;
	}

	void ImageGenerator2D::augment_mask(cv::Mat& mask) const
	{
	}

	void ImageGenerator2D::augment_bckground(cv::Mat& bckr) const
	{
		ImgEnhancer_.rescale(bckr, AugParams_.Rescaling.dimensions);

		// AUGMENTATION: FLIP-IMAGE
		auto flip_params = this->AugParams_.AxisFlipping;
		if (Random::randomProb(flip_params.flip_prob))
		{
			if (flip_params.do_hozizontal_flip && flip_params.do_vertical_flip)
			{
				ImgEnhancer_.flip(bckr, FlipMode::HORIZONTAL_AND_VERTICAL_FLIP);
			}
			else if (flip_params.do_hozizontal_flip)
			{
				ImgEnhancer_.flip(bckr, FlipMode::HORIZONTAL_FLIP);
			}
			else if (flip_params.do_vertical_flip)
			{
				ImgEnhancer_.flip(bckr, FlipMode::VERTICAL_FLIP);
			}
		}

		// AUGMENTATION: CHANGE BRIGTHNESS
		auto brigthness_params = this->AugParams_.ShiftBrightness;
		if (brigthness_params.do_shift)
		{
			double beta = Random::uniformDouble(brigthness_params.brightness_range.lower, brigthness_params.brightness_range.upper);
			this->ImgEnhancer_.shiftBrightness(bckr, beta, 1);
		}

		//// AUGMENTATION: Rotate
		//auto rotation_params = this->AugParams_.Rotation;
		//if (rotation_params.do_rotate)
		//{
		//	double degree = Random::uniformInt(rotation_params.rotation_range.lower, rotation_params.rotation_range.upper);
		//	bckr = this->ImgEnhancer_.rotate(bckr, degree);
		//}

	}

	void ImageGenerator2D::compose() const
	{
	}

	bool ImageGenerator2D::overlap() const
	{
	}
}