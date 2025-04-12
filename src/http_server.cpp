#include "http_server.h"
#include "log.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <sstream>

HttpServer::HttpServer(const std::string &ip, int port) : server_ip(ip), server_port(port), server_fd(-1) {}

HttpServer::~HttpServer()
{
    if (server_fd != -1)
    {
        close(server_fd);
    }
}

void HttpServer::start()
{
    // 创建套接字;基于TCP
    server_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_fd == -1)
    {
        LOG_ERROR("Failed to create socket!");
        return;
    }
    sockaddr_in server_addr{};
    server_addr.sin_family      = AF_INET;
    server_addr.sin_port        = htons(server_port);
    server_addr.sin_addr.s_addr = inet_addr(server_ip.c_str());
    // 绑定
    if (bind(server_fd, (sockaddr *) &server_addr, sizeof(server_addr)) < 0)
    {
        LOG_ERROR("Bind error!");
        return;
    }
    // 监听
    if (listen(server_fd, 10) < 0)
    {
        LOG_ERROR("Listen failed!");
        return;
    }

    std::ostringstream oss;
    oss << "Server started at " << server_ip << ":" << server_port;
    LOG_INFO(oss.str());

    while (true)
    {
        sockaddr_in client_addr{};
        socklen_t   client_len = sizeof(client_addr);

        int client_fd = accept(server_fd, (sockaddr *) &client_addr, &client_len);
        if (client_fd < 0)
        {
            LOG_WARNING("Accept client failed!");
            continue;
        }

        handleClient(client_fd);
        close(client_fd);
    }
}

void HttpServer::handleClient(int client_fd)
{
    std::string method, path;
    parseRequest(client_fd, method, path);

    if (method == "GET")
    {
        handleGet(client_fd, path);
    }
    else if (method == "POST")
    {
        handlePost(client_fd, path);
    }
    else
    {
        std::string response = "HTTP/1.1 400 Bad Request\r\n\r\nUnsupported Method";
        send(client_fd, response.c_str(), response.size(), 0);
    }
}

void HttpServer::parseRequest(int client_fd, std::string &method, std::string &path)
{
    char buffer[1024] = {0};
    recv(client_fd, buffer, sizeof(buffer), 0);

    std::istringstream iss(buffer);
    iss >> method >> path;

    std::ostringstream oss;
    oss << "Request Method: " << method << ",Path: " << path;
    LOG_INFO(oss.str());
}

void HttpServer::handleGet(int client_fd, const std::string &path)
{
    std::string response = "HTTP/1.1 200 OK\r\n\r\nGET request received: " + path;
    send(client_fd, response.c_str(), response.size(), 0);
}

void HttpServer::handlePost(int client_fd, const std::string &path)
{
    std::string response = "HTTP/1.1 200 OK\r\n\r\nPOST request received: " + path;
    send(client_fd, response.c_str(), response.size(), 0);
}