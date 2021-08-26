#include "Directory.h"
#include <filesystem>
#include "Random.h"
#include <iostream>
#include <sstream>
namespace simG
{
	std::ostream& operator<<(std::ostream& os, const simG::Directory& obj)
	{
		std::stringstream outstream;
		outstream << "./" << obj.dirName() << "[" << "\n";
		for (const auto& entry : obj.entries)
		{
			outstream << entry << ", " << "\n";
		}
		outstream << "]";

		return os << outstream.str();
	}

	Directory::Directory(const std::string& dir_path)
		: dir_path(dir_path), entries(list_entries(dir_path))
	{
	}

	bool Directory::dirExists() const
	{
		return std::filesystem::is_directory(dir_path);
	}

	// yes
	bool Directory::isEmpty() const
	{
		return entries.size() == 0;
	}

	// rescans dir-entries and updates member
	void Directory::update()
	{
		entries = list_entries(dir_path);
	}

	std::string Directory::dirName() const
	{
		return  dir_path.substr(dir_path.find_last_of("/\\") + 1);
	}

	std::string Directory::relativeFilePath(const std::string& file_name) const
	{
		// Much cheaper
		const char last_element = dir_path.back();

		if (last_element != '/' && last_element != '\\')
		{
			return dir_path + '/' + file_name;
		}
		return dir_path + file_name;
	}

	std::string Directory::absoluteFilePath(const std::string& file_name) const
	{
		std::string rel_path = relativeFilePath(file_name);
		return std::filesystem::absolute(std::filesystem::path(rel_path)).string();
	}

	std::string Directory::absoluteDirPath() const
	{
		return std::filesystem::absolute(dir_path).string();
	}

	std::vector<std::string> Directory::listEntries() const
	{
		return list_entries(dir_path);
	}

	// gets the next file-entry in the directory.
	// Restarts from begining if last element is hit. The entry-list can be shuffled if shuffle_on_end is set true
	std::string Directory::cycleEntry(bool shuffle_on_end /*= true*/)
	{
		if (cycle_idx_ == entries.size() - 1)
		{
			if (shuffle_on_end)
			{
				simG::Random::shuffleList(entries);
			}
			cycle_idx_ = -1;
		}
		++cycle_idx_;
		return entries[cycle_idx_];
	}

	bool Directory::createDirectory(const std::string& dir_path)
	{
		std::filesystem::create_directory(dir_path);
	}

	std::vector<std::string> Directory::list_entries(const std::string& path) const
	{
		std::vector<std::string> file_entries;
		if (std::filesystem::is_directory(path)) // otherwise entries remain empty
		{
			for (const auto& entry : std::filesystem::directory_iterator(path))
			{
				file_entries.push_back(entry.path().filename().string());
			}
		}
		return file_entries;
	}
}