#pragma once
#include <stdlib.h>
#include <iostream>
#include <string>

const std::string IP_CA = "192.168.0.1";

class NetworkManager
{
public:
	NetworkManager();
	~NetworkManager();

	
};

class TcpServer
{
public:
	TcpServer();
	~TcpServer();

private:
	void incomingConnection(int socketDescriptor);

};

class TcpClient
{
public:
	TcpClient();
	~TcpClient();

private:

};



