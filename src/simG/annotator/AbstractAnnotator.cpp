#include "AbstractAnnotator.h"

namespace simG::annotators
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