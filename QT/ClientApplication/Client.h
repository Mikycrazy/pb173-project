#pragma once
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include "CryptoManager.h"
#include "NetworkManager.h"


const int ID_LENGHT = 1;
const int RANDOM_BYTES_LENGTH = 10;
const int LOGIN_REQUEST = 1;
const int LOGIN_RESPONSE = 2;
const int LOGOUT_REQUEST = 3;
const int LOGOUT_RESPONSE = 4;

class Client
{
private:
	std::string mUsername;
	std::string mEmail;

	unsigned char* mAESkey;
	unsigned char* mPublicRSAKey;
	unsigned char* mPrivateRSAKey;

	unsigned char* mCertificate;

	bool mLoggedToServer;
	bool mConnectedToClient;

    QString mReceiverIP;
	int mReceiverPort;

public:
	/**
	* Konstruktor pro tridu Client.
	* Nastavi username a email novemu clientovi.
	*
	* @param username		uzivatelke jmeno
	* @param email			email
	*/
    Client();
	Client(std::string username, std::string email);
	~Client();

	std::string Username() const { return mUsername; }
	void Username(std::string username)	{ mUsername = username; }

	std::string Email() const	{ return mEmail; }
	void Email(std::string email) { mEmail = email; }

	/**
	* Vygenerovani novych rsa klicu.
	*/
	int generateNewRSAKeys();
	/**
	* Vygenerovani noveho AES klice.
	*/
	int generateNewAESKey();
	/**
	* Odesle pozadavek na prihlaseni na server.
	*/
	int login();
	/**
	* Odesle pozadavek na odhlaseni na server.
	*/
	int logout();

	/**
	* Odesle pozadavek na vydani certifikatu uzivatele, na certifikacni autoritu
	*
	* @param username		uzivatelske jmeno 
	*/
	int getCertificat(std::string username);
	/**
	* Odesle pozadavek k registraci s username a email na certifikacni autoritu.
	*/
	int registerToCA();

	/**
	* Odesle pozadavek ke komunikaci ke klientovi
	*
	* @param username		uzivatelske jmeno 
	*/
	int connectToClient(std::string username);

	/**
	* Zahaji komunikaci s klientem
	*
	* @param data		data
	*/
	int startCommunication(unsigned char* data);

    /**
    * Zpracuje prisly packet, a data necha v paketu
    *
    * @param packet		prisli packet
    * @param data		sem se ulozi data obsazena v paketu
    *
    * @return vrati id paketu
    */
    int processData(unsigned char* packet, unsigned char** data, int *dataSize);

    /**
    * Zpracuje data a id pozadavku do podoby paketu k odeslani po siti
    *
    * @param id         id paketu, jestli jde o login/logout nebo neco dalsiho
    * @param data		data
    * @param packet		sem se ulozi packet, ktery je pripraven k odeslani
    * @param size		delka dat
    */

    int packetCreator(unsigned char id, unsigned char* data, unsigned char **packet, int size);

private:



};

