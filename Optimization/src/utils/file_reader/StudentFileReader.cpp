#include "pch.h"
#include "StudentFileReader.h"

solution StudentFileReader::Read(const vector<filesystem::path> filePaths)
{
	ifstream xData(filePaths.at(0));

	if (!xData.is_open()) throw std::runtime_error("Error while opening file");

	string line;
	while (getline(xData, line))
	{
		istringstream iss(line);
		string tempNumber;

		while (iss >> tempNumber)
		{
			cout << tempNumber << "\n";
		}
	}

	return solution();
}
