#pragma once
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include "CryptoManager.h"
#include "NetworkManager.h"

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

	std::string mReceiverIP;
	int mReceiverPort;

public:
	/**
	* Konstruktor pro tridu Client.
	* Nastavi username a email novemu clientovi.
	*
	* @param username		uzivatelke jmeno
	* @param email			email
	*/
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

private:
	/**
	* Zpracuje prisla data
	*
	* @param data		data
	*/
	int processData(unsigned char* data);
};

