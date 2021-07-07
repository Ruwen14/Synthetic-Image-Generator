#include "MultiThreadGeneratorLMAO.h"



namespace simG
{

	void MultithreadGeneratorLMAO::generate()
	{

	}

	void MultithreadGeneratorLMAO::generateThreaded(int workers /*= omp_get_num_procs()*/)
	{
		cv::Mat img;
#pragma omp parallel for num_threads(18)
		for (int i = 0; i < 10000; i++)
		{
			std::cout << omp_get_thread_num() << "\n";

		}
	}

}