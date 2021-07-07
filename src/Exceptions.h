#pragma once
#include <string>
#include <stdexcept>

namespace simG
{
	// Base exception class
	// @class Exception
	class Exception : public std::runtime_error
	{
	public:
		Exception(const std::string& message)
			: std::runtime_error(message)
		{}
	};

	class InvalidDirectoryException : public Exception
	{
	public:
		InvalidDirectoryException()
			: Exception("File not found")
		{}

		InvalidDirectoryException(const std::string& dirname)
			: Exception("Directory not found or emptry: (" + std::string(dirname.begin(), dirname.end()) + ")")
		{}
	};
}