#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <thread>
#include <vector>

class HttpServer
{
public:
	HttpServer(int port);
	void Start();
private:
	void NewClient(SOCKET Client);


	sockaddr_in sin{};
	SOCKET slisten{};
	std::vector<std::thread> threads;
};
