#pragma once

#include <string>
#include <list>
#include "CaUser.h"


class CA
{
	unsigned char* mPublicRSAKey;
	unsigned char* mPrivateRSAKey;
	std::list<CaUser> mUsers;
	
private:
public:
	CA::CA();
	CA::~CA();
	/**
	* Overi platnost certifikatu daneho uzivatele
	* 
	*
	* @param username		uzivatelke jmeno
	* @param email			email
	* @param certificate	certifikat
	*/
	int verifyCertificate(unsigned char* certificate, std::string userName, int size);

	/**
	* Prida noveho uzivatele do CA s jeho public klicem
	*
	*/
	int addNewCaUser(unsigned char* publicKey, std::string userName, std::string email);

	/**
	* Zneplatni certifikat daneho uzivatele
	*
	*/
	int revokeCertificate(unsigned char* certificate, std::string userName);

	/**
	* Zmeni uzivateli certifikat
	*
	*/
	int updateCertificate(unsigned char* certificate, std::string userName);

private:

	int send_data(std::string IP, int port, unsigned char* data);

	int findUser(std::string userName);
	
};


