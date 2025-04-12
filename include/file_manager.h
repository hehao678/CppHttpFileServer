#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <string>

class FileManager
{
  public:
    //   构造函数，设置文件根目录
    explicit FileManager(const std::string &basePath);
    //读取文件（用于GET请求）
    bool readFile(const std::string &relativePath, std::string &fileContent);

    // 写入文件（用于POST请求）
    bool writeFile(const std::string &relativePath, std::string &fileContent);

  private:
    std::string basePath_; //文件操作根目录
};

#endif // FILE_MANAGER_H