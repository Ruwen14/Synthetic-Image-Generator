#include "CocoISAnnotator.h"

#include "../../external/rapidjson/document.h"
#include "../../external/rapidjson/writer.h"
#include "../../external/rapidjson/prettywriter.h"

#include "../../external/rapidjson/stringbuffer.h"
#include "../../external/rapidjson/filereadstream.h"
#include <../../external/rapidjson/istreamwrapper.h>

#include <iostream>
#include <chrono>

namespace simG
{
	CocoISAnnotator::CocoISAnnotator(const std::string& annofile_savepath)
		: AbstractAnnotator(annofile_savepath)
	{
	}

	void CocoISAnnotator::annotateImg(const std::vector<cv::Mat>& masks, const AnnotationDictionary& annotationDict)
	{
		anno_file_ << "CocoIsAnnotor has written";
	}

	// About 0.0150 ms per call --> very fast
	void ADD_COCO()
	{
		// COCO STRUCTURE
		//{
		//	"info": {...},
		//		"licenses" : [...] ,
		//		"images" : [...] ,
		//		"annotations" : [...] ,
		//		"categories" : [...] , 
		//}

		std::vector testVector{ 634.97, 417.61, 632.15, 413.01, 627.55, 409.12, 622.6, 406.65, 616.95, 404.53, 611.29, 403.82, 605.28, 403.82, 599.63, 404.18, 593.62, 405.59, 587.61, 406.65, 581.25, 407.71, 575.95, 408.42, 570.65, 409.48, 564.99, 409.48, 559.34, 409.48, 553.68, 407.71, 548.73, 405.59, 543.08, 404.18, 539.54, 408.42, 537.07, 413.37, 531.42, 413.37, 525.05, 414.07, 519.4, 414.07, 513.74, 414.07, 507.73, 414.43, 501.37, 414.43, 495.72, 414.78, 489.36, 414.78, 482.64, 414.78, 477.34, 415.13, 472.04, 416.19, 466.38, 416.9, 460.73, 417.61, 454.72, 417.96, 449.42, 418.31, 444.11, 419.73, 438.81, 420.43, 433.51, 421.85, 425.74, 423.97, 421.49, 427.86, 417.25, 435.28, 416.55, 443.76, 417.61, 455.78, 417.61, 467.44, 417.96, 477.34, 420.43, 489.0, 422.91, 499.61, 425.74, 508.79, 429.62, 514.1, 437.05, 517.98, 445.18, 519.4, 465.32, 520.81, 472.39, 520.81, 478.05, 521.17, 484.41, 521.17, 491.12, 520.81, 497.13, 520.81, 503.49, 520.46, 508.79, 520.11, 514.45, 519.04, 520.11, 518.69, 526.47, 517.63, 531.77, 517.28, 537.07, 516.92, 542.73, 516.57, 555.1, 515.51, 551.56, 520.11, 549.79, 523.29, 549.79, 525.05, 555.1, 525.76, 557.92, 523.29, 559.69, 519.04, 561.46, 516.57, 572.06, 514.45, 592.21, 515.16, 604.93, 513.39, 621.54, 510.92, 628.61, 508.09, 636.74, 499.61, 639.57, 492.54, 641.34, 481.58, 641.34, 465.67, 640.63, 451.54, 638.15, 433.16 };

		auto start = std::chrono::high_resolution_clock::now();


		//auto start = std::chrono::high_resolution_clock::now();

		rapidjson::Document rootObj;
		// define the document as an object rather than an array
		rootObj.SetObject();
		rapidjson::Document::AllocatorType& allocator = rootObj.GetAllocator();

		rapidjson::Value AnnotationFieldsArray(rapidjson::kArrayType);
		rapidjson::Value Bbox(rapidjson::kArrayType);
		//rootObj.AddMember("annotations", AnnotationsArray, allocator);

		// **************************************************
		// apppend to "annotations": []
		// **************************************************
		rapidjson::Value SingleAnnotationFieldObject(rapidjson::kObjectType);
		SingleAnnotationFieldObject.AddMember("id", 15, allocator);
		SingleAnnotationFieldObject.AddMember("image_id", 3, allocator);
		SingleAnnotationFieldObject.AddMember("category_id", 1, allocator);
		SingleAnnotationFieldObject.AddMember("iscrowd", 0, allocator);
		SingleAnnotationFieldObject.AddMember("area", 22859.0, allocator);
		Bbox.PushBack(416.55, allocator).PushBack(403.82, allocator).PushBack(224.79, allocator).PushBack(121.94, allocator);
		SingleAnnotationFieldObject.AddMember("bbox", Bbox, allocator);
		rapidjson::Value SegmentationArray(rapidjson::kArrayType);
		for (const auto& el : testVector)
		{
			SegmentationArray.PushBack(el, allocator);
		}
		rapidjson::Value DummyArrayBrackets(rapidjson::kArrayType);
		DummyArrayBrackets.PushBack(SegmentationArray, allocator);
		SingleAnnotationFieldObject.AddMember("segmentation", DummyArrayBrackets, allocator);

		//rootObj["annotations"].PushBack(SingleAnnotationObject, allocator);;Evertime we add someting to list of annotations
		AnnotationFieldsArray.PushBack(SingleAnnotationFieldObject, allocator);
		rootObj.AddMember("annotations", AnnotationFieldsArray, allocator);


		// **************************************************
		// apppend to "images": []
		// **************************************************
		rapidjson::Value ImageFieldsArray(rapidjson::kArrayType);
		rapidjson::Value SingleImageFieldObject(rapidjson::kObjectType);


		SingleImageFieldObject.AddMember("id", 15, allocator);
		SingleImageFieldObject.AddMember("width", 640, allocator);
		SingleImageFieldObject.AddMember("height", 427, allocator);
		SingleImageFieldObject.AddMember("file_name", "000000037777.jpg", allocator);
		SingleImageFieldObject.AddMember("coco_url", "", allocator);
		SingleImageFieldObject.AddMember("date_captured", "", allocator);
		SingleImageFieldObject.AddMember("flickr_url", "", allocator);
		SingleImageFieldObject.AddMember("license", 0, allocator);

		ImageFieldsArray.PushBack(SingleImageFieldObject, allocator);

		rootObj.AddMember("images", ImageFieldsArray, allocator);

		// **************************************************
		// apppend to "images": []
		// **************************************************
		rapidjson::Value CategoryFieldsArray(rapidjson::kArrayType);




		//rapidjson::StringBuffer strbuf;
		//rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(strbuf);
		//rootObj.Accept(writer);

		//std::cout << strbuf.GetString() << std::endl;

		auto end = std::chrono::high_resolution_clock::now();
		//duration<double, std::milli> ms_double = end - start;
		//std::cout << ms_double.count() << "ms\n";
	}



}