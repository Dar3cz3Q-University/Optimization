#pragma once

#include "FileReader.h"
#include "Lab6_DataType.h"

class Lab6_FileReader : public FileReader
{
public:
	unique_ptr<DataType> Read(const std::vector<std::filesystem::path> filePaths);
};

