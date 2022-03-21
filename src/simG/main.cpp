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
#include "../cpputils/optionsmap.h"
#include "../cpputils/timer.h"
#include "../cpputils/FileDirectory.h"

#include "utils/datasetviz.h"
#include "utils/Polygon.h"

#include "../cpputils/typecheck.h"

#include "../../external/clipper/clipper.hpp"

#include <iostream>
#include <chrono>
#include <variant>
#include <cstdio>
#include <optional>
#include <functional>
#include <thread>
#include <iterator>

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
}

void clipperTest()
{
	std::vector<cv::Point> cntrs1 = { {47, 0},{46, 1},{40, 1},{39, 2},{38, 2},{37, 3},{33, 3},{32, 4},{31, 4},{30, 5},{28, 5},{26, 7},{23, 7},{21, 9},{18, 9},{16, 11},{15, 11},{14, 12},{13, 12},{13, 13},{12, 14},{12, 18},{11, 19},{10, 19},{7, 22},{7, 23},{5, 25},{5, 28},{4, 29},{4, 32},{3, 33},{3, 37},{4, 38},{4, 44},{5, 45},{5, 49},{7, 51},{7, 52},{11, 56},{11, 58},{12, 59},{11, 60},{11, 77},{10, 78},{11, 79},{11, 81},{10, 82},{10, 101},{9, 102},{10, 103},{10, 104},{9, 105},{9, 129},{8, 130},{8, 145},{7, 146},{7, 148},{8, 149},{8, 152},{7, 153},{7, 160},{3, 164},{3, 165},{2, 166},{2, 167},{1, 168},{1, 178},{0, 179},{0, 180},{1, 181},{0, 182},{0, 186},{1, 187},{1, 190},{3, 192},{3, 194},{6, 197},{7, 197},{8, 198},{8, 202},{9, 203},{9, 204},{11, 206},{11, 207},{12, 207},{13, 208},{13, 209},{21, 217},{22, 216},{23, 217},{23, 218},{24, 217},{25, 218},{25, 219},{27, 219},{28, 220},{29, 219},{30, 219},{32, 221},{33, 221},{34, 222},{40, 222},{41, 221},{42, 221},{44, 223},{49, 223},{50, 222},{55, 222},{56, 223},{59, 223},{60, 222},{61, 223},{61, 224},{62, 223},{63, 224},{76, 224},{78, 222},{79, 223},{81, 223},{82, 222},{83, 222},{84, 221},{89, 221},{90, 220},{91, 220},{92, 219},{93, 219},{94, 218},{95, 218},{96, 217},{97, 217},{103, 211},{104, 211},{114, 201},{114, 200},{115, 199},{115, 198},{116, 197},{117, 197},{117, 196},{118, 195},{118, 192},{119, 191},{119, 188},{120, 187},{120, 177},{121, 176},{121, 172},{122, 171},{122, 160},{123, 159},{123, 154},{124, 153},{124, 152},{125, 151},{130, 151},{131, 150},{132, 150},{133, 149},{134, 149},{137, 146},{138, 146},{138, 145},{139, 144},{140, 144},{140, 143},{141, 142},{141, 140},{138, 137},{134, 137},{133, 138},{130, 138},{129, 139},{127, 139},{126, 140},{124, 138},{124, 120},{125, 119},{125, 63},{126, 62},{126, 49},{125, 48},{125, 45},{124, 44},{124, 35},{123, 34},{123, 32},{122, 31},{122, 27},{121, 26},{121, 24},{120, 23},{120, 19},{119, 18},{119, 17},{118, 16},{118, 15},{116, 13},{116, 12},{113, 9},{112, 9},{110, 7},{109, 7},{107, 5},{104, 5},{102, 3},{98, 3},{97, 2},{96, 2},{95, 1},{84, 1},{83, 0} };
	std::vector<cv::Point> cntrs2 = { {144, 30},{143, 31},{138, 31},{137, 32},{136, 31},{134, 33},{133, 33},{132, 34},{131, 34},{130, 35},{129, 35},{127, 37},{127, 38},{126, 39},{126, 40},{125, 41},{124, 41},{124, 42},{122, 44},{122, 46},{120, 48},{120, 49},{118, 51},{118, 52},{117, 53},{116, 53},{116, 55},{114, 57},{114, 58},{112, 60},{112, 61},{111, 62},{111, 63},{110, 64},{110, 65},{108, 67},{109, 68},{109, 69},{108, 70},{108, 71},{107, 72},{107, 76},{105, 78},{106, 79},{105, 80},{105, 87},{104, 88},{105, 89},{104, 90},{104, 93},{103, 94},{104, 95},{104, 96},{103, 97},{103, 103},{102, 104},{102, 110},{101, 111},{101, 116},{100, 117},{101, 118},{101, 119},{100, 120},{101, 121},{101, 123},{100, 124},{100, 133},{99, 134},{99, 145},{100, 146},{99, 147},{99, 151},{100, 152},{100, 153},{99, 154},{100, 155},{100, 156},{99, 157},{100, 158},{99, 159},{99, 161},{98, 162},{97, 162},{96, 161},{94, 161},{93, 160},{93, 161},{92, 162},{91, 162},{91, 164},{90, 165},{90, 166},{91, 167},{91, 169},{95, 173},{96, 173},{98, 175},{100, 175},{102, 177},{103, 177},{104, 178},{103, 179},{104, 180},{104, 182},{105, 183},{105, 188},{106, 189},{106, 192},{107, 193},{107, 200},{108, 201},{108, 205},{109, 206},{109, 210},{110, 211},{110, 214},{111, 215},{111, 218},{112, 219},{111, 220},{112, 221},{112, 222},{113, 223},{113, 224},{114, 225},{113, 226},{116, 229},{116, 230},{118, 232},{117, 233},{118, 233},{123, 238},{123, 239},{125, 241},{126, 241},{128, 243},{129, 243},{130, 244},{130, 245},{132, 245},{134, 247},{135, 247},{136, 248},{137, 248},{138, 249},{140, 249},{141, 250},{144, 250},{145, 251},{149, 251},{150, 252},{155, 252},{156, 253},{187, 253},{188, 252},{189, 252},{190, 253},{193, 253},{194, 252},{199, 252},{200, 251},{204, 251},{205, 250},{206, 250},{207, 249},{208, 249},{208, 248},{209, 247},{210, 247},{214, 243},{215, 243},{215, 242},{220, 237},{220, 236},{223, 233},{223, 229},{224, 228},{224, 227},{225, 226},{225, 225},{228, 222},{228, 220},{231, 217},{231, 215},{232, 214},{232, 213},{233, 212},{233, 203},{232, 202},{232, 201},{231, 200},{231, 198},{230, 197},{230, 195},{229, 194},{229, 192},{227, 190},{227, 186},{226, 185},{226, 154},{227, 153},{227, 137},{226, 136},{227, 135},{227, 134},{226, 133},{227, 132},{226, 131},{227, 130},{226, 129},{226, 109},{227, 108},{227, 104},{228, 103},{228, 102},{227, 101},{227, 100},{228, 99},{228, 92},{233, 87},{233, 86},{234, 85},{234, 84},{235, 83},{235, 82},{236, 81},{236, 80},{237, 79},{237, 63},{236, 62},{236, 60},{235, 59},{235, 57},{233, 55},{233, 54},{231, 52},{231, 51},{229, 51},{227, 49},{227, 46},{225, 44},{225, 43},{224, 42},{225, 41},{224, 41},{222, 39},{221, 39},{220, 38},{219, 38},{217, 36},{216, 36},{215, 35},{211, 35},{210, 34},{209, 34},{208, 33},{203, 33},{202, 32},{202, 31},{201, 32},{199, 32},{198, 31},{189, 31},{188, 30} };
	scaleContours(cntrs2, 1.25);
	simG::algorithms::translateContour(cntrs1, 30, 50);
	simG::algorithms::translateContour(cntrs2, -65, 30);
	// will be called per object once at beginning
	simG::algorithms::simplifyContour(cntrs1, cntrs1, 0.0012);
	simG::algorithms::simplifyContour(cntrs2, cntrs2, 0.0012);

	auto start = std::chrono::high_resolution_clock::now();
	auto t0 = simG::Timer::startTimer();

	auto overlap = simG::algorithms::fastContourIntersect(cntrs1, cntrs2);
	simG::Timer::timeElapsed(t0);

	if (!overlap)
	{
		auto t1 = simG::Timer::startTimer();
		//auto inside = simG::coreutils::doesPolygonContainPoint(cntrs2, cntrs1[0]);
		//auto inside = simG::coreutils::pointInContourMaybeFasterButNotTested(cntrs1[0], cntrs2);
		auto inside = simG::coreutils::isContourInside(cntrs1, cntrs2);
		simG::Timer::timeElapsed(t1);
		std::cout << "Contain:" << inside << "\n";
	}

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
namespace simG
{
	class PolygonSandbox
	{
	public:
		PolygonSandbox() {
			simG::algorithms::translateContour(m_sample1, 30, 50);

			simG::algorithms::simplifyContour(m_sample1, m_sample1, 0.0024);
			simG::algorithms::simplifyContour(m_sample2, m_sample2, 0.0024);
		}

		void update(int key)
		{
			if (key == 27)
			{
				exit(0);
			}
			else if (key == 100)
			{
				simG::algorithms::translateContour(m_sample2, 1, 0);
			}
			else if (key == 97)
			{
				simG::algorithms::translateContour(m_sample2, -1, 0);
			}
			else if (key == 115)
			{
				simG::algorithms::translateContour(m_sample2, 0, 1);
			}
			else if (key == 119)
			{
				simG::algorithms::translateContour(m_sample2, 0, -1);
			}
			else if (key == 43)
			{
				m_scale = m_scale + 0.01;
				simG::coreutils::scaleContour(m_sample2, m_scale);
			}
			else if (key == 45)
			{
				m_scale = m_scale - 0.01;
				simG::coreutils::scaleContour(m_sample2, m_scale);
			}
		}

		void draw()
		{
			bool inside = false;
			auto t0 = simG::Timer::startTimer();
			auto overlap = simG::algorithms::fastContourIntersect(m_sample1, m_sample2);
			simG::Timer::timeElapsed(t0);
			std::cout << "Overlap:" << overlap << "\n";

			if (!overlap)
			{
				inside = simG::coreutils::isContourInside(m_sample1, m_sample2);
			}
			std::cout << "Contain:" << inside << "\n";

			auto poly1 = simG::coreutils::cvContourToClipperPoly(m_sample1);
			auto poly2 = simG::coreutils::cvContourToClipperPoly(m_sample2);

			auto area = simG::coreutils::polygonIntersectArea(poly1, poly2);
			std::cout << "overlap Area: " << (abs(area) / abs(ClipperLib::Area(poly1))) * 100 << "%\n";

			auto t1 = simG::Timer::startTimer();
			auto intersection = simG::coreutils::getPolygonIntersection(poly1, poly2);
			simG::Timer::timeElapsed(t1);

			CVContours tmp1 = { m_sample1 };
			CVContours tmp2 = { m_sample2 };
			cv::Mat blankImg = cv::Mat::zeros(cv::Size(500, 500), CV_8UC1);
			cv::cvtColor(blankImg, blankImg, cv::COLOR_GRAY2BGR);
			cv::drawContours(blankImg, tmp1, -1, cv::Scalar(255, 0, 0), 1);
			cv::drawContours(blankImg, tmp2, -1, cv::Scalar(0, 0, 255), 1);

			print(intersection.size());
			drawIntersection(blankImg, intersection);

			cv::Mat blankImg2 = cv::Mat::zeros(cv::Size(500, 500), CV_8UC1);

			if (overlap && !inside)
			{
				auto t2 = simG::Timer::startTimer();
				auto clipped = simG::coreutils::clipperPolyToCvContour(simG::coreutils::clipPolygon(poly1, intersection));
				simG::Timer::timeElapsed(t2);
				CVContours tmp3 = { clipped };
				cv::drawContours(blankImg2, tmp3, -1, cv::Scalar(255, 0, 255), 1);
			}

			cv::imshow("", blankImg);
			cv::imshow("clipped", blankImg2);

			auto k = cv::waitKey(30);
			update(k);
		}

		void findEps()
		{
			double tolerance = 1.0;
			std::vector<cv::Point2f> srcCntr{ {210.5, 462.32},
			{ 200.32, 453.83 },
			{ 200.32, 447.04 },
			{ 210.5, 433.46 },
			{ 207.11, 421.57 },
			{ 208.81, 413.09 },
			{ 207.11, 385.92 },
			{ 207.11, 377.44 },
			{ 202.02, 340.09 },
			{ 205.41, 299.35 },
			{ 207.11, 275.58 },
			{ 196.92, 287.46 },
			{ 190.13, 294.25 },
			{ 179.95, 287.46 },
			{ 176.55, 278.97 },
			{ 183.34, 273.88 },
			{ 193.53, 268.79 },
			{ 196.92, 267.09 },
			{ 196.92, 233.14 },
			{ 200.32, 205.98 },
			{ 203.71, 199.19 },
			{ 217.29, 192.4 },
			{ 220.69, 166.93 },
			{ 218.99, 151.65 },
			{ 222.39, 134.68 },
			{ 232.57, 124.49 },
			{ 247.85, 124.49 },
			{ 258.04, 131.28 },
			{ 264.83, 141.47 },
			{ 261.43, 170.33 },
			{ 268.22, 192.4 },
			{ 281.8, 195.79 },
			{ 283.5, 205.98 },
			{ 288.59, 239.93 },
			{ 291.99, 256.91 },
			{ 285.2, 268.79 },
			{ 280.11, 273.88 },
			{ 268.22, 260.3 },
			{ 269.92, 294.25 },
			{ 269.92, 323.11 },
			{ 266.53, 348.58 },
			{ 261.43, 368.95 },
			{ 259.73, 396.11 },
			{ 254.64, 418.18 },
			{ 251.25, 433.46 },
			{ 261.43, 443.64 },
			{ 261.43, 453.83 },
			{ 251.25, 460.62 },
			{ 246.15, 458.92 },
			{ 244.46, 447.04 },
			{ 239.36, 433.46 },
			{ 234.27, 426.67 },
			{ 234.27, 419.88 },
			{ 235.97, 406.3 },
			{ 237.67, 385.92 },
			{ 239.36, 367.25 },
			{ 234.27, 343.48 },
			{ 230.88, 375.74 },
			{ 227.48, 409.69 },
			{ 227.48, 423.27 },
			{ 229.18, 436.85 },
			{ 220.69, 438.55 },
			{ 220.69, 447.04 },
			{ 217.29, 457.22 },
			{ 213.9, 464.01 }
			};

			for (int i = 0; i < 1000; i++)
			{
				tolerance /= 1.25;
				CVContour dstCntr;

				auto eps = tolerance * cv::arcLength(m_sample1, true);
				cv::approxPolyDP(m_sample2, dstCntr, 1.66273, true);
				print(dstCntr.size());
				break;
				if (dstCntr.size() >= 25 && dstCntr.size() <= 35)
				{
					print(dstCntr.size());
					print(eps);
					print(tolerance);
					break;
				}
			}
		}

	private:
		CVContour m_sample1 = { {47, 0},{46, 1},{40, 1},{39, 2},{38, 2},{37, 3},{33, 3},{32, 4},{31, 4},{30, 5},{28, 5},{26, 7},{23, 7},{21, 9},{18, 9},{16, 11},{15, 11},{14, 12},{13, 12},{13, 13},{12, 14},{12, 18},{11, 19},{10, 19},{7, 22},{7, 23},{5, 25},{5, 28},{4, 29},{4, 32},{3, 33},{3, 37},{4, 38},{4, 44},{5, 45},{5, 49},{7, 51},{7, 52},{11, 56},{11, 58},{12, 59},{11, 60},{11, 77},{10, 78},{11, 79},{11, 81},{10, 82},{10, 101},{9, 102},{10, 103},{10, 104},{9, 105},{9, 129},{8, 130},{8, 145},{7, 146},{7, 148},{8, 149},{8, 152},{7, 153},{7, 160},{3, 164},{3, 165},{2, 166},{2, 167},{1, 168},{1, 178},{0, 179},{0, 180},{1, 181},{0, 182},{0, 186},{1, 187},{1, 190},{3, 192},{3, 194},{6, 197},{7, 197},{8, 198},{8, 202},{9, 203},{9, 204},{11, 206},{11, 207},{12, 207},{13, 208},{13, 209},{21, 217},{22, 216},{23, 217},{23, 218},{24, 217},{25, 218},{25, 219},{27, 219},{28, 220},{29, 219},{30, 219},{32, 221},{33, 221},{34, 222},{40, 222},{41, 221},{42, 221},{44, 223},{49, 223},{50, 222},{55, 222},{56, 223},{59, 223},{60, 222},{61, 223},{61, 224},{62, 223},{63, 224},{76, 224},{78, 222},{79, 223},{81, 223},{82, 222},{83, 222},{84, 221},{89, 221},{90, 220},{91, 220},{92, 219},{93, 219},{94, 218},{95, 218},{96, 217},{97, 217},{103, 211},{104, 211},{114, 201},{114, 200},{115, 199},{115, 198},{116, 197},{117, 197},{117, 196},{118, 195},{118, 192},{119, 191},{119, 188},{120, 187},{120, 177},{121, 176},{121, 172},{122, 171},{122, 160},{123, 159},{123, 154},{124, 153},{124, 152},{125, 151},{130, 151},{131, 150},{132, 150},{133, 149},{134, 149},{137, 146},{138, 146},{138, 145},{139, 144},{140, 144},{140, 143},{141, 142},{141, 140},{138, 137},{134, 137},{133, 138},{130, 138},{129, 139},{127, 139},{126, 140},{124, 138},{124, 120},{125, 119},{125, 63},{126, 62},{126, 49},{125, 48},{125, 45},{124, 44},{124, 35},{123, 34},{123, 32},{122, 31},{122, 27},{121, 26},{121, 24},{120, 23},{120, 19},{119, 18},{119, 17},{118, 16},{118, 15},{116, 13},{116, 12},{113, 9},{112, 9},{110, 7},{109, 7},{107, 5},{104, 5},{102, 3},{98, 3},{97, 2},{96, 2},{95, 1},{84, 1},{83, 0} };
		CVContour m_sample2 = { {144, 30},{143, 31},{138, 31},{137, 32},{136, 31},{134, 33},{133, 33},{132, 34},{131, 34},{130, 35},{129, 35},{127, 37},{127, 38},{126, 39},{126, 40},{125, 41},{124, 41},{124, 42},{122, 44},{122, 46},{120, 48},{120, 49},{118, 51},{118, 52},{117, 53},{116, 53},{116, 55},{114, 57},{114, 58},{112, 60},{112, 61},{111, 62},{111, 63},{110, 64},{110, 65},{108, 67},{109, 68},{109, 69},{108, 70},{108, 71},{107, 72},{107, 76},{105, 78},{106, 79},{105, 80},{105, 87},{104, 88},{105, 89},{104, 90},{104, 93},{103, 94},{104, 95},{104, 96},{103, 97},{103, 103},{102, 104},{102, 110},{101, 111},{101, 116},{100, 117},{101, 118},{101, 119},{100, 120},{101, 121},{101, 123},{100, 124},{100, 133},{99, 134},{99, 145},{100, 146},{99, 147},{99, 151},{100, 152},{100, 153},{99, 154},{100, 155},{100, 156},{99, 157},{100, 158},{99, 159},{99, 161},{98, 162},{97, 162},{96, 161},{94, 161},{93, 160},{93, 161},{92, 162},{91, 162},{91, 164},{90, 165},{90, 166},{91, 167},{91, 169},{95, 173},{96, 173},{98, 175},{100, 175},{102, 177},{103, 177},{104, 178},{103, 179},{104, 180},{104, 182},{105, 183},{105, 188},{106, 189},{106, 192},{107, 193},{107, 200},{108, 201},{108, 205},{109, 206},{109, 210},{110, 211},{110, 214},{111, 215},{111, 218},{112, 219},{111, 220},{112, 221},{112, 222},{113, 223},{113, 224},{114, 225},{113, 226},{116, 229},{116, 230},{118, 232},{117, 233},{118, 233},{123, 238},{123, 239},{125, 241},{126, 241},{128, 243},{129, 243},{130, 244},{130, 245},{132, 245},{134, 247},{135, 247},{136, 248},{137, 248},{138, 249},{140, 249},{141, 250},{144, 250},{145, 251},{149, 251},{150, 252},{155, 252},{156, 253},{187, 253},{188, 252},{189, 252},{190, 253},{193, 253},{194, 252},{199, 252},{200, 251},{204, 251},{205, 250},{206, 250},{207, 249},{208, 249},{208, 248},{209, 247},{210, 247},{214, 243},{215, 243},{215, 242},{220, 237},{220, 236},{223, 233},{223, 229},{224, 228},{224, 227},{225, 226},{225, 225},{228, 222},{228, 220},{231, 217},{231, 215},{232, 214},{232, 213},{233, 212},{233, 203},{232, 202},{232, 201},{231, 200},{231, 198},{230, 197},{230, 195},{229, 194},{229, 192},{227, 190},{227, 186},{226, 185},{226, 154},{227, 153},{227, 137},{226, 136},{227, 135},{227, 134},{226, 133},{227, 132},{226, 131},{227, 130},{226, 129},{226, 109},{227, 108},{227, 104},{228, 103},{228, 102},{227, 101},{227, 100},{228, 99},{228, 92},{233, 87},{233, 86},{234, 85},{234, 84},{235, 83},{235, 82},{236, 81},{236, 80},{237, 79},{237, 63},{236, 62},{236, 60},{235, 59},{235, 57},{233, 55},{233, 54},{231, 52},{231, 51},{229, 51},{227, 49},{227, 46},{225, 44},{225, 43},{224, 42},{225, 41},{224, 41},{222, 39},{221, 39},{220, 38},{219, 38},{217, 36},{216, 36},{215, 35},{211, 35},{210, 34},{209, 34},{208, 33},{203, 33},{202, 32},{202, 31},{201, 32},{199, 32},{198, 31},{189, 31},{188, 30} };
		float m_scale = 1.0;
	};
}

simG::CVContour simplifyContourFixed(const simG::CVContour& cntr, size_t targetSize)
{
	simG::CVContour dstCntr;

	auto n_iter = 0;
	auto max_iter = 100;

	double lb = 0.;
	double  ub = 1.;

	while (true)
	{
		n_iter += 1;
		if (n_iter > max_iter)
		{
			return cntr;
		}

		auto k = (lb + ub) / 2.;
		auto eps = k * cv::arcLength(cntr, true);

		cv::approxPolyDP(cntr, dstCntr, eps, true);

		if (dstCntr.size() > targetSize)
		{
			lb = (lb + ub) / 2.;
		}
		else if (dstCntr.size() < targetSize)
		{
			ub = (lb + ub) / 2.;
		}
		else
		{
			return dstCntr;
		}
	}
}

class Pet
{
public:
	Pet(const std::string& name, int age)
		:name_(name), age_(age)
	{
	}
	~Pet() = default;

	void show() { std::cout << "I am " << name_ << " and I am " << age_ << " years old.\n"; }

protected:
	std::string name_;
	int age_;
};

class GenericValue
{
public:
	enum class ValueType { NUM_INTEGER, NUM_DOUBLE, BOOLEAN, STRING, NULLPTR };
	GenericValue(int i) : value_(i), type_(ValueType::NUM_INTEGER) { print("i"); }
	GenericValue(double d) : value_(d), type_(ValueType::NUM_DOUBLE) { print("d"); }
	GenericValue(bool b) : value_(b), type_(ValueType::BOOLEAN) { print("bool"); }
	GenericValue(const std::string& str) : value_(str), type_(ValueType::STRING) { print("string"); }
	GenericValue(const char* str) : value_(std::string(str)), type_(ValueType::STRING) { print("char"); }
	GenericValue(std::nullptr_t nullp) : value_(nullp), type_(ValueType::NULLPTR) { print("NULL"); }

public:
	ValueType type() { return type_; }
public:
	bool isInt() const { return type_ == ValueType::NUM_INTEGER; }
	bool isDouble() const { return type_ == ValueType::NUM_DOUBLE; }
	bool isNumeric() const { return isInt() || isDouble(); }
	bool isBoolean() const { return type_ == ValueType::BOOLEAN; }
	bool isString() const { return type_ == ValueType::STRING; }
	bool isNull() const { return type_ == ValueType::NULLPTR; }

public:
	int getInt() const { return std::get<int>(value_); };
	double getDouble() const { return std::get<double>(value_); };
	bool getBoolean() const { return std::get<bool>(value_); };
	std::string getString() const {
		if (isString())
			return std::get<std::string>(value_);

		if (isNull())
			return std::string("null");

		if (isInt())
			return std::to_string(getInt());

		if (isDouble())
			return std::to_string(getDouble());

		return std::to_string(getBoolean());
	};

private:
	ValueType type_;
	std::variant<int, double, bool, std::string, std::nullptr_t> value_;
};

std::ostream& operator<< (std::ostream& out, const GenericValue& gval)
{
	out << gval.getString() << "\n";
	return out;
}

template<class ValueType>
class Options
{
public:
	Options() = default;
	void store(const std::string& key, ValueType value)
	{
		optionmap_[key] = value;
	}

	template<typename T>
	typename std::enable_if<cpputils::is_variant_v<ValueType>, T>::type get(const std::string& key) const
	{
		return std::get<T>(optionmap_.at(key));
	}

	template<typename T = ValueType>
	typename std::enable_if<!cpputils::is_variant_v<ValueType>, T>::type get(const std::string& key) const
	{
		return optionmap_.at(key);
	}

	template<typename T>
	typename std::enable_if<cpputils::is_variant_v<ValueType>, T>::type is(const std::string& key) const
	{
		if (auto rptr = std::get_if<T>(&optionmap_.at(key)))
		{
			return true;
		}
		return false;
	}

	std::vector<std::string> get_keys() const
	{
		std::vector<std::string> keyvec;
		for (auto const& opt : optionmap_)
		{
			keyvec.push_back(opt.first);
		}
		return keyvec;
	}

	bool has_key(const std::string& key) const
	{
		if (optionmap_.find(key) == optionmap_.end())
		{
			return false;
		}
		return true;
	}

	std::string dump() const
	{
		std::stringstream ss;
		for (const auto& [k, v] : optionmap_)
		{
			ss << "[" << k << "] : ";
			std::visit([&ss](const auto& x) { ss << x; }, v);
			ss << "\n";
		}
		return ss.str();
	}

private:
	std::map<std::string, ValueType> optionmap_;
};

template<typename T>
std::ostream& operator<<(std::ostream& os, const Options<T>& opt)
{
	os << opt.dump();
	return os;
}

#include <filesystem>
using cpputils::FileDirectory;
//bool ExpandWildCard(std::vector<std::string>& names, std::vector<std::string>& result, const std::regex& wildcard) {
//	auto oldsize = result.size();
//	std::copy_if(std::begin(names), std::end(names),
//		std::back_inserter(result),
//		[&](std::string const& name) {
//			return std::regex_match(name, wildcard);
//		}
//	);
//
//	return (result.size() > oldsize);
//}

void get_date_string()
{
	std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
	std::time_t start_time = std::chrono::system_clock::to_time_t(now);
	char timedisplay[100];
	struct tm buf;
	errno_t err = localtime_s(&buf, &start_time);
	if (std::strftime(timedisplay, sizeof(timedisplay), "%H:%M:%S", &buf)) {
		std::cout << timedisplay << '\n';
	}
}

#include <ctime>
#include "../cpputils/stringutils.h"
#include <array>

#include "../cpputils/cdebug.h"

//#define cDebug(msg) cDebug(msg, __FILE__, __LINE__)

//template<typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
//testover() {}

//
//
//
//template <class T>
//typename std::enable_if<std::is_floating_point<T>::value, T>::type
//testover() {
//	// ....
//}
//
//template<> int testover() {}

#include <type_traits>

//template<typename T>
//std::optional<T> str_convert_container(std::string_view str)
//{
//	return converters::StrConverter<T>::convert(str);
//}
//
//template<typename Container>
//std::optional<Container> str_cast_container(std::string_view str, const char delim)
//{
//	using dtype = typename Container::value_type;
//}
//
//template<typename Container>
//std::optional<Container> str_cast_container(std::string_view str, const char delim, const char prefix, const char suffix)
//{
//}

using namespace std;

template<typename T, typename = void>
struct is_std_heap_container : std::false_type
{};

template<typename T>
struct is_std_heap_container<T,
	std::void_t<decltype(std::declval<T>().begin()),
	decltype(std::declval<T>().end()),
	typename T::value_type
	>> : std::true_type // will  be enabled for iterable objects
{};

//template<typename To>
//typename std::enable_if<is_std_container<To>::value,
//	std::optional<To>>::type string_cast(std::string_view view)
//{
//	return {};
//}

template<typename To>
std::optional<To> string_cast(std::string_view view)
{
	return {};
}

template<typename To>
typename std::enable_if<is_std_heap_container<To>::value,
	std::optional<To>>::type string_cast(std::string_view view, char delim)
{
	return {};
}

template<typename To>
typename std::enable_if<is_std_heap_container<To>::value,
	std::optional<To>>::type string_cast(std::string_view view, char delim,
		char prefix, char suffix)
{
	return {};
}

//template<typename InContainer, typename OutIter>
//bool str_cast_for_each(InIter inBegin, InIter inEnd, OutIter outEnd)
//{
//	using dtype = typename OutIter::value_type;
//	for (InIter it = inBegin; it != inEnd; it++)
//
//
//

template<typename DstType, typename SrcType, template<typename...> class ConType, typename... Args>
cpputils::cast_result<ConType<DstType>> str_cast_for_each(const ConType<SrcType, Args...>& src)
{
	static_assert((std::is_same_v<SrcType, std::string> || std::is_same_v<SrcType, std::string_view>),
		"function template 'str_cast_for_each' only accepts containers of type 'std::string' or 'std::string_view'");

	if (src.empty()) // Fail
		return {};

	ConType<DstType> out;
	if constexpr (cpputils::has_reserve<ConType<DstType>>::value)
		out.reserve(src.size());

	for (const auto& elem : src)
	{
		if (auto result = cpputils::str_cast<DstType>(elem))
			out.push_back(*result);
	}

	if (out.size() != src.size()) // Atleast one cast failed
		return {};

	return out;
}

#define DECLARE_SUB_CLASS(sub_class_name, base_class_name, value1) \
print(##sub_class_name)
//class sub_class_name:base_class_name \
//{ \
//public: \
//    virtual int initialize(const void *); \
//    virtual int run(const void *); \
//    virtual void reset(); \
//    virtual int output(const char*); \
//    virtual void terminate(); \
//private: \
//    static const char m_szValue=#value1; \
//};

template<typename T = std::string>
T gete()
{
}

template <typename T, size_t Size>
std::array<T, Size> getArray(const std::string& key)
{
	//USAGE:

	//std::string str = "113,132,1,yes";
	//cpputils::Timer t1;
	//auto resAr = getArray<bool, 6>(str);

	std::array<T, Size> valAr{};
	auto strVec = cpputils::str_split(key, ',');
	for (size_t i = 0; i < Size && i < strVec.size(); i++)
	{
		if (auto result = cpputils::str_cast<T>(strVec[i]))
		{
			valAr[i] = *result;
		}
	}

	return valAr;
}

#include "../cpputils/iniparser.h"
#include "../cpputils/prettyprint.h"

#include "../cpputils/magic_enum.h"
enum class Color { RED = 2, BLUE = 4, GREEN = 8 };

template<typename T, typename = typename
	std::enable_if<std::is_enum_v<T>>::type>
	void enum2Str(T enum_value)
{
	auto enum_value_str = magic_enum::enum_name(enum_value);
	auto enum_type_name_str = magic_enum::enum_type_name<T>();

	std::cout << enum_type_name_str << "::" << enum_value_str;
}

template<typename T, typename = typename
	std::enable_if<std::is_enum_v<T>>::type>
	std::optional<T> str2Enum(std::string_view enum_str) {
	auto pos = enum_str.find("::");
	if (pos != std::string_view::npos)
		enum_str = enum_str.substr(pos + 2);
	auto enum_type = magic_enum::enum_cast<T>(enum_str);
	return enum_type;
}



#include "../cpputils/ECS.h"

class TransformationComponent : public ECS::ESCComponent<TransformationComponent>
{

};


class w : public ECS::ESCComponent<w>
{

};

class e : public ECS::ESCComponent<e>
{

};

struct trans
{};



template <class T>
struct BaseComponent
{
	T* as()
	{
		return static_cast<T*>(this);
	}

	virtual ~BaseComponent() {}
};

template <typename T>
struct Component : public BaseComponent<T>
{
	virtual ~Component() {}
};

struct PositionComponent : public Component<PositionComponent>
{
	float x, y, z;

	virtual ~PositionComponent() {}
};

template<typename T>
void adwda(T& base)
{
	constexpr auto be = base.as();
	//print(std::is_same<be, PositionComponent*>::value);

	//print(std::is_same_v<be, PositionComponent*>);
}



class Testi
{
public:
	Testi(std::unique_ptr<int> p)
		: p_(std::move(p)){}
	

private:
	std::unique_ptr<int> p_;
};


void doStuff(std::nullptr_t nllptr)
{
	print(nllptr);
}


using cpputils::pprint;

template<typename T>
void smartprint(std::unique_ptr<T>& ptr)
{
	if (ptr)
		pprint(*ptr);
	else
		pprint(nullptr);
}

//template<typename T>
//auto doStuff(const T& t)
//{
//	//return cpputils::converters::ToStrConverter<T>::convert(t);
//}
int main()
{
	//cpputils::pprint_internal(intPtr);
	//cpputils::converters::ToStrConverter<std::unique_ptr<int>>::convert(intPtr);
	//pprint(intPtr);
	//cpputils::stringify(30);

	//print(intPtr);

	//pprint(nullptr);
	//doStuff(intPtr);

	//cpputils::pprint(intPtr);

	//if (b)
	//{
	//	print("hey");
	//}

	//simG::ThreadingStatus status = simG::ThreadingStatus::NUM_THREADS_4;
	//std::string str = "We think in , but we live in details.";

	//std::string_view status_str{ "NUM_THREADS_8" };
	//auto cresult = cpputils::converters::FromStrConverter<simG::ThreadingStatus>::convert(status_str);

	//std::cout << cpputils::stringify(cresult);
	//auto pos = status_str.find("::");
	//if (pos != std::string_view::npos)
	//{
	//	cpputils::pprint("Success");
	//	cpputils::pprint(status_str.substr(pos + 2));
	//}
	//else
	//{
	//	cpputils::pprint(status_str);
	//}

	//if (auto status = str2Enum<simG::ThreadingStatus>(color_name))
	//{
	//	cpputils::pprint(*status);
	//}
	//cpputils::pprint(color_name);
	std::complex<double> foo = 1. + 2.5i;
	std::complex<double> bar = 9. + 4i;
	std::optional<int> opt = 5;
	std::optional<int> opt2;
	cpputils::pprint(bar);


	//p.first = 3;
	//p.second = 241.1;

	//cpputils::pprint(m);

	//cpputils::print(bar, 39, opt, opt2, foo2);

	exit(0);
	std::string hey = "no";
	cpputils::Timer t;
	auto result = cpputils::str_cast<bool>(hey);

	if (result)
	{
		t.toc();
		print(*result);
	}

	cpputils::IniConfig config;
	//config.getVectorOr()
	//std::string s = "1, 22ad21313m";
	//cpputils::str_remove_ws(s);
	//auto strVec = cpputils::str_split(s, ',');
	//auto intVec = str_cast_for_each<int>(strVec);
	//std::list<int> e(intVec->begin(), intVec->end());
	//print(stringify(e));

	config.set("hey", "ja", 30);
	//config.getValue<>

	//try
	//{
	//	//auto b = config.getVector<std::vector<int>>("hey", "ja");
	//}
	//catch (std::out_of_range& e)
	//{
	//	std::cout << "yeet" << std::endl;
	//}

	//auto bb = cpputils::str_cast<std::vector<int>>(s);

	//auto e = config.getCollection<std::list<int>>();
	//
	//
	//
	//cpputils::str_cast()

	exit(0);
	//IniOptions e;
	//std::vector<int> vec;

	//cpputils::Timer t;

	//t.toc();

	//print(*e);

	//{
	//}

	//
	//cpputils::Timer t;

	//std::vector<double> out;
	//out.reserve(we.size());

	//for (const auto& e : we)
	//{
	//	if (auto result = str_cast<double>(e))
	//	{
	//		out.push_back(*result);
	//	}
	//}
	//t.toc();

	//

	//std::string s = "{ 1, 2,    3,4 ,5,6 }";
	//std::string s2 = " 1, 2   ,3    ,4,5,  6 ";

	////auto r1 = str_cast_vec<int>(s, ',', '{', '}');
	////print(r1.value());
	//
	//std::list<double> e{ 2.0, 2.3, 15012.2 };
	//auto eb = stringify(e);
	//print(eb);
	//
	//
	//
	//
	//

	//auto r21 = str_cast_for_each<int>(r20);

	//print(r21.has_value());
	////print(r21.value().size());

	//std::vector<int> s = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 105, 89, 104, 90, 104, 93, 103, 94, 104, 95, 104, 96, 103, 97, 103, 103, 102, 104, 102, 110, 101, 111, 101, 116, 100, 117, 101, 118, 101, 119, 100, 120, 101, 121, 101, 123, 100, 124, 100, 133, 99, 134, 99, 145, 100, 146, 99, 147, 99, 151 };

	//auto e = str_cast_for_each(b.begin(), b.end(), k.end());

	//std::vector<std::string> b;
	//auto e = str_cast_for_each<std::vector<double>>(b);
	//

	//
	//auto e = str_cast_for_each<double>(e);

	//auto e = string_cast<std::vector<int>>("hey", ',');

	//std::string e;
	//auto b = string_cast<std::vector<int>, ','>(e);

	//auto e
	//auto e = string_cast()t
	//std::vector<int> s = { 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,105,89,104,90,104,93,103,94,104,95,104,96,103,97,103,103,102,104,102,110,101,111,101,116,100,117,101,118,101,119,100,120,101,121,101,123,100,124,100,133,99,134,99,145,100,146,99,147,99,151 };
	////std::array<int, 2> s = { 1, 51 };

	//cpputils::Timer t;
	//std::string result = str_join(s.begin(), s.end(), ",");
	//t.toc();
	//print(result);

	//	std::vector<int> data = { 104, 97, 104, 97 };
	//std:string e{ std::begin(data), std::end(data) };
	//	print(e);

		// add str_cast<std::chrono::timepoint>

		//print(e.value_or(std::vector<int>({123})));

		//std::string strD;
		//static const std::array<std::pair<std::string, bool>, 8> s2b{ {
		//		{"1", true}, { "true", true }, { "yes", true }, { "on", true },
		//		{ "0", false }, { "false", false }, { "no", false }, { "off", false }} };

		//for (const auto& p : s2b)
		//{
		//	if (p.first == strD)
		//	{
		//		print(p.second);
		//	}
		//}

		//auto e = str_convert<std::vector<int>>(

		//auto e = converters::StrConverter<std::vector<int>>::convert()

		//auto b = str_to_vec<int>(s, ',', '{', '}');

		//auto b = str_to_array<std::string,5>(s, ',', '{', '}');

		//std::string s = "{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,105,89,104,90,104,93,103,94,104,95,104,96,103,97,103,103,102,104,102,110,101,111,101,116,100,117,101,118,101,119,100,120,101,121,101,123,100,124,100,133,99,134,99,145,100,146,99,147,99,151}";
		//auto e = cpputils::str_to_numeric_vec<int>(s, ',', '{', '}');

		//cpputils::cDebug(e);
		//cpputils::cDebug("ja");

		//cpputils::CDebug cdebug (std::cout);
		//cdebug << e;

		//t.toc();
		//std::vector<std::string> v;
		//cpputils::Timer t1;
		//parse(v,s, false, ',');
		//t1.toc();

		//str_to_vec<int>(s, ',', '{', '}');

	//try
	//{
	//	print(result);
	//}
	//catch (const std::exception& c)
	//{
	//	print(c.what());
	//}
	//if (error.ec != std::errc())
	//{
	//	std::cout << "That isn't a number.\n";
	//}

	//else { print(result); }

	cpputils::FileDirectory dir(R"(C:\Users\ruwen\Downloads\test_tree)");
	auto k = dir.listEntries("*.weights", FileDirectory::Recursive, FileDirectory::All);
	//print(k);
	//for (const auto& entry : iter)
	//{
	//	print(entry.path().extension());
	//}

	//auto e = wildmatch("sampleja", "sa*ja*.cpp");

	//print(e);

	//std::vector<std::string> v;
	//for (const auto& e : std::filesystem::directory_iterator(R"(C:\Users\ruwen\Downloads\test_tree)"))
	//{
	//	if (!std::filesystem::is_directory(e))
	//	{
	//	}
	//	v.push_back(e.path().filename().string());
	//}

	//E C : \Users\ruwen\Desktop\iav_Werkstudent\IAV - jsonmeta\pyqt5\examples\dbus\remotecontrolledcar\car\car.py

	//t.toc();
	//print(v);
	//print(files);
	//print(files.size());

	//for (const auto& entry : std::filesystem::recursive_directory_iterator(R"(C:\Users\ruwen\Downloads\test_tree)"))
	//{
	//	if (std::filesystem::is_directory(entry))
	//	{
	//		continue;
	//	}
	//	print(entry);
	//}

	//print(dir.listEntries().size());
	//print(tree.str());

	//auto end1 = std::chrono::high_resolution_clock::now();
	//duration<int, std::nano> ms_double1 = end1 - start1;
	//auto e = ms_double1.count();

	//auto e = t;

	//int n = 0;
	//print(&n);
	//int* ptr1 = &n;
	//int* ptr2;
	//ptr2 = ptr1;

	//print("");
	//std::cout << &ptr1 << std::endl;
	//std::cout << &ptr2 << std::endl;
	//print("");

	//std::cout << &*ptr1 << std::endl;
	//print(ptr1);

	exit(0);

	// TODO FIND CONTOURS !!!CV_CHAIN_APPROX_TC89_L1!!!
	// TODO IDEE AutoLabeling for self supervised learning (AI-assisted labeling)
	simG::PolygonSandbox pg;
	//while (true)
	//{
	//	pg.draw();
	//}

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
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file s