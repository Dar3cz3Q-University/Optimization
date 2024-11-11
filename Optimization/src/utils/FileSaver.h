#pragma once

auto constexpr PRECISION = 5;
auto constexpr RESULTS_FOLDER_PATH = "..\\Results";

#ifdef ENABLE_SAVING_RESULTS
	#define SAVE_TO_FILE(filename) (FileSaver::GetInstance().OpenFile(filename), FileSaver::GetInstance().GetOfStream(filename)) << std::setprecision(PRECISION)
#else
	#define SAVE_TO_FILE(filename) if (false) std::ofstream()
#endif

class FileSaver
{
public:
	static FileSaver& GetInstance()
	{
		static FileSaver s_Instance;
		return s_Instance;
	}

	void OpenFile(const std::string& filename)
	{
		if (m_Ofstreams.find(filename) == m_Ofstreams.end())
		{
			GenerateFolder();
			std::ofstream& ofs = m_Ofstreams[filename];
			ofs.open(s_Folder / filename, std::ios::app);
		}
	}

	std::ofstream& GetOfStream(const std::string& filename)
	{
		return m_Ofstreams.at(filename);
	}

	~FileSaver()
	{
		for (auto& [_, ofs] : m_Ofstreams)
		{
			if (ofs.is_open())
			{
				ofs << "\n";
				ofs.close();
			}
		}
	}

private:
	FileSaver() = default;
	FileSaver(const FileSaver&) = delete;
	FileSaver& operator=(const FileSaver&) = delete;

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
	std::unordered_map<std::string, std::ofstream> m_Ofstreams;
	static std::filesystem::path s_Folder;
};
