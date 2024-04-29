#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <thread>
#include <vector>
#include <sstream>
#include <fstream>
#include "HttpServer.h"
#pragma comment(lib,"ws2_32.lib")

HttpServer::HttpServer(int port)
{
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA wsaData;
	if (WSAStartup(sockVersion, &wsaData) != 0)
	{
		exit(0);
	}
	slisten = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	sin.sin_addr.S_un.S_addr = INADDR_ANY;
	if (bind(slisten, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)
	{
		printf("bind error !");
	}
}

void HttpServer::Start()
{
	if (listen(slisten, 5) == SOCKET_ERROR)
	{
		printf("listen error !");
	}
	sockaddr_in remoteAddr{};
	int nAddrlen = sizeof(remoteAddr);
	char ipstr[INET_ADDRSTRLEN];
	std::cout << "等待链接........" << std::endl;

	while (true)
	{
		SOCKET sClient = accept(slisten, (SOCKADDR*)&remoteAddr, &nAddrlen);
		inet_ntop(AF_INET, &(remoteAddr.sin_addr), ipstr, INET_ADDRSTRLEN);
		std::cout << "ip:" << ipstr << std::endl;
		threads.push_back(std::thread(&HttpServer::NewClient, this, sClient));
	}
}

void HttpServer::NewClient(SOCKET Client)
{
	char ClientData[1024]{};
	recv(Client, ClientData, 255, 0);
	std::istringstream iss(ClientData);
	std::string cmd{}, url{};
	iss >> cmd >> url;
	if (cmd == "GET")
	{
		if (url == "/")
		{
			std::ifstream file("./test.html");
			std::stringstream buffer{};
			buffer << file.rdbuf();
			std::string htmlContent = buffer.str();
			std::string response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n" + htmlContent + "\r\n\r\n";
			send(Client, response.c_str(), response.size(), 0);
		}
		else
		{
			std::ifstream file(url);
			std::stringstream buffer{};
			buffer << file.rdbuf();
			std::string htmlContent = buffer.str();
			std::string response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n" + htmlContent + "\r\n\r\n";
			send(Client, response.c_str(), response.size(), 0);
		}
	}

	shutdown(Client, SD_SEND);
	closesocket(Client);
}