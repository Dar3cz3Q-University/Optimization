#include "pch.h"
#include "FileReaderFactory.h"

FileReaderFactory::FileReaderFactory()
{
    m_FactoryMap[FileTypeEnum::Lab4] = []() { return std::make_unique<Lab4_FileReader>(); };
}

std::unique_ptr<FileReader> FileReaderFactory::CreateFileReader(FileTypeEnum fileType)
{
    auto it = m_FactoryMap.find(fileType);

    if (it != m_FactoryMap.end())
    {
        return it->second();
    }

    throw std::invalid_argument("Unsupported file type");
}
