#pragma once
#include "Random.h"
#include "../../../external/rapidjson/document.h"

#include <array>
#include <string>
#include <vector>
#include <opencv2/core/mat.hpp>
#include <opencv2/core/types.hpp>

namespace simG::viz
{
	/**
	 * @brief Base(abstract) class for all Dataset vizualizers (like COCO, PascalVOC...).
	 *
	 * The virtual function 'showAnnotations' has to be overriden in every subclass.
	*/
	class DatasetVisualizer
	{
	public:
		DatasetVisualizer() = default;
		virtual ~DatasetVisualizer() = default;

		/**
		 * @brief Queries and draws annotations on the image belonging to the input path
		 * @param filepath path of the image for which the annotation should be shown
		 * @return image with drawn annotations
		*/
		virtual cv::Mat showAnnotations(const std::string& filepath) const = 0;

		void setLineWidth(int linewidth) { linewidth_ = linewidth; }
		void setFontFace(int fontface) { fontFace_ = fontface; }
		void setFontScale(double fontScale) { fontScale_ = fontScale; }
		void doShowBBox(bool bboxstate) { drawbbox_ = bboxstate; }
		void doShowCategory(bool catstate) { drawcat_ = catstate; }
		void setColorMap(const std::vector<cv::Scalar>& cmap) { colormap_ = cmap; }

	protected:
		void drawBBox(cv::Mat& dst, cv::Point upperleft, cv::Point bottomright, cv::Scalar color) const;
		void drawCategoryTag(cv::Mat& dst, const std::string& category, cv::Point pos, cv::Scalar color) const;
		void drawSegmentationMask(cv::Mat& dst, const std::vector<cv::Point>& cntr, cv::Scalar color) const;
		cv::Scalar getRandomColor() const;

		int linewidth_ = 2;
		int fontFace_ = 2;
		double fontScale_ = 0.65;

		bool drawbbox_ = true;
		bool drawcat_ = true;

		std::vector<cv::Scalar> colormap_;
	};

	/**
	 * @brief Vizualizer for the COCO-Dataset-Format. Supports formats: (object detection, instance segmentation and semantic segmentation). Inherits from class DatasetVizualizer
	*/
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

		/**
		 * @brief Queries and draws annotations on the image belonging to the input path
		 * @param filepath path of the image for which the annotation should be shown
		 * @return image with drawn annotations
		*/
		cv::Mat showAnnotations(const std::string& filepath) const override;
		void doShowSegMask(bool maskstate) { drawmask_ = maskstate; }

	public:
		int getImageId(const std::string fname) const;
		std::string getCategoryById(int catId) const;
		std::vector<cv::Point> translateListToContour(const std::vector<double>& src) const;

		bool drawmask_ = true;
		rapidjson::Document json_;
	};

	/**
	 * @brief Vizualizer for the PascalVOC-Dataset-Format. Supports formats: (object detection). Inherits from class DatasetVizualizer
	*/
	class PascalVOCVizualizer : public DatasetVisualizer
	{
		PascalVOCVizualizer(const std::string& annotationsDir);
	};

	/**
	 * @brief Vizualizer for the YOLO(Darknet)-Format. Supports formats: (object detection). Inherits from class DatasetVizualizer
	*/
	class YOLOVizualizer : public DatasetVisualizer
	{
		YOLOVizualizer(const std::string& annotationsDir);
	};
}
