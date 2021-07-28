#pragma once
#include <string>
#include <array>
#include <vector>
#include <opencv2/core/mat.hpp>
#include <fstream>

namespace simG
{
	// Write all like an API
	class AutoAnnotator // Abstract-Base-Class for all other annotators
	{
	};

	class COCOAnnotator : public AutoAnnotator
	{
	public:
		struct MaskAnnotation {
			float area;
			std::array<float, 4> bbox;
			std::vector<float> segmentation_pts;
		};
		struct AnnotationLabel {
			int id;
			int image_id;
			int category_id;
			int iscrowd;
			MaskAnnotation mask_annotation;
		};
		struct ImageLabel {
			int image_id;
			std::string filename;
			int width;
			int height;
			std::string date_captured;
			int license;
		};
		struct Category {
			int id;
			std::string name;
			std::string supercategory;
		};
		struct InfoField {
			std::string description;
			std::string url;
			std::string version;
			std::string year;
			std::string contributor;
			std::string date_created;
		};
		struct LicenseField {
			int id;
			std::string name;
			std::string url;
		};

		COCOAnnotator(const std::string& outJsonPath);
		~COCOAnnotator() = default; // add close file // add infofield and license at the end before close

		void set_info_field(const InfoField& info_field);
		void set_license_field(int license_id, const std::string& license_name, const std::string& url);
		void add_category(int id, const std::string& category_name, const std::string& supercategory);
		void add_annotation(int id, int image_id, int category_id, float area, std::array<float, 4> bbox, std::vector<float> segmentation_points);
		MaskAnnotation get_mask_annotation(const cv::Mat& mask) const;

	private:
		void open();
		void close();

		std::string out_json_path_;
		std::vector<Category> categories_;
		InfoField info_field_;
		LicenseField license_field_;
	};
}