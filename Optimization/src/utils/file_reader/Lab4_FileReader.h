#pragma once

#include "FileReader.h"
#include "Lab4_DataType.h"

class Lab4_FileReader : public FileReader
{
public:
	unique_ptr<DataType> Read(const std::vector<std::filesystem::path> filePaths);
};
