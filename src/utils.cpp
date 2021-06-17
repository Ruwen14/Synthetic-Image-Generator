#include "utils.h"
#include <filesystem>

namespace simG
{
	std::vector<std::string> listdir(const std::string& dir)
	{
		std::vector<std::string> entries;
		for (const auto& entry : std::filesystem::recursive_directory_iterator(dir))
		{
			entries.push_back(entry.path().string());
		}
		return entries;
	}

	bool ispath(const std::string& path)
	{
		return std::filesystem::exists(path);
	}
}