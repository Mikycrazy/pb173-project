#pragma once
#include <stdlib.h>
#include <iostream>
#include <string.h>

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
	Client(std::string username, std::string email);
	~Client();

	std::string Username() const { return mUsername; }
	void Username(std::string username)	{ mUsername = username; }

	std::string Email() const	{ return mEmail; }
	void Email(std::string email) { mEmail = email; }


	int generateNewRSAKeys();
	int generateNewAESKey();

	int login();
	int logout();

	int getCertificat(std::string username);
	int registerToCA();

	int connectToClient(std::string username);
	int startCommunication(unsigned char* data);

private:
	int processData(unsigned char* data);
	int sendData(std::string IP, std::string port, unsigned char* data);
};

