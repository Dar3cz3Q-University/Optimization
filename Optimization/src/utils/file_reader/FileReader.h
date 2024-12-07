#pragma once

#include "solution.h"

class FileReader
{
public:
	virtual ~FileReader() = default;
	virtual solution Read(const std::vector<std::filesystem::path> filePaths) = 0;
};

