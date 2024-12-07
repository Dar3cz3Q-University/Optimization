#pragma once

#include "solution.h"
#include "DataType.h"

class FileReader
{
public:
	virtual ~FileReader() = default;
	virtual unique_ptr<DataType> Read(const std::vector<std::filesystem::path> filePaths) = 0;
};
