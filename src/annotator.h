#pragma once

namespace simG
{
	// Write all like an API
	class BasicAnnotator // Abstract-Base-Class for all other annotators
	{
	};

	class COCOAnnotator : public BasicAnnotator
	{
	};
}