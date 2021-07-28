#include "AbstractAnnotator.h"

namespace simG
{
	AbstractAnnotator::AbstractAnnotator(const std::string& annofile_savepath)
		: anno_file_(annofile_savepath)
	{
	}

	void AbstractAnnotator::close()
	{
		anno_file_.close();
	}
}