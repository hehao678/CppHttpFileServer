#include "file_manager.h"
#include "log.h"

#include <fstream>
#include <sstream>
#include <filesystem>

FileManager::FileManager(const std::string &basePath) : basePath_(basePath)
{
    // 确保目录存在
    std::filesystem::create_directories(basePath_);
}

bool FileManager::readFile(const std::string &relativePath, std::string &fileContent)
{
    // 路径安全检查
    if (relativePath.find("..") != std::string::npos)
    {
        LOG_WARNING("Illegal path detected in readFile.");
        return false;
    }

    std::string   fullPath = basePath_ + relativePath;
    std::ifstream ifs(fullPath, std::ios::binary);

    if (!ifs.is_open())
    {
        std::ostringstream oss;
        oss << "Failed to open file for reading: " << fullPath;
        LOG_WARNING(oss.str());
        return false;
    }

    std::ostringstream oss;
    oss << ifs.rdbuf();
    fileContent = oss.str();

    LOG_INFO("Read file success: " + fullPath);
    return true;
}

bool FileManager::writeFile(const std::string &relativePath, std::string &fileContent)
{
    // 路径安全检查
    if (relativePath.find("..") != std::string::npos)
    {
        LOG_WARNING("Illegal path detected in writeFile.");
        return false;
    }

    std::string   fullPath = basePath_ + relativePath;
    std::ofstream ofs(fullPath, std::ios::binary);

    if (!ofs.is_open())
    {
        std::ostringstream oss;
        oss << "Failed to open file for writing: " << fullPath;
        LOG_WARNING(oss.str());
        return false;
    }

    ofs.write(fileContent.c_str(), fileContent.size());
    ofs.close();

    LOG_INFO("Write file success: " + fullPath);
    return true;
}