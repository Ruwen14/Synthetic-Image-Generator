#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <functional>

namespace simG
{
	// Directory Class is inspired by QDir from the Qt-Framework and provides access to directories and their contents
	class Directory
	{
		friend std::ostream& operator<<(std::ostream& os, const simG::Directory& obj);
	public:
		Directory(const std::string& dir_path);
		~Directory() = default;

		std::string dir_path;
		std::vector<std::string> entries;

		bool dirExists() const;
		bool isEmpty() const;
		void update();

		std::string dirName() const;
		std::string relativeFilePath(const std::string& file_name) const;
		std::string absoluteFilePath(const std::string& file_name) const;
		std::string absoluteDirPath() const;

		std::vector<std::string> listEntries() const;

		std::string cycleEntry(bool shuffle_on_end = true);

		static bool createDirectory(const std::string& dir_path);

	private:
		std::vector<std::string> list_entries(const std::string& path) const;
		int cycle_idx_ = -1; // used by nextEntry()
	};
}
