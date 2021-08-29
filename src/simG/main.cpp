// Synthethic-Image-Generator.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "../simG.h"
#include "utils/Timer.h"
#include "core/algorithms.h"
#include "core/types.h"

#include "../../external/rapidjson/document.h"
#include "../../external/rapidjson/writer.h"
#include "../../external/rapidjson/prettywriter.h"

#include "../../external/rapidjson/stringbuffer.h"
#include "../../external/rapidjson/filereadstream.h"
#include <../../external/rapidjson/istreamwrapper.h>
#include "utils/datasetviz.h"

#include "../../external/clipper/clipper.hpp"

#include <iostream>
#include <chrono>
#include <variant>
#include <cstdio>
#include <optional>
#include <functional>
#include <thread>

#include <ctime>

using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;

using simG::print;
//#ifndef SIMG_EXPORT
//# if (defined _WIN32 || defined WINCE || defined __CYGWIN__)
//#   define SIMG_EXPORT __declspec(dllexport)
//# elif defined __GNUC__ && __GNUC__ >= 4 && (defined(CVAPI_EXPORTS) || defined(__APPLE__))
//#   define SIMG_EXPORT __attribute__ ((visibility ("default")))
//# endif
//#endif

// From https://github.com/TheCherno/Hazel/blob/master/Hazel/src/Hazel/Core/PlatformDetection.h
// Platform detection using predefined macros
//#ifdef _WIN32
//	/* Windows x64/x86 */
//#ifdef _WIN64
//	/* Windows x64  */
//#define HZ_PLATFORM_WINDOWS
//#else
//	/* Windows x86 */
//#error "x86 Builds are not supported!"
//#endif
//#elif defined(__APPLE__) || defined(__MACH__)
//#include <TargetConditionals.h>
///* TARGET_OS_MAC exists on all the platforms
// * so we must check all of them (in this order)
// * to ensure that we're running on MAC
// * and not some other Apple platform */
//#if TARGET_IPHONE_SIMULATOR == 1
//#error "IOS simulator is not supported!"
//#elif TARGET_OS_IPHONE == 1
//#define HZ_PLATFORM_IOS
//#error "IOS is not supported!"
//#elif TARGET_OS_MAC == 1
//#define HZ_PLATFORM_MACOS
//#error "MacOS is not supported!"
//#else
//#error "Unknown Apple platform!"
//#endif
// /* We also have to check __ANDROID__ before __linux__
//  * since android is based on the linux kernel
//  * it has __linux__ defined */
//#elif defined(__ANDROID__)
//#define HZ_PLATFORM_ANDROID
//#error "Android is not supported!"
//#elif defined(__linux__)
//#define HZ_PLATFORM_LINUX
//#error "Linux is not supported!"
//#else
//	/* Unknown compiler/platform */
//#error "Unknown platform!"
//#endif // End of platform detection

namespace sa = simG::annotators;
#include "core/coreutils.h"

cv::Mat getBinaryImg(const std::string& path)
{
	auto img = cv::imread(path, -1);
	cv::Mat imgray;
	cv::cvtColor(img, imgray, cv::COLOR_BGR2GRAY);
	cv::Mat thresh;
	cv::threshold(imgray, thresh, 0, 255, 0);

	return thresh;
}

void testcvintersection()
{
	cv::Mat blankImg = simG::algorithms::generateZeroMatrix(298, 450);
	cv::Mat blankImg2 = simG::algorithms::generateZeroMatrix(298, 450);

	std::vector<cv::Point> cntrs1 = { {144, 30},{143, 31},{138, 31},{137, 32},{136, 31},{134, 33},{133, 33},{132, 34},{131, 34},{130, 35},{129, 35},{127, 37},{127, 38},{126, 39},{126, 40},{125, 41},{124, 41},{124, 42},{122, 44},{122, 46},{120, 48},{120, 49},{118, 51},{118, 52},{117, 53},{116, 53},{116, 55},{114, 57},{114, 58},{112, 60},{112, 61},{111, 62},{111, 63},{110, 64},{110, 65},{108, 67},{109, 68},{109, 69},{108, 70},{108, 71},{107, 72},{107, 76},{105, 78},{106, 79},{105, 80},{105, 87},{104, 88},{105, 89},{104, 90},{104, 93},{103, 94},{104, 95},{104, 96},{103, 97},{103, 103},{102, 104},{102, 110},{101, 111},{101, 116},{100, 117},{101, 118},{101, 119},{100, 120},{101, 121},{101, 123},{100, 124},{100, 133},{99, 134},{99, 145},{100, 146},{99, 147},{99, 151},{100, 152},{100, 153},{99, 154},{100, 155},{100, 156},{99, 157},{100, 158},{99, 159},{99, 161},{98, 162},{97, 162},{96, 161},{94, 161},{93, 160},{93, 161},{92, 162},{91, 162},{91, 164},{90, 165},{90, 166},{91, 167},{91, 169},{95, 173},{96, 173},{98, 175},{100, 175},{102, 177},{103, 177},{104, 178},{103, 179},{104, 180},{104, 182},{105, 183},{105, 188},{106, 189},{106, 192},{107, 193},{107, 200},{108, 201},{108, 205},{109, 206},{109, 210},{110, 211},{110, 214},{111, 215},{111, 218},{112, 219},{111, 220},{112, 221},{112, 222},{113, 223},{113, 224},{114, 225},{113, 226},{116, 229},{116, 230},{118, 232},{117, 233},{118, 233},{123, 238},{123, 239},{125, 241},{126, 241},{128, 243},{129, 243},{130, 244},{130, 245},{132, 245},{134, 247},{135, 247},{136, 248},{137, 248},{138, 249},{140, 249},{141, 250},{144, 250},{145, 251},{149, 251},{150, 252},{155, 252},{156, 253},{187, 253},{188, 252},{189, 252},{190, 253},{193, 253},{194, 252},{199, 252},{200, 251},{204, 251},{205, 250},{206, 250},{207, 249},{208, 249},{208, 248},{209, 247},{210, 247},{214, 243},{215, 243},{215, 242},{220, 237},{220, 236},{223, 233},{223, 229},{224, 228},{224, 227},{225, 226},{225, 225},{228, 222},{228, 220},{231, 217},{231, 215},{232, 214},{232, 213},{233, 212},{233, 203},{232, 202},{232, 201},{231, 200},{231, 198},{230, 197},{230, 195},{229, 194},{229, 192},{227, 190},{227, 186},{226, 185},{226, 154},{227, 153},{227, 137},{226, 136},{227, 135},{227, 134},{226, 133},{227, 132},{226, 131},{227, 130},{226, 129},{226, 109},{227, 108},{227, 104},{228, 103},{228, 102},{227, 101},{227, 100},{228, 99},{228, 92},{233, 87},{233, 86},{234, 85},{234, 84},{235, 83},{235, 82},{236, 81},{236, 80},{237, 79},{237, 63},{236, 62},{236, 60},{235, 59},{235, 57},{233, 55},{233, 54},{231, 52},{231, 51},{229, 51},{227, 49},{227, 46},{225, 44},{225, 43},{224, 42},{225, 41},{224, 41},{222, 39},{221, 39},{220, 38},{219, 38},{217, 36},{216, 36},{215, 35},{211, 35},{210, 34},{209, 34},{208, 33},{203, 33},{202, 32},{202, 31},{201, 32},{199, 32},{198, 31},{189, 31},{188, 30} };

	std::vector<cv::Point> cntrs2 = { {47, 0},{46, 1},{40, 1},{39, 2},{38, 2},{37, 3},{33, 3},{32, 4},{31, 4},{30, 5},{28, 5},{26, 7},{23, 7},{21, 9},{18, 9},{16, 11},{15, 11},{14, 12},{13, 12},{13, 13},{12, 14},{12, 18},{11, 19},{10, 19},{7, 22},{7, 23},{5, 25},{5, 28},{4, 29},{4, 32},{3, 33},{3, 37},{4, 38},{4, 44},{5, 45},{5, 49},{7, 51},{7, 52},{11, 56},{11, 58},{12, 59},{11, 60},{11, 77},{10, 78},{11, 79},{11, 81},{10, 82},{10, 101},{9, 102},{10, 103},{10, 104},{9, 105},{9, 129},{8, 130},{8, 145},{7, 146},{7, 148},{8, 149},{8, 152},{7, 153},{7, 160},{3, 164},{3, 165},{2, 166},{2, 167},{1, 168},{1, 178},{0, 179},{0, 180},{1, 181},{0, 182},{0, 186},{1, 187},{1, 190},{3, 192},{3, 194},{6, 197},{7, 197},{8, 198},{8, 202},{9, 203},{9, 204},{11, 206},{11, 207},{12, 207},{13, 208},{13, 209},{21, 217},{22, 216},{23, 217},{23, 218},{24, 217},{25, 218},{25, 219},{27, 219},{28, 220},{29, 219},{30, 219},{32, 221},{33, 221},{34, 222},{40, 222},{41, 221},{42, 221},{44, 223},{49, 223},{50, 222},{55, 222},{56, 223},{59, 223},{60, 222},{61, 223},{61, 224},{62, 223},{63, 224},{76, 224},{78, 222},{79, 223},{81, 223},{82, 222},{83, 222},{84, 221},{89, 221},{90, 220},{91, 220},{92, 219},{93, 219},{94, 218},{95, 218},{96, 217},{97, 217},{103, 211},{104, 211},{114, 201},{114, 200},{115, 199},{115, 198},{116, 197},{117, 197},{117, 196},{118, 195},{118, 192},{119, 191},{119, 188},{120, 187},{120, 177},{121, 176},{121, 172},{122, 171},{122, 160},{123, 159},{123, 154},{124, 153},{124, 152},{125, 151},{130, 151},{131, 150},{132, 150},{133, 149},{134, 149},{137, 146},{138, 146},{138, 145},{139, 144},{140, 144},{140, 143},{141, 142},{141, 140},{138, 137},{134, 137},{133, 138},{130, 138},{129, 139},{127, 139},{126, 140},{124, 138},{124, 120},{125, 119},{125, 63},{126, 62},{126, 49},{125, 48},{125, 45},{124, 44},{124, 35},{123, 34},{123, 32},{122, 31},{122, 27},{121, 26},{121, 24},{120, 23},{120, 19},{119, 18},{119, 17},{118, 16},{118, 15},{116, 13},{116, 12},{113, 9},{112, 9},{110, 7},{109, 7},{107, 5},{104, 5},{102, 3},{98, 3},{97, 2},{96, 2},{95, 1},{84, 1},{83, 0} };

	simG::Timer::ScopeTimer sc;

	std::vector<std::vector<cv::Point>> temp_cntr1;
	temp_cntr1.push_back(cntrs1);
	cv::drawContours(blankImg, temp_cntr1, -1, cv::Scalar(255, 0, 255), cv::FILLED);
	std::vector<std::vector<cv::Point>> temp_cntr2;
	temp_cntr2.push_back(cntrs2);
	cv::drawContours(blankImg2, temp_cntr2, -1, cv::Scalar(255, 0, 0), cv::FILLED);

	//cv::imshow("", blankImg);
	//cv::imshow("ee", blankImg2);

	cv::Mat dst;
	cv::bitwise_and(blankImg, blankImg2, dst);
	auto area = cv::countNonZero(dst);
	//print(area);
	//cv::waitKey(0);
}

void smplfyContoursTest()
{
	//std::vector<cv::Point> cntrs1 = { {144, 30},{143, 31},{138, 31},{137, 32},{136, 31},{134, 33},{133, 33},{132, 34},{131, 34},{130, 35},{129, 35},{127, 37},{127, 38},{126, 39},{126, 40},{125, 41},{124, 41},{124, 42},{122, 44},{122, 46},{120, 48},{120, 49},{118, 51},{118, 52},{117, 53},{116, 53},{116, 55},{114, 57},{114, 58},{112, 60},{112, 61},{111, 62},{111, 63},{110, 64},{110, 65},{108, 67},{109, 68},{109, 69},{108, 70},{108, 71},{107, 72},{107, 76},{105, 78},{106, 79},{105, 80},{105, 87},{104, 88},{105, 89},{104, 90},{104, 93},{103, 94},{104, 95},{104, 96},{103, 97},{103, 103},{102, 104},{102, 110},{101, 111},{101, 116},{100, 117},{101, 118},{101, 119},{100, 120},{101, 121},{101, 123},{100, 124},{100, 133},{99, 134},{99, 145},{100, 146},{99, 147},{99, 151},{100, 152},{100, 153},{99, 154},{100, 155},{100, 156},{99, 157},{100, 158},{99, 159},{99, 161},{98, 162},{97, 162},{96, 161},{94, 161},{93, 160},{93, 161},{92, 162},{91, 162},{91, 164},{90, 165},{90, 166},{91, 167},{91, 169},{95, 173},{96, 173},{98, 175},{100, 175},{102, 177},{103, 177},{104, 178},{103, 179},{104, 180},{104, 182},{105, 183},{105, 188},{106, 189},{106, 192},{107, 193},{107, 200},{108, 201},{108, 205},{109, 206},{109, 210},{110, 211},{110, 214},{111, 215},{111, 218},{112, 219},{111, 220},{112, 221},{112, 222},{113, 223},{113, 224},{114, 225},{113, 226},{116, 229},{116, 230},{118, 232},{117, 233},{118, 233},{123, 238},{123, 239},{125, 241},{126, 241},{128, 243},{129, 243},{130, 244},{130, 245},{132, 245},{134, 247},{135, 247},{136, 248},{137, 248},{138, 249},{140, 249},{141, 250},{144, 250},{145, 251},{149, 251},{150, 252},{155, 252},{156, 253},{187, 253},{188, 252},{189, 252},{190, 253},{193, 253},{194, 252},{199, 252},{200, 251},{204, 251},{205, 250},{206, 250},{207, 249},{208, 249},{208, 248},{209, 247},{210, 247},{214, 243},{215, 243},{215, 242},{220, 237},{220, 236},{223, 233},{223, 229},{224, 228},{224, 227},{225, 226},{225, 225},{228, 222},{228, 220},{231, 217},{231, 215},{232, 214},{232, 213},{233, 212},{233, 203},{232, 202},{232, 201},{231, 200},{231, 198},{230, 197},{230, 195},{229, 194},{229, 192},{227, 190},{227, 186},{226, 185},{226, 154},{227, 153},{227, 137},{226, 136},{227, 135},{227, 134},{226, 133},{227, 132},{226, 131},{227, 130},{226, 129},{226, 109},{227, 108},{227, 104},{228, 103},{228, 102},{227, 101},{227, 100},{228, 99},{228, 92},{233, 87},{233, 86},{234, 85},{234, 84},{235, 83},{235, 82},{236, 81},{236, 80},{237, 79},{237, 63},{236, 62},{236, 60},{235, 59},{235, 57},{233, 55},{233, 54},{231, 52},{231, 51},{229, 51},{227, 49},{227, 46},{225, 44},{225, 43},{224, 42},{225, 41},{224, 41},{222, 39},{221, 39},{220, 38},{219, 38},{217, 36},{216, 36},{215, 35},{211, 35},{210, 34},{209, 34},{208, 33},{203, 33},{202, 32},{202, 31},{201, 32},{199, 32},{198, 31},{189, 31},{188, 30} };
	std::vector<cv::Point> cntrs1 = { {144, 30},{143, 31},{138, 31},{137, 32},{136, 31},{134, 33},{133, 33},{132, 34},{131, 34},{130, 35},{129, 35},{127, 37},{127, 38},{126, 39},{126, 40},{125, 41},{124, 41},{124, 42},{122, 44},{122, 46},{120, 48},{120, 49},{118, 51},{118, 52},{117, 53},{116, 53},{116, 55},{114, 57},{114, 58},{112, 60},{112, 61},{111, 62},{111, 63},{110, 64},{110, 65},{108, 67},{109, 68},{109, 69},{108, 70},{108, 71},{107, 72},{107, 76},{105, 78},{106, 79},{105, 80},{105, 87},{104, 88},{105, 89},{104, 90},{104, 93},{103, 94},{104, 95},{104, 96},{103, 97},{103, 103},{102, 104},{102, 110},{101, 111},{101, 116},{100, 117},{101, 118},{101, 119},{100, 120},{101, 121},{101, 123},{100, 124},{100, 133},{99, 134},{99, 145},{100, 146},{99, 147},{99, 151} };

	//std::vector<cv::Point> cntrs1 = { {47, 0},{46, 1},{40, 1},{39, 2},{38, 2},{37, 3},{33, 3},{32, 4},{31, 4},{30, 5},{28, 5},{26, 7},{23, 7},{21, 9},{18, 9},{16, 11},{15, 11},{14, 12},{13, 12},{13, 13},{12, 14},{12, 18},{11, 19},{10, 19},{7, 22},{7, 23},{5, 25},{5, 28},{4, 29},{4, 32},{3, 33},{3, 37},{4, 38},{4, 44},{5, 45},{5, 49},{7, 51},{7, 52},{11, 56},{11, 58},{12, 59},{11, 60},{11, 77},{10, 78},{11, 79},{11, 81},{10, 82},{10, 101},{9, 102},{10, 103},{10, 104},{9, 105},{9, 129},{8, 130},{8, 145},{7, 146},{7, 148},{8, 149},{8, 152},{7, 153},{7, 160},{3, 164},{3, 165},{2, 166},{2, 167},{1, 168},{1, 178},{0, 179},{0, 180},{1, 181},{0, 182},{0, 186},{1, 187},{1, 190},{3, 192},{3, 194},{6, 197},{7, 197},{8, 198},{8, 202},{9, 203},{9, 204},{11, 206},{11, 207},{12, 207},{13, 208},{13, 209},{21, 217},{22, 216},{23, 217},{23, 218},{24, 217},{25, 218},{25, 219},{27, 219},{28, 220},{29, 219},{30, 219},{32, 221},{33, 221},{34, 222},{40, 222},{41, 221},{42, 221},{44, 223},{49, 223},{50, 222},{55, 222},{56, 223},{59, 223},{60, 222},{61, 223},{61, 224},{62, 223},{63, 224},{76, 224},{78, 222},{79, 223},{81, 223},{82, 222},{83, 222},{84, 221},{89, 221},{90, 220},{91, 220},{92, 219},{93, 219},{94, 218},{95, 218},{96, 217},{97, 217},{103, 211},{104, 211},{114, 201},{114, 200},{115, 199},{115, 198},{116, 197},{117, 197},{117, 196},{118, 195},{118, 192},{119, 191},{119, 188},{120, 187},{120, 177},{121, 176},{121, 172},{122, 171},{122, 160},{123, 159},{123, 154},{124, 153},{124, 152},{125, 151},{130, 151},{131, 150},{132, 150},{133, 149},{134, 149},{137, 146},{138, 146},{138, 145},{139, 144},{140, 144},{140, 143},{141, 142},{141, 140},{138, 137},{134, 137},{133, 138},{130, 138},{129, 139},{127, 139},{126, 140},{124, 138},{124, 120},{125, 119},{125, 63},{126, 62},{126, 49},{125, 48},{125, 45},{124, 44},{124, 35},{123, 34},{123, 32},{122, 31},{122, 27},{121, 26},{121, 24},{120, 23},{120, 19},{119, 18},{119, 17},{118, 16},{118, 15},{116, 13},{116, 12},{113, 9},{112, 9},{110, 7},{109, 7},{107, 5},{104, 5},{102, 3},{98, 3},{97, 2},{96, 2},{95, 1},{84, 1},{83, 0} };

	std::vector<cv::Point> approxpoly;
	print(cntrs1.size());

	auto start = std::chrono::high_resolution_clock::now();
	simG::algorithms::simplifyContour(cntrs1, approxpoly, 0.0012);
	auto end = std::chrono::high_resolution_clock::now();
	duration<double, std::milli> ms_double = end - start;
	std::cout << ms_double.count() << "ms\n";
	print(approxpoly.size());
}

void drawIntersection(cv::Mat& img, const ClipperLib::Path& intersection)
{
	std::vector<std::vector<cv::Point>> temp;
	temp.push_back(simG::coreutils::clipperPolyToCvContour(intersection));
	cv::drawContours(img, temp, -1, cv::Scalar(0, 255, 0), cv::FILLED);
}

void drawClipperPoly(cv::Mat& img, ClipperLib::Path& poly)
{
	std::vector<std::vector<cv::Point>> temp;
	temp.push_back(simG::coreutils::clipperPolyToCvContour(poly));
	cv::drawContours(img, temp, -1, cv::Scalar(255, 255, 0), 1);
}

void scaleContours(std::vector<cv::Point>& cntrs, float scale)
{
	auto M = cv::moments(cntrs);
	auto cx = static_cast<int>(M.m10 / M.m00);
	auto cy = static_cast<int>(M.m01 / M.m00);

	for (auto& point : cntrs)
	{
		cv::Point centerP(cx, cy);
		point -= centerP;
		point.x *= scale;
		point.y *= scale;
		point += centerP;
	}

	//std::vector<cv::Point> cnt_norm;
	//for (const auto& point : cntrs)
	//{
	//	cnt_norm.push_back(point - cv::Point(cx, cy));
	//}

	//for (auto& point : cnt_norm)
	//{
	//	point.x *= scale;
	//	point.y *= scale;
	//}

	//for (auto& point : cnt_norm)
	//{
	//	point += cv::Point(cx, cy);
	//}
	//return cnt_norm;
}

void clipperTest()
{
	std::vector<cv::Point> cntrs1 = { {47, 0},{46, 1},{40, 1},{39, 2},{38, 2},{37, 3},{33, 3},{32, 4},{31, 4},{30, 5},{28, 5},{26, 7},{23, 7},{21, 9},{18, 9},{16, 11},{15, 11},{14, 12},{13, 12},{13, 13},{12, 14},{12, 18},{11, 19},{10, 19},{7, 22},{7, 23},{5, 25},{5, 28},{4, 29},{4, 32},{3, 33},{3, 37},{4, 38},{4, 44},{5, 45},{5, 49},{7, 51},{7, 52},{11, 56},{11, 58},{12, 59},{11, 60},{11, 77},{10, 78},{11, 79},{11, 81},{10, 82},{10, 101},{9, 102},{10, 103},{10, 104},{9, 105},{9, 129},{8, 130},{8, 145},{7, 146},{7, 148},{8, 149},{8, 152},{7, 153},{7, 160},{3, 164},{3, 165},{2, 166},{2, 167},{1, 168},{1, 178},{0, 179},{0, 180},{1, 181},{0, 182},{0, 186},{1, 187},{1, 190},{3, 192},{3, 194},{6, 197},{7, 197},{8, 198},{8, 202},{9, 203},{9, 204},{11, 206},{11, 207},{12, 207},{13, 208},{13, 209},{21, 217},{22, 216},{23, 217},{23, 218},{24, 217},{25, 218},{25, 219},{27, 219},{28, 220},{29, 219},{30, 219},{32, 221},{33, 221},{34, 222},{40, 222},{41, 221},{42, 221},{44, 223},{49, 223},{50, 222},{55, 222},{56, 223},{59, 223},{60, 222},{61, 223},{61, 224},{62, 223},{63, 224},{76, 224},{78, 222},{79, 223},{81, 223},{82, 222},{83, 222},{84, 221},{89, 221},{90, 220},{91, 220},{92, 219},{93, 219},{94, 218},{95, 218},{96, 217},{97, 217},{103, 211},{104, 211},{114, 201},{114, 200},{115, 199},{115, 198},{116, 197},{117, 197},{117, 196},{118, 195},{118, 192},{119, 191},{119, 188},{120, 187},{120, 177},{121, 176},{121, 172},{122, 171},{122, 160},{123, 159},{123, 154},{124, 153},{124, 152},{125, 151},{130, 151},{131, 150},{132, 150},{133, 149},{134, 149},{137, 146},{138, 146},{138, 145},{139, 144},{140, 144},{140, 143},{141, 142},{141, 140},{138, 137},{134, 137},{133, 138},{130, 138},{129, 139},{127, 139},{126, 140},{124, 138},{124, 120},{125, 119},{125, 63},{126, 62},{126, 49},{125, 48},{125, 45},{124, 44},{124, 35},{123, 34},{123, 32},{122, 31},{122, 27},{121, 26},{121, 24},{120, 23},{120, 19},{119, 18},{119, 17},{118, 16},{118, 15},{116, 13},{116, 12},{113, 9},{112, 9},{110, 7},{109, 7},{107, 5},{104, 5},{102, 3},{98, 3},{97, 2},{96, 2},{95, 1},{84, 1},{83, 0} };
	auto flatten = simG::coreutils::flattenContours(cntrs1);
	print(flatten.size());
	print(flatten);
	std::vector<cv::Point> cntrs2 = { {144, 30},{143, 31},{138, 31},{137, 32},{136, 31},{134, 33},{133, 33},{132, 34},{131, 34},{130, 35},{129, 35},{127, 37},{127, 38},{126, 39},{126, 40},{125, 41},{124, 41},{124, 42},{122, 44},{122, 46},{120, 48},{120, 49},{118, 51},{118, 52},{117, 53},{116, 53},{116, 55},{114, 57},{114, 58},{112, 60},{112, 61},{111, 62},{111, 63},{110, 64},{110, 65},{108, 67},{109, 68},{109, 69},{108, 70},{108, 71},{107, 72},{107, 76},{105, 78},{106, 79},{105, 80},{105, 87},{104, 88},{105, 89},{104, 90},{104, 93},{103, 94},{104, 95},{104, 96},{103, 97},{103, 103},{102, 104},{102, 110},{101, 111},{101, 116},{100, 117},{101, 118},{101, 119},{100, 120},{101, 121},{101, 123},{100, 124},{100, 133},{99, 134},{99, 145},{100, 146},{99, 147},{99, 151},{100, 152},{100, 153},{99, 154},{100, 155},{100, 156},{99, 157},{100, 158},{99, 159},{99, 161},{98, 162},{97, 162},{96, 161},{94, 161},{93, 160},{93, 161},{92, 162},{91, 162},{91, 164},{90, 165},{90, 166},{91, 167},{91, 169},{95, 173},{96, 173},{98, 175},{100, 175},{102, 177},{103, 177},{104, 178},{103, 179},{104, 180},{104, 182},{105, 183},{105, 188},{106, 189},{106, 192},{107, 193},{107, 200},{108, 201},{108, 205},{109, 206},{109, 210},{110, 211},{110, 214},{111, 215},{111, 218},{112, 219},{111, 220},{112, 221},{112, 222},{113, 223},{113, 224},{114, 225},{113, 226},{116, 229},{116, 230},{118, 232},{117, 233},{118, 233},{123, 238},{123, 239},{125, 241},{126, 241},{128, 243},{129, 243},{130, 244},{130, 245},{132, 245},{134, 247},{135, 247},{136, 248},{137, 248},{138, 249},{140, 249},{141, 250},{144, 250},{145, 251},{149, 251},{150, 252},{155, 252},{156, 253},{187, 253},{188, 252},{189, 252},{190, 253},{193, 253},{194, 252},{199, 252},{200, 251},{204, 251},{205, 250},{206, 250},{207, 249},{208, 249},{208, 248},{209, 247},{210, 247},{214, 243},{215, 243},{215, 242},{220, 237},{220, 236},{223, 233},{223, 229},{224, 228},{224, 227},{225, 226},{225, 225},{228, 222},{228, 220},{231, 217},{231, 215},{232, 214},{232, 213},{233, 212},{233, 203},{232, 202},{232, 201},{231, 200},{231, 198},{230, 197},{230, 195},{229, 194},{229, 192},{227, 190},{227, 186},{226, 185},{226, 154},{227, 153},{227, 137},{226, 136},{227, 135},{227, 134},{226, 133},{227, 132},{226, 131},{227, 130},{226, 129},{226, 109},{227, 108},{227, 104},{228, 103},{228, 102},{227, 101},{227, 100},{228, 99},{228, 92},{233, 87},{233, 86},{234, 85},{234, 84},{235, 83},{235, 82},{236, 81},{236, 80},{237, 79},{237, 63},{236, 62},{236, 60},{235, 59},{235, 57},{233, 55},{233, 54},{231, 52},{231, 51},{229, 51},{227, 49},{227, 46},{225, 44},{225, 43},{224, 42},{225, 41},{224, 41},{222, 39},{221, 39},{220, 38},{219, 38},{217, 36},{216, 36},{215, 35},{211, 35},{210, 34},{209, 34},{208, 33},{203, 33},{202, 32},{202, 31},{201, 32},{199, 32},{198, 31},{189, 31},{188, 30} };
	scaleContours(cntrs2, 1.25);
	simG::algorithms::translateContour(cntrs1, 5, 50);
	simG::algorithms::translateContour(cntrs2, -92, 30);
	// will be called per object once at beginning
	simG::algorithms::simplifyContour(cntrs1, cntrs1, 0.0012);
	simG::algorithms::simplifyContour(cntrs2, cntrs2, 0.0012);

	auto start = std::chrono::high_resolution_clock::now();

	auto overlap = simG::algorithms::fastContourIntersect(cntrs1, cntrs2);
	auto poly1 = simG::coreutils::cvContourToClipperPoly(cntrs1);
	auto poly2 = simG::coreutils::cvContourToClipperPoly(cntrs2);

	auto area = simG::coreutils::polygonIntersectArea(poly1, poly2);
	auto intersection = simG::coreutils::getPolygonIntersection(poly1, poly2);
	auto clipped = simG::coreutils::clipPolygon(poly1, intersection);

	auto percentage = abs(area) / abs(ClipperLib::Area(poly1));
	auto end = std::chrono::high_resolution_clock::now();
	duration<double, std::milli> ms_double = end - start;
	std::cout << ms_double.count() << "ms\n";
	std::cout << "Overlap:" << overlap << "\n";
	//print(area);
	std::cout << "overlap[percentage]: " << percentage * 100 << "%\n";

	cv::Mat blankImg = simG::algorithms::generateZeroMatrix(298, 450);
	cv::cvtColor(blankImg, blankImg, cv::COLOR_GRAY2BGR);

	std::vector<std::vector<cv::Point>> temp_cntr1;
	temp_cntr1.push_back(cntrs1);
	cv::drawContours(blankImg, temp_cntr1, -1, cv::Scalar(255, 0, 0), 1);
	std::vector<std::vector<cv::Point>> temp_cntr2;
	temp_cntr2.push_back(cntrs2);
	cv::drawContours(blankImg, temp_cntr2, -1, cv::Scalar(0, 0, 255), 1);

	drawIntersection(blankImg, intersection);
	//drawClipperPoly(blankImg, clipped);

	cv::imshow("", blankImg);
	cv::waitKey(0);

	
}

void testMargisnspeed()
{
	simG::Timer::ScopeTimer sc;
	auto zeromat = simG::algorithms::generateZeroMatrix(500, 500);
	cv::cvtColor(zeromat, zeromat, cv::COLOR_GRAY2BGR);
	cv::Rect rect(100, 100, 320, 320);

	cv::rectangle(zeromat, rect, { 255,0,255 });
	simG::ContentMargins c;
	auto bounds = c.getBoundsRelativeTo(rect);
	//print(bounds);
	cv::rectangle(zeromat, bounds, { 0,0,255 });

	cv::imshow("", zeromat);
	cv::waitKey(0);
}

int main()
{

	// TODO FIND CONTOURS !!!CV_CHAIN_APPROX_TC89_L1!!!
	// TODO IDEE AutoLabeling for self supervised learning (AI-assisted labeling)
	clipperTest();
	sa::COCOAnnotator annotator("hi", sa::COCOAnnotator::INSTANCE_SEG);
	exit(0);
	//std::variant<int, simG::ImageGenerator> vec;
	// TODO: Use RapidXML for parsing XML; it's header-only and pretty fast. But PugiXML is more convenient and still fast

	//TEST_RAPIDJSON();
	//yoloDetect();
	// ToDo translate Contours https://medium.com/analytics-vidhya/tutorial-how-to-scale-and-rotate-contours-in-opencv-using-python-f48be59c35a2
	// And https://github.com/danielthank/jetson-robot/blob/0db11c6d63df76bbc0a35439181ae3e83351a650/alphabet/letter_detector.py
	//test_overlay();

	//atuo back = Mat::zeros(Size(image.cols, image.rows), CV_8UC1);

	//config_.RandomRotation({ 0, 360 });
	//config_.RandomCrop({ 1024, 576 }, true);
	//config_.RandomNoise(0.0, 10.0);
	//config_.RandomHorizontalFlip(0.5);
	//config_.RandomVerticalFlip(0.5);
	//config_.RandomBrightnessShift({ -20.5, 20.5 });
	simG::ImageGenerator::AugmentationParams params;
	//simG::TransformsList transforms{
	//	AddTransform<simG::RandomBrightness>({-20.5, 20.5})
	//};

	// Defaults
	//params.randomnoise = false;
	//params.BackgroundAugs.axisflipping.do_random_flip = true;
	//params.BackgroundAugs.axisflipping.include_no_flip = true;
	//params.BackgroundAugs.brightness.do_shift = true;
	//params.BackgroundAugs.brightness.brightness_range = { -20.5, 20.5 };
	//params.BackgroundAugs.crop.do_crop = true;
	//params.BackgroundAugs.crop.target_dim = { 1024, 576 };
	//params.BackgroundAugs.crop.keep_aspect = true;

	//params.MaskAugs.rotation.do_rotate = true;
	//params.MaskAugs.rotation.rotation_range = { 0, 360 };
	//params.MaskAugs.brightness.do_shift = true;
	//params.MaskAugs.brightness.brightness_range = { -20.5, 20.5 };

	//simG::transforms::Sequential maskTransforms; //<- Random rotation; radomscale flips, noise
	//simG::transforms::Sequential BckgrTransforms; <-resize, noise, crop; check that random scale is not smaller than resize
	//simG::transforms::Sequential BckgrTransforms;
	//simG::Directory maskDir;
	//simG::Directory maskDir;

	//ToDO add Placement Ruleset
	simG::ImageGenerator generator(R"(C:\Users\ruwen\Desktop\iav_Werkstudent\Dataset\Datasat_keep_aspect\train\images)",
		R"(C:\Users\ruwen\Desktop\Learning_CPP\Synthethic-Image-Generator\Test)", params);
	//MultithreadGenerator mGen(R"(C:\Users\ruwen\Desktop\SyntheticDataGenerator_Bachelor\Dataset\input\templates\transportation\car)");

	auto test_img = cv::imread(R"(C:\Users\ruwen\Desktop\Learning_CPP\Synthethic-Image-Generator\resources\t_00000001.jpg)");

	auto start = std::chrono::high_resolution_clock::now();
	//
	//simG::ImageAugmenter augmenter;
	//auto start = std::chrono::high_resolution_clock::now();

	//overlayImage(back, back, { 0,0 });
	//cv::imshow("", back);
	//cv::waitKey(0);
	//
	// ToDo: check out https://github.com/jbohnslav/opencv_transforms/blob/master/opencv_transforms/transforms.py
	//simG::transforms::RandomRotation layer({ 60,60 });
	//simG::transforms::Sequential transforms({
	//	simG::transforms::RandomScale({1.5, 1.5}),
	//	simG::transforms::RandomCrop({550, 100}, true),
	//	simG::transforms::Resize({500, 128}, false),
	//	simG::transforms::RandomBrightness({-150, -150}),
	//	simG::transforms::GaussianBlur(1.0),
	//	//	//simG::transforms::GaussianBlur(1.0),
	//	//	//simG::transforms::RandomGaussNoise(1.0),
	//	//	//simG::transforms::RandomVerticalFlip(1.0),
	//	//	//simG::transforms::RandomHorizontalFlip(1.0),
	//	//	////put annotator as transform like in https://github.com/LinkedAi/flip

	//	// //  //simG::transforms::RandomRotation90(1.0),
	//	// //  //simG::transforms::RandomRotation180(1.0),
	//	// //  //simG::transforms::RandomRotation270(1.0),
	//	// //  //simG::transforms::RandomRotation({30, 30})
	//	});

	//simG::transforms::Sequential2 trans({
	//	//simG::transforms::RandomScale({1.5, 1.5}),
	//	simG::transforms::RandomBrightness({-150, -150}),

	//	simG::transforms::RandomCrop({550, 100}, true),
	//	simG::transforms::Resize({500, 128}, true),
	//	//simG::transforms::GaussianBlur(1.0),
	//	//simG::transforms::RandomGaussNoise(1.0),
	//	//simG::transforms::RandomVerticalFlip(1.0),
	//	//simG::transforms::RandomHorizontalFlip(1.0),
	//	////put annotator as transform like in https://github.com/LinkedAi/flip

	//   //simG::transforms::RandomRotation90(1.0),
	//   //simG::transforms::RandomRotation180(1.0),
	//   //simG::transforms::RandomRotation270(1.0),
	//   //simG::transforms::RandomRotation({30, 30})
	//	});

	//cv::Mat dst;
	////trans.apply(test_img, dst);
	////trans.add(rb);
	//trans.measureTransforms(test_img, dst);
	//simG::print(dst.size());

	//transforms.apply(test_img.clone(), dst2);
	//cv::imshow("Original", test_img);

	//cv::imshow("dst1", dst);
	//cv::imshow("dst2", dst2);

	//cv::waitKey(0);
	//std::vector<std::variant<simG::transforms::RandomRotation>> vars{
	//	//simG::transforms::RandomRotation({60,60})
	//};

	//auto c = transforms;

	//simG::transforms::Sequential maskTransforms({
	//	//simG::transforms::RandomScale({0.5, 1.5}),
	//	// simG::transforms::RandomCrop({550, 100}, true),
	//	//simG::transforms::Resize({600, 400}, true),
	//	//simG::transforms::RandomBrightness({-50, 50}),
	//	//simG::transforms::GaussianBlur(1.0),
	//	});

	//generator.setThreading(simG::ThreadingStatus::ADJUST_TO_CPU);
	//simG::print(generator.isThreadingEnabled());
	//generator.addTransforms(transforms, simG::APPLY_ON_BCKGROUND);

	//simG::print(generator.bckgrTransforms_.count());

	//auto b = static_cast<simG::transforms::RandomScale>(generator.m_ImageTransformations[0].first.transforms_[0]);

	//simG::print(generator.m_ImageTransformations[0].first.transforms_[0]->getType());

	//generator.addTransforms(transforms, simG::APPLY_ON_MASK);

	//std::vector<std::pair<simG::transforms::Sequential, simG::TransformTarget>> transformsCont;
	//transformsCont.emplace_back(transforms, simG::APPLY_ON_BCKGROUND);

	//transformss.emplace_back(transforms, simG::APPLY_ON_RESULT);

	//for (const auto& tr :imageTransforms)
	//{
	//	if (tr.target == simG::APPLY_ON_BCKGROUND)
	//	{
	//		simG::print("Ja");
	//	}

	//cv::Mat dst;
	//transforms.apply(test_img, dst);
	//cv::imshow("", dst);
	//cv::waitKey(0);

	//simG::Directory dir(R"(C:\Users\ruwen\Desktop\iav_Werkstudent\Dataset\Datasat_keep_aspect\train\images)");

//	std::vector<cv::Mat> img_array;
//	img_array.reserve(10);
////
//////#pragma omp parallel for num_threads(4)
//	for (int i = 0; i < 1; i++)
//	{
//		//std::cout << omp_get_thread_num() << "\n";
//		cv::Mat dst;
//		cv::Mat src;
//		test_img.copyTo(src);
//		transforms.apply(src, dst);
//		img_array.push_back(dst);
//	}
	//simG::subplot(img_array, 4);

	//generator.setThreading(simG::ThreadingStatus::ADJUST_TO_CPU);
	//auto dst = rbright(test_img);

	//mGen.generateThreaded();

	//auto img = cv::imread("./resources/t_00000001.jpg");

	//#pragma omp parallel for num_threads(4)
	//	for (int i = 0; i < 1000; i++)
	//	{
	//		std::cout << omp_get_thread_num() << "\n";
	//	}
		//simG::print(generator.num_workers_);

		//simG::print(cache);
		//simG::print(c);

		//simG::Directory testDir(R"(C:\Users\ruwen\Desktop\SyntheticDataGenerator_Bachelor\Dataset\input\templates\transportation\car)");
		//for (int i = 0; i < v.size(); i++)
		//{
		////	std::cout << "Without Thread" << b[i] << "\n";
		////	std::cout << "With    Thread" << v[i] << "\n";

		//}

		//simG::print(omp_get_num_procs());

		//mGen.generateThreaded();

		//simG::Directory input_dir(R"(C:\Users\ruwen\Desktop\SyntheticDataGenerator_Bachelor\Dataset\input\templates\transportation\car)");

		//load_images(input_dir);
		//auto b = D.absoluteFilePath("ja");

		//cv::resize(img, img, cv::Size(1024, 576), 0, 0, cv::INTER_LINEAR);

		//cv::imshow("Windows", img);
		//cv::waitKey(0);

		//auto result = generator.generate();

		//while (!generator.hasFinished())
		//{
		//	generator.generate();
		//	//simG::print(ImgaGenerator.image_count);
		//}

	//try
	//{
	//	simG::ImageGenerator2D ImgaGenerator("Stuff", "Stuff", 3000, 5,params);
	//}
	//catch (const std::exception& e)
	//{
	//	std::cerr << e.what() << "\n";
	//}

	auto end = std::chrono::high_resolution_clock::now();
	duration<double, std::milli> ms_double = end - start;
	std::cout << ms_double.count() << "ms\n";
	std::string warningText =
		"[WARNING]: Oops. It looks like you tried to enable threading, but forgot to set the '/openmp' flag during compilation. \n"
		"	   >> Threading was therefore disabled for the rest of the program. \n"
		"	   -- \n"
		"	   If you still want to use threading add '-openmp' to your compilation commands or \n"
		"	   select C/C.++->Language, and change 'OpenMP Support' to 'Yes' if using Visual Studio 2019. \n"
		"	   -- \n"
		"	   Otherwise do 'setThreading(simG::ThreadingStatus::DISABLE_THREADING)' to disable the warning. \n";
	//if (!ImgaGenerator.is_generator_valid())
	//{
	//	exit(1);
	//}

	//while (!ImgaGenerator.has_finished())
	//{
	//	ImgaGenerator.generate();
	//	ImgaGenerator.image_count;
	//}
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started:
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file