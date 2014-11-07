#ifndef SERVER_H
#define SERVER_H

#include "User.h"
#include <vector>

using namespace std;

class Server
{
private:
	int mPort;
	vector<User*> mUsers;
	unsigned char* mPrivateKey;
	unsigned char* mPublicKey;

public:

	/**
	* Konstruktor pre triedu Server.
	* Nastavi port na ktorom bude server pocuvat prichadzajuce spojenia
	*
	* @param port	server port
	*/
	Server(const int port) : mPort(port) {}
	~Server();

	/**
	* Bindne server na port zadany v konstruktori a zacne spracovavat prichadzajuce spojenia
	*
	* @return	true ak sa server podarilo spustit, inak false
	*/
	bool start();

private:

	/**
	* Prihlasenie pouzivatela
	*
	* @param user	ukazatel na objekt uzivatela ktory ma byt prihlaseny
	*/
	bool loginUser(User* user);

	/**
	* Overenie uzivatela u certifikacnej autority
	*
	* @param user	ukazatel na objekt uzivatela ktory ma byt overeny
	*/
	bool verifyUser(User* user);

	/**
	* Odhlasenie pouzivatela
	*
	* @param user	ukazatel na objekt uzivatela ktory ma byt odhlaseny
	*/
	bool logoutUser(User* user);

	/**
	* Odoslanie zoznamu online uzivatelov vybranemu pouzivatelovy.
	*
	* @param user	ukazatel na objekt uzivatela ktory ma byt zaslany zozam online uzivatelov
	*/
	void sendOnlineList(User* user);

	/**
	* Odoslanie ziadosti o nadviazanie spojenia medzi dvoma uzivatelmi
	*
	* @param from	ukazatel na objekt uzivatela ktory ziada o vytvorenie spojenia
	* @param to		ukazatel na objekt uzivatela ktoremu je zaslana ziadost
	*/
	bool sendConnectionRequest(User* from, User* to);
};

#endif