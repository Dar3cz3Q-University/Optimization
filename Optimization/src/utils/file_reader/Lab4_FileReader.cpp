#include "pch.h"
#include "Lab4_FileReader.h"

unique_ptr<DataType> Lab4_FileReader::Read(const vector<filesystem::path> filePaths)
{
	constexpr int m = 100; // W konspekcie napisane jest, ze danych jest m = 100

	// Read X data

	ifstream xDataFile(filePaths.at(0));

	if (!xDataFile.is_open()) throw std::runtime_error("Error while opening file: " + filePaths.at(0).string());

	matrix x(m, 3);

	string line;
	int i = 0;
	while (getline(xDataFile, line))
	{
		istringstream iss(line);
		string tempNumber;

		int j = 0;
		while (iss >> tempNumber)
			x(j++, i) = stoi(tempNumber);

		i++;
	}

	xDataFile.close();

	// Read Y data

	ifstream yDataFile(filePaths.at(1));

	if (!yDataFile.is_open()) throw std::runtime_error("Error while opening file: " + filePaths.at(1).string());

	matrix y(m, 1);

	string result;
	i = 0;
	while (yDataFile >> result)
		y(i++) = stoi(result);

	yDataFile.close();

	unique_ptr<Lab4_DataType> finalData = make_unique<Lab4_DataType>();
	finalData->x = x;
	finalData->y = y;

	return finalData;
}
