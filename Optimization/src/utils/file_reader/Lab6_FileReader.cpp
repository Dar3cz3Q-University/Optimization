#include "pch.h"
#include "Lab6_FileReader.h"

unique_ptr<DataType> Lab6_FileReader::Read(const std::vector<std::filesystem::path> filePaths)
{
	constexpr int n = 1002; // W konspekcie napisane jest, ze danych jest n = 1001

	ifstream dataFile(filePaths.at(0));

	if (!dataFile.is_open()) throw std::runtime_error("Error while opening file: " + filePaths.at(0).string());

	matrix x(n, 2);

	string line;
	int i = 0;
	while (getline(dataFile, line))
	{
		istringstream iss(line);
		string tempNumber;

		int j = 0;
		while (iss >> tempNumber)
			x(i, j++) = stod(tempNumber);

		i++;
	}

	dataFile.close();

	unique_ptr<Lab6_DataType> finalData = make_unique<Lab6_DataType>();
	finalData->x = x;

	return finalData;
}
