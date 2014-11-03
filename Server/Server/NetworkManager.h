#ifndef NETWORK_MANAGER_H
#define NETWORK_MANAGER_H

#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <vector>

using namespace std;

class NetworkManager
{
private:
	string mIPAddress;
	int mPort;
	vector<int> connections;

public:

	/**
	* Konstruktor pre triedu NetworkManager.
	* Nastavi ip a port na ktorom bude aplikacia ocakavat prichadzajuce spojenia
	*
	* @param address	server ip
	* @param port		server port
	*/
	NetworkManager(string address, int port) : mIPAddress(address), mPort(port) {}

	/**
	* Prijme prichadzajuce spojenie, prida ho do zoznamu spojeni a vrati jeho ID
	*
	* @return	ID spojenia
	*/
	int acceptConnection();

	/**
	* Odosle data zadanemu spojeniu
	*
	* @param connectionID	id spojenia
	* @param data			data na odoslanie
	* @param size			velkost dat na odoslanie
	* @return				true ak sa odoslanie podarilo, inak false
	*/
	bool sendData(int connectionID, unsigned char* data, int size);

	/**
	* Prijme data od vybraneho spojenia
	*
	* @param connectionID	id spojenia
	* @param data			prijate data
	* @return				velkost prijatych dat
	*/
	int receiveData(int connectionID, unsigned char* data);
}

#endif