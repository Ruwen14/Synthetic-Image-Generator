#include "annotator.h"

namespace simG
{
	COCOAnnotator::COCOAnnotator(const std::string& outJsonPath)
		:out_json_path_(outJsonPath), info_field_({ "","","","","","" }), license_field_({ 1, "","" })
	{
	}

	void COCOAnnotator::set_info_field(const InfoField& info_field)
	{
		this->info_field_ = info_field;
	}

	void COCOAnnotator::set_license_field(int license_id, const std::string& license_name, const std::string& url)
	{
		this->license_field_.id = license_id;
		this->license_field_.name = license_name;
		this->license_field_.url = url;
	}

	void COCOAnnotator::add_category(int id, const std::string& category_name, const std::string& supercategory)
	{
		Category new_category = {
			id,
			category_name,
			supercategory
		};
		this->categories_.push_back(new_category);
	}

	void COCOAnnotator::add_annotation(int id, int image_id, int category_id, float area, std::array<float, 4> bbox, std::vector<float> segmentation_points)
	{
		AnnotationLabel new_annotation = {
			id,
			image_id,
			category_id,
			0,
			{
				area,
				bbox,
				segmentation_points
			}
		};
	}

	simG::COCOAnnotator::MaskAnnotation COCOAnnotator::get_mask_annotation(const cv::Mat& mask) const
	{
		return {};
	}
}