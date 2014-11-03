#pragma once

#include <string>

class CaUser
{
	std::string mUserName;
	std::string mEmail;
	unsigned char* mPublicKey;
public:
	CaUser();
	~CaUser();
	std::string UserName() const { return mUserName; }
	void UserName(std::string username)	{ mUserName = username; }

	std::string Email() const	{ return mEmail; }
	void Email(std::string email) { mEmail = email; }

	unsigned char* PublicKey() const	{ return mPublicKey; }
	void PublicKey(unsigned char* publicKey) { mPublicKey = publicKey; }
private:

};

