#pragma once
#include "AbstractAnnotator.h"

namespace simG::annotators
{
	enum class PVOCType { OBJ_DET, SEM_SEG };

	class PVOCAnnotator : public AbstractAnnotator
	{
	public:
		PVOCAnnotator() = default;
		//needs virtual destructor
		~PVOCAnnotator() = default;

		void forward(const std::vector<cv::Mat>& masks, const AnnotationDictionary& annotationDict) override;

		void close() override;
	};

	//std::ostringstream testStringAsXml()
	//{
	//	std::ostringstream xml;

	//	// TODO: Candidate for fmt lib
	//	std::string folder = "VOC2012";
	//	std::string filename = "007_000027.jpg";
	//	int width = 486;
	//	int height = 500;
	//	int depth = 3;
	//	int xmin = 174;
	//	int ymin = 101;
	//	int xmax = 349;
	//	int ymax = 351;
	//	std::string person = "cat";

	//	//xml << "<annotation>\n";
	//	//xml << "<folder>" << folder << "</folder>\n";
	//	//xml << "<filename>" << filename << "</filename>\n";
	//	//xml << "<source>\n";
	//	//xml << "<database>Unknown</database>\n";
	//	//xml << "</source>\n";
	//	//xml << "<size>\n";
	//	//xml << "<width>" << width << "</width>\n";
	//	//xml << "<height>" << height << "</height>";
	//	//xml << "<depth>" << depth << "</depth>\n";
	//	//xml << "</size>\n";
	//	//xml << "<segmented>0 </segmented>\n";
	//	//xml << "<object>\n";
	//	//xml << "<name>" << person << "</name>\n";
	//	//xml << "<pose>Unspecified</pose>\n";
	//	//xml << "<truncated>0</truncated>\n";
	//	//xml << "<difficult>0</difficult>\n";
	//	//xml << "<bndbox>\n";
	//	//xml << "<xmin>" << xmin << "</xmin>\n";
	//	//xml << "<ymin>" << ymin << "</ymin>\n";
	//	//xml << "<xmax>" << xmax << "</xmax>\n";
	//	//xml << "<ymax>" << ymax << "</ymax>\n";
	//	//xml << "</bndbox>\n";
	//	//xml << "</object>\n";
	//	//xml << "</annotation>\n";

	//	xml << "<annotation>\n<folder>" << folder << "</folder>\n<filename>" << filename << "</filename>\n<source>\n<database>Unknown</database>\n</source>\n<size>\n<width>" << width << "</width>\n<height>"
	//		<< height << "</height>\n<depth>" << depth << "</depth>\n</size>\n<segmented>0</segmented>\n<object>\n<name>" << person << "</name>\n<pose>Unspecified</pose>\n<truncated>0</truncated>\n<difficult>0</difficult>\n<bndbox>\n<xmin>"
	//		<< xmin << "</xmin>\n<ymin>" << ymin << "</ymin>\n<xmax>" << xmax << "</xmax>\n<ymax>" << ymax << "</ymax>\n</bndbox>\n</object>\n</annotation>\n";

	//	//xml << "<?xml version=\"1.0\"?><some node><some child node/><some child node/></some node>";
	//	return xml;

	//}
}
