#pragma once
#include "Random.h"
#include "../../external/rapidjson/document.h"

#include <array>
#include <string>
#include <vector>
#include <opencv2/core/mat.hpp>
#include <opencv2/core/types.hpp>

namespace simG::viz
{
	class DatasetVisualizer
	{
	public:
		DatasetVisualizer() = default;
		virtual ~DatasetVisualizer() = default;

		virtual cv::Mat showAnnotations(const std::string& filepath) const = 0;

		void setLineWidth(int linewidth) { linewidth_ = linewidth; }
		void setFontFace(int fontface) { fontFace_ = fontface; }
		void setFontScale(double fontScale) { fontScale_ = fontScale; }
		void setColorMap(const std::vector<cv::Scalar>& cmap) { colormap_ = cmap; }

	protected:
		void drawBBox(cv::Mat& dst, cv::Point upperleft, cv::Point bottomright, cv::Scalar color) const;
		void drawCategoryTag(cv::Mat& dst, const std::string& category, cv::Point pos, cv::Scalar color) const;
		void drawSegmentationMask(cv::Mat& dst, const std::vector<cv::Point>& cntr, cv::Scalar color) const;
		cv::Scalar getRandomColor() const;

		int linewidth_ = 2;
		int fontFace_ = 2;
		double fontScale_ = 0.65;
		std::vector<cv::Scalar> colormap_;
	};

	class COCOVizualizer : public DatasetVisualizer
	{
	public:
		struct Annotations
		{
			std::array<double, 4> bbox;
			std::vector<double> segmentations;
			std::string category;
		};

		COCOVizualizer(const std::string& annotationfile);

		cv::Mat showAnnotations(const std::string& filepath) const override;

	public:
		int getImageId(const std::string fname) const;
		std::string getCategoryById(int catId) const;
		std::vector<cv::Point> translateListToContour(const std::vector<double>& src) const;

		rapidjson::Document json_;
	};

	class PascalVOCVizualizer : public DatasetVisualizer
	{
		PascalVOCVizualizer(const std::string& annotationsDir);
	};

	class YOLOVizualizer : public DatasetVisualizer
	{
		YOLOVizualizer(const std::string& annotationsDir);
	};
}
