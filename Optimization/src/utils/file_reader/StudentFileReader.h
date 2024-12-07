#pragma once

#include "FileReader.h"

class StudentFileReader : public FileReader
{
public:
	RETURN_TYPE Read(const std::vector<std::filesystem::path> filePaths);
};

