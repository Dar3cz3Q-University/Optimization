#pragma once

#include "FileReader.h"

class StudentFileReader : public FileReader
{
public:
	solution Read(const std::vector<std::filesystem::path> filePaths);
};

