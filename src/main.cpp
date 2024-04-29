#include <iostream>
#include "HttpServer.h"

int main()
{
	HttpServer server{ 8888 };
	server.Start();
	return 0;
}