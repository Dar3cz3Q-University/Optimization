#pragma once

#include "solution.h"

#define RETURN_TYPE variant<pair<matrix, matrix>>

class FileReader
{
public:
	virtual ~FileReader() = default;
	virtual RETURN_TYPE Read(const std::vector<std::filesystem::path> filePaths) = 0;
};

