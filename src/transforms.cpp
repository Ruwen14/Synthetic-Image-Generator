#include "transforms.h"
#include "Utils/Random.h"
#include <sstream>

namespace simG
{
	namespace transforms
	{
		cv::Mat RandomRotation::operator()(cv::Mat& srcSample) const
		{
			cv::Mat dst;
			double degree = Random::uniformInt(rotation_range_.lower, rotation_range_.upper);

			if (degree == 0)
			{
				return srcSample;
			}
			// get rotation matrix for rotating the image around its center in pixel coordinates
			cv::Point2f image_center(static_cast<float>(srcSample.cols / 2.0), static_cast<float>(srcSample.rows / 2.0));
			cv::Mat rotation_matrix = cv::getRotationMatrix2D(image_center, degree, 1.0);

			// determine bounding rectangle
			cv::Rect2f bbox = cv::RotatedRect(cv::Point2f(), srcSample.size(), static_cast<float>(degree)).boundingRect2f();

			// adjust transformation matrix
			rotation_matrix.at<double>(0, 2) += bbox.width / 2.0 - srcSample.cols / 2.0;
			rotation_matrix.at<double>(1, 2) += bbox.height / 2.0 - srcSample.rows / 2.0;

			// transform
			cv::warpAffine(srcSample, dst, rotation_matrix, bbox.size());

			return dst;
		}

		cv::Mat RandomRotation90::operator()(cv::Mat& srcSample) const
		{
			if (Random::randomProb(chance_))
			{
				cv::Mat dst;
				cv::rotate(srcSample, dst, 0);
				return dst;
			}
			return srcSample;
		}

		cv::Mat RandomRotation180::operator()(cv::Mat& srcSample) const
		{
			if (Random::randomProb(chance_))
			{
				cv::Mat dst;
				cv::rotate(srcSample, dst, 1);
				return dst;
			}
			return srcSample;
		}
		cv::Mat RandomRotation270::operator()(cv::Mat& srcSample) const
		{
			if (Random::randomProb(chance_))
			{
				cv::Mat dst;
				cv::rotate(srcSample, dst, 2);
				return dst;
			}
			return srcSample;
		}

		cv::Mat RandomHorizontalFlip::operator()(cv::Mat& srcSample) const
		{
			if (Random::randomProb(chance_))
			{
				cv::flip(srcSample, srcSample, 1);
			}
			return srcSample;
		}

		cv::Mat RandomVerticalFlip::operator()(cv::Mat& srcSample) const
		{
			if (Random::randomProb(chance_))
			{
				cv::flip(srcSample, srcSample, 0);
			}
			return srcSample;
		}

		cv::Mat RandomCrop::operator()(cv::Mat& srcSample) const
		{
			cv::Size img_size = srcSample.size();
			Dimensions max_crop_dim;

			if (this->keep_aspect_)
			{
				max_crop_dim = calcPreservedAspectRatio({ img_size.width, img_size.height }, this->crop_dim_);
			}
			else
			{
				max_crop_dim = this->crop_dim_;
			}

			assert(img_size.width >= max_crop_dim.width && img_size.height >= max_crop_dim.height);

			int max_x = img_size.width - max_crop_dim.width;
			int max_y = img_size.height - max_crop_dim.height;

			int crop_x = Random::uniformInt(0, max_x);
			int crop_y = Random::uniformInt(0, max_y);

			cv::Rect roi(crop_x, crop_y, max_crop_dim.width, max_crop_dim.height);

			cv::Mat cropped_img = srcSample(roi);
			return cropped_img;
		}

		simG::transforms::Dimensions RandomCrop::calcPreservedAspectRatio(Dimensions srcDim, Dimensions targetDim) const
		{
			Dimensions new_dim;

			if (srcDim.width == targetDim.width && srcDim.height == targetDim.height)
			{
				return { srcDim.width, srcDim.height };
			}

			float aspect_ratio = (float)srcDim.width / (float)srcDim.height;
			if (targetDim.width >= targetDim.height)
			{
				new_dim.width = targetDim.width;
				new_dim.height = (int)(targetDim.width / aspect_ratio);
			}
			else
			{
				new_dim.height = targetDim.height;
				new_dim.width = (int)(targetDim.height * aspect_ratio);
			}

			return new_dim;
		}

		cv::Mat Resize::operator()(cv::Mat& srcSample) const
		{
			cv::Size img_size = srcSample.size();

			if (img_size.width == this->dim_.width && img_size.height == this->dim_.height)
			{
				return srcSample;
			}

			if (this->keep_aspect_)
			{
				Dimensions new_dim = calcPreservedAspectRatio({ img_size.width, img_size.height }, this->dim_);
				cv::resize(srcSample, srcSample, cv::Size(new_dim.width, new_dim.height));
				return srcSample;
			}
			cv::resize(srcSample, srcSample, cv::Size(this->dim_.width, this->dim_.height));
			return srcSample;
		}

		simG::transforms::Dimensions Resize::calcPreservedAspectRatio(Dimensions srcDim, Dimensions targetDim) const
		{
			Dimensions new_dim;

			if (srcDim.width == targetDim.width && srcDim.height == targetDim.height)
			{
				return { srcDim.width, srcDim.height };
			}

			float aspect_ratio = (float)srcDim.width / (float)srcDim.height;
			if (targetDim.width >= targetDim.height)
			{
				new_dim.width = targetDim.width;
				new_dim.height = (int)(targetDim.width / aspect_ratio);
			}
			else
			{
				new_dim.height = targetDim.height;
				new_dim.width = (int)(targetDim.height * aspect_ratio);
			}

			return new_dim;
		}

		cv::Mat RandomBrightness::operator()(cv::Mat& srcSample) const
		{
			double beta = Random::uniformDouble(beta_r_.lower, beta_r_.upper);
			if (beta == 0)
			{
				return srcSample;
			}
			//std::cout << beta << "\n";
			srcSample.convertTo(srcSample, -1, 1.0, beta);

			return srcSample;
		}

		cv::Mat RandomGaussNoise::operator()(cv::Mat& srcSample) const
		{
			if (Random::randomProb(chance_))
			{
				cv::Mat mSrc_16SC;
				cv::Mat dst;
				cv::Mat mGaussian_noise = cv::Mat(srcSample.size(), CV_16SC3);
				randn(mGaussian_noise, cv::Scalar::all(this->mean_), cv::Scalar::all(this->standard_dev_));

				srcSample.convertTo(mSrc_16SC, CV_16SC3);
				cv::addWeighted(mSrc_16SC, 1.0, mGaussian_noise, 1.0, 0.0, mSrc_16SC);
				mSrc_16SC.convertTo(dst, srcSample.type());

				return dst;
			}
			return srcSample;
		}

		cv::Mat GaussianBlur::operator()(cv::Mat& srcSample) const
		{
			if (Random::randomProb(chance_))
			{
				cv::Mat dst;
				cv::GaussianBlur(srcSample, dst, kernelsize_, 0);
				return dst;
			}
			return srcSample;
		}

		TransformsBucket::TransformsBucket(const std::vector<Transformers>& transformsList)
		{
			populate(transformsList);
		}
		
		void TransformsBucket::apply(const cv::Mat& src, cv::Mat& dst) const
		{
			dst = src;
			for (const auto& transformation : transforms_)
			{
				dst = transformation->operator()(dst);
			}
		}

		void TransformsBucket::operator()(const cv::Mat& src, cv::Mat& dst) const
		{
			apply(src, dst);
		}

		int TransformsBucket::count() const
		{
			return transforms_.size();
		}

		std::string TransformsBucket::dump() const
		{
			std::stringstream out;
			out << typeid(*this).name() << " {\n";
			int pos = 0;
			for (const auto& trans : transforms_)
			{
				out << "\tINDEX <" << pos << ">: " << trans->getType() << "\n";
				++pos;
			}
			out << "}\n";

			return out.str();
		}

		void TransformsBucket::populate(const std::vector<Transformers>& transforms_list)
		{
			for (const auto& curr_transform : transforms_list)
			{
				auto type_index = curr_transform.index();

				switch (type_index)
				{
				case 0:
					transforms_.push_back(
						std::make_unique<RandomRotation>(std::get<RandomRotation>(curr_transform))
					);
					break;
				case 1:
					transforms_.push_back(
						std::make_unique<RandomRotation90>(std::get<RandomRotation90>(curr_transform))
					);
					break;
				case 2:
					transforms_.push_back(
						std::make_unique<RandomRotation180>(std::get<RandomRotation180>(curr_transform))
					);
					break;
				case 3:
					transforms_.push_back(
						std::make_unique<RandomRotation270>(std::get<RandomRotation270>(curr_transform))
					);
					break;
				case 4:
					transforms_.push_back(
						std::make_unique<RandomHorizontalFlip>(std::get<RandomHorizontalFlip>(curr_transform))
					);
					break;
				case 5:
					transforms_.push_back(
						std::make_unique<RandomVerticalFlip>(std::get<RandomVerticalFlip>(curr_transform))
					);
					break;
				case 6:
					transforms_.push_back(
						std::make_unique<RandomCrop>(std::get<RandomCrop>(curr_transform))
					);
					break;
				case 7:
					transforms_.push_back(
						std::make_unique<Resize>(std::get<Resize>(curr_transform))
					);
					break;
				case 8:
					transforms_.push_back(
						std::make_unique<RandomBrightness>(std::get<RandomBrightness>(curr_transform))
					);
					break;
				case 9:
					transforms_.push_back(
						std::make_unique<RandomGaussNoise>(std::get<RandomGaussNoise>(curr_transform))
					);
					break;
				case 10:
					transforms_.push_back(
						std::make_unique<GaussianBlur>(std::get<GaussianBlur>(curr_transform))
					);
					break;
				default:
					break;
				}
			}
		}
	}
}