#pragma once

#ifdef ENABLE_SAVING_RESULTS
#define SAVE_TO_FILE(filename) FileSaver(filename)
#else
#define SAVE_TO_FILE(filename) if (false) std::ofstream()
#endif

auto constexpr PRECISION = 5;
auto constexpr SEPARATOR = ';';
auto constexpr RESULTS_FOLDER_PATH = "..\\Results";

class FileSaver
{
public:
	FileSaver(const std::string& filename)
	{
		GenerateFolder();
		m_Ofstream = std::ofstream(s_Folder / filename, std::ios::app);
	}

	~FileSaver()
	{
		m_Ofstream << "\n";
		m_Ofstream.close();
	}

	template <typename T>
	FileSaver& operator<<(const T& data)
	{
		m_Ofstream << std::setprecision(PRECISION) << data << SEPARATOR;
		return *this;
	}
private:
	static void GenerateFolder()
	{
		if (!std::filesystem::exists(RESULTS_FOLDER_PATH)) std::filesystem::create_directory(RESULTS_FOLDER_PATH);

		if (std::filesystem::exists(s_Folder)) return;

		std::filesystem::path workingDir = std::filesystem::current_path();

		std::time_t rawTime;
		struct tm result;
		time(&rawTime);

#ifdef _WIN32
		localtime_s(&result, &rawTime);
#else
		localtime_r(&rawTime, &result);
#endif

		std::ostringstream newFolderName;
		newFolderName << std::put_time(&result, "%Y-%m-%d_%H-%M-%S");

		s_Folder = workingDir / RESULTS_FOLDER_PATH / newFolderName.str();

		std::cout << "Generating folder: " << s_Folder << "\n";

		std::filesystem::create_directory(s_Folder);
	}

private:
	std::ofstream m_Ofstream;
	static std::filesystem::path s_Folder;
};
