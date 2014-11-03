#pragma once

#include <string>
const int AES_KEY_LENGTH = 128;
const int RSA_KEY_LENGTH = 1024;

class CA
{
	std::string mPublicRSAKey;
	std::string mPrivateRSAKey;
private:
public:
	CA::CA()
	{
	}

	CA::~CA()
	{
	}
	int VerifyCertificate(std::string certificate, std::string userName);

	int AddNewCaUser(std::string publicKey, std::string userName, std::string email);

	int AsymetricCypher(std::string data, std::string key);

	int AsymetricDecypher(std::string data, std::string key);

private:

	int send_data(std::string IP, std::string port, std::string data);
};

class Client
{
private:
	std::string mUsername;
	std::string mEmail;

	unsigned char* mAESkey;
	unsigned char* mPublicRSAKey;
	unsigned char* mPrivateRSAKey;

	unsigned char* mCertificate;

	bool mLogged;

public:
	Client(const char* username, const char* email);
	~Client();

	int register_to_CA();

	int generate_new_RSA_keys();
	int generate_new_AES_key();

	int login();
	int logout();

	int get_certificat(const char* username);

	int connect_to_client(const char* username);

private:

	int send_certificat_to_CA();

	int send_request_to_communication(const char* IP, const char* port);

	int send_data(const char* IP, const char* port, unsigned char* data);


};

