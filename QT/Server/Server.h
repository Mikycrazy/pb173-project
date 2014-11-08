#ifndef SERVER_H
#define SERVER_H

#include <vector>
#include <stdlib.h>
#include "User.h"


using namespace std;

const int ID_LENGHT = 1;
const int RANDOM_BYTES_LENGTH = 10;
const int LOGIN_REQUEST = 1;
const int LOGIN_RESPONSE = 2;
const int LOGOUT_REQUEST = 3;
const int LOGOUT_RESPONSE = 4;
const int GET_ONLINE_USER_LIST_REQUEST = 5;
const int GET_ONLINE_USER_LIST_RESPONSE = 6;

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
    Server() : mPort(8888) {}
    Server(int port) : mPort(port) {}
    ~Server() {}

	/**
	* Bindne server na port zadany v konstruktori a zacne spracovavat prichadzajuce spojenia
	*
	* @return	true ak sa server podarilo spustit, inak false
	*/
	bool start();

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

    /**
    * Zpracuje prisly packet, vyparsuje data a ulozi je do parametru data
    *
    * @param packet		prisli packet
    * @param data		sem se ulozi data obsazena v paketu
    *
    * @return vrati velikost dat
    */
    int processPacket(unsigned char* packet, unsigned char** data);

    /**
    * Zpracuje data a id pozadavku do podoby paketu k odeslani po siti
    *
    * @param id         id paketu, jestli jde o login/logout nebo neco dalsiho
    * @param data		data
    * @param packet		sem se ulozi packet, ktery je pripraven k odeslani
    * @param size		delka dat
    */

    int createPacket(unsigned char id, unsigned char* data, unsigned char **packet, int size);
};

#endif
