#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H
#include <string>

#include "file_manager.h"

class HttpServer
{
  public:
    HttpServer(const std::string &ip, int port);
    ~HttpServer();

    // 启动服务器，主循环
    void start();

  private:
    int         server_fd;   //监听socket文件描述符
    std::string server_ip;   //服务器ip
    int         server_port; //服务器端口

    FileManager fileManager_; //文件管理模块实例

    // 处理客户端请求的入口
    void handleClient(int client_fd);

    // 简单的HTTP请求协议（只解析方法和路径）
    // void parseRequest(int client_fd, std::string &method, std::string &path);
    // void parseRequest(int client_fd, std::string &method, std::string &path, int &contentLength);
    void parseRequest(int client_fd, std::string &method, std::string &path, int &contentLength, std::string &bodyData);

    //处理GET请求
    void handleGet(int client_fd, const std::string &path);
    // 处理POST请求
    // void handlePost(int client_fd, const std::string &path);
    // void handlePost(int client_fd, const std::string &path, int contentLength
    void handlePost(int client_fd, const std::string &path, int contentLength, const std::string &bodyData);
};

#endif // HTTP_SERVER_H