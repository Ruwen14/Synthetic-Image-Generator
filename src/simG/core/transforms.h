#pragma once
#include <opencv2/opencv.hpp>
#include <variant>

namespace simG
{
	namespace transforms
	{
		template<typename T>
		struct Range
		{
			T lower;
			T upper;
		};

		struct Dimensions
		{
			int width;
			int height;
		};

		/** @brief Base class of all transformations. Not intended for initialization.
		*/
		class Transformation
		{
		public:
			virtual ~Transformation() = default;

			virtual cv::Mat operator() (cv::Mat& srcSample) const = 0;

			std::string getType() const { return typeid(*this).name(); }
		};

		/** @brief Rotate the input by an angle selected randomly from the uniform distribution.
		*@param range:  rotation-angle is chosen randomly between range(int min, int max).
		*/
		class RandomRotation : public Transformation
		{
		public:
			RandomRotation(Range<int> range) : rotation_range_(range) {};

			cv::Mat operator() (cv::Mat& srcSample) const override;

		private:
			Range<int> rotation_range_;
		};

		/** @brief Randomly rotate the input by 90 degrees.
		*@param chance (double):  probability of applying the transform.
		*/
		class RandomRotation90 : public Transformation
		{
		public:
			RandomRotation90(double chance) : chance_(chance) {};

			cv::Mat operator() (cv::Mat& srcSample) const override;

		private:
			double chance_;
		};

		/** @brief Randomly rotate the input by 180 degrees.
		*@param chance (double):  probability of applying the transform.
		*/
		class RandomRotation180 : public Transformation
		{
		public:
			RandomRotation180(double chance) : chance_(chance) {};

			cv::Mat operator() (cv::Mat& srcSample) const override;

		private:
			double chance_;
		};

		/** @brief Randomly rotate the input by 270 degrees.
		*@param chance (double):  probability of applying the transform.
		*/
		class RandomRotation270 : public Transformation
		{
		public:
			RandomRotation270(double chance) : chance_(chance) {};

			cv::Mat operator() (cv::Mat& srcSample) const override;

		private:
			double chance_;
		};

		/** @brief Randomly flip the input around y-axis.
		*@param chance (double):  probability of applying the transform.
		*/
		class RandomHorizontalFlip : public Transformation
		{
		public:
			RandomHorizontalFlip(double chance) : chance_(chance) {};
			//virtual ~RandomHorizontalFlip() = default;

			cv::Mat operator() (cv::Mat& srcSample) const override;

		private:
			double chance_;
		};

		/** @brief Randomly flip the input around x-axis.
		*@param chance (double):  probability of applying the transform.
		*/
		class RandomVerticalFlip : public Transformation
		{
		public:
			RandomVerticalFlip(double chance) : chance_(chance) {};
			//virtual ~RandomVerticalFlip() = default;

			cv::Mat operator() (cv::Mat& srcSample) const override;

		private:
			double chance_;
		};

		/** @brief Crop the given image at a random location.
		*@param targetDim (Dimensions): dimensions of the crop.
		*@param keepAspectRatio (bool): whether to keep the aspect ratio when cropping.
		*/
		class RandomCrop : public Transformation
		{
		public:
			RandomCrop(Dimensions targetDim, bool keepAspectRatio) :
				crop_dim_(targetDim), keep_aspect_(keepAspectRatio) {};
			//virtual ~RandomCrop() = default;

			cv::Mat operator() (cv::Mat& srcSample) const override;

		private:
			Dimensions calcPreservedAspectRatio(Dimensions srcDim, Dimensions targetDim) const;

			Dimensions crop_dim_;
			bool keep_aspect_;
		};

		/** @brief Resize the input to the given height and width.
		*@param dim (Dimensions): desired dimensions.
		*@param keepAspectRatio (bool): whether to keep the aspect ratio when resizing.
		*/
		class Resize : public Transformation
		{
		public:
			Resize(Dimensions dim, bool keepAspectRatio) :
				dim_(dim), keep_aspect_(keepAspectRatio) {};
			//virtual ~Resize() = default;

			cv::Mat operator() (cv::Mat& srcSample) const override;

		private:
			Dimensions calcPreservedAspectRatio(Dimensions srcDim, Dimensions targetDim) const;

			Dimensions dim_;
			bool keep_aspect_;
		};

		class RandomTranslation : public Transformation
		{
		};

		/** @brief Randomly resize the input.
		*@param scaleLimits: min and max values between which a value is randomly selected.
		*/
		class RandomScale : public Transformation
		{
		public:
			RandomScale(Range<double> scaleLimits)
				: scale_limits_(scaleLimits) {};
			//virtual ~RandomScale() = default;

			cv::Mat operator() (cv::Mat& srcSample) const override;

		private:
			Range<double> scale_limits_;
		};

		/** @brief Randomly change brightness of the input image.
		*@param betaRange:  brightness factor is chosen randomly between range(double min, double max).
		*@param alpha: alpha factor. Default = 1.
		*/
		class RandomBrightness : public Transformation
		{
		public:
			RandomBrightness(Range<double> betaRange, double alpha = 1)
				: beta_r_(betaRange), alpha_(alpha) {}
			//virtual ~RandomBrightness() = default;

			cv::Mat operator() (cv::Mat& srcSample) const override;

		public:
			Range<double> beta_r_;
			double alpha_;
		};

		/** @brief Randomly apply gaussian noise to the input image.
		*@param chance (double):  probability of applying the transform.
		*@param mean (double): mean value.
		*@param stdDev (double): standard deviation value.
		*/
		class RandomGaussNoise : public Transformation
		{
		public:
			RandomGaussNoise(double chance, double mean = 0.0, double stdDev = 10.0)
				: chance_(chance), mean_(mean), standard_dev_(stdDev) {}
			//virtual ~RandomGaussNoise() = default;

			cv::Mat operator() (cv::Mat& srcSample) const override;

		public:
			double chance_;
			double mean_;
			double standard_dev_;
		};

		/** @brief Randomly apply gaussian blur to the input image.
		*@param chance (double):  probability of applying the transform.
		*@param kSize (cv::Size): kernel size.
		*/
		class GaussianBlur : public Transformation
		{
		public:
			GaussianBlur(double chance, cv::Size kSize = { 3,3 })
				: chance_(chance), kernelsize_(kSize) {}
			//virtual ~GaussianBlur() = default;

			cv::Mat operator() (cv::Mat& srcSample) const override;

		public:
			double chance_;
			cv::Size kernelsize_;
		};

		/** @brief A sequential container that holds transforms (see simG::transforms in transforms.h).
		 * Transforms are added in the order in which they are passed in the constructor.
		 * The ``apply()`` method is used to apply selected transforms to an input sample.
		 * @param transformsList: vector of transformers  listed in transforms.h (accessible via simG::transforms::).
		 */
		class Sequential
		{
		public:
			using Transformers = std::variant<
				RandomRotation, RandomRotation90, RandomRotation180, RandomRotation270, RandomHorizontalFlip,
				RandomVerticalFlip, RandomCrop, Resize, RandomScale, RandomBrightness, RandomGaussNoise, GaussianBlur>;

			using TfContainer = std::vector<std::shared_ptr<Transformation>>;

			Sequential() = default;
			Sequential(const std::vector<Transformers>& transformsList);
			//Sequential(const Sequential&) = delete;
			//Sequential(const Sequential&) {std::cout << "Copied Sequential\n"; }
			~Sequential() = default;
			/** @brief Applies selected transformations to an input sample.
			@param src: input matrix. @param dst: destination matrix that reflects transformations.
			*/
			void apply(const cv::Mat& src, cv::Mat& dst) const;

			/** @brief Applies selected transformations to an input sample.
			@param src: input matrix. @param dst: destination matrix that reflects transformations.
			*/
			void operator()(const cv::Mat& src, cv::Mat& dst) const;

			/** @brief Returns the number of transformers that the bucket currently contains.
			*/
			int count() const;

			/** @brief Returns a printable string representation of the bucket and its contents.
			*/
			std::string dump() const;

		public:
			void populate(const std::vector<Transformers>& transforms_list);

			TfContainer transforms_;
		};
	}
}
