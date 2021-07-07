#pragma once
#include <string>
#include <omp.h>
#include "utils/Directory.h"
#include <opencv2/opencv.hpp>


#include "imgaugmenter.h"
#include "annotators/AbstractAnnotator.h"
#include "utils/Directory.h"

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <opencv2/core/mat.hpp>


namespace simG
{
	class MultithreadGeneratorLMAO
	{
	public:
		MultithreadGeneratorLMAO(const std::string& dirName) :m_Dir(dirName) { };
		~MultithreadGeneratorLMAO() = default;
	
		void generate();
		void generateThreaded(int workers = omp_get_num_procs());
	
		int m_frameCount = 0;
	
	public:
		simG::Directory m_Dir;
	};
	
}
