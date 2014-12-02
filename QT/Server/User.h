#ifndef USER_H
#define USER_H

#include <stdlib.h>
#include <iostream>
#include <string.h>

using namespace std;

class User
{
private:
	string mUsername;
	string mEmail;
	string mIPAddress;
    unsigned char* mPublicKey = nullptr;
	bool mOnline;
    int mConnectionID;

public:
    User() : mUsername(), mEmail(), mIPAddress(), mOnline(false), mConnectionID() {}
    User(string username, string email, string ip, unsigned char* key, int connectionID) :
        mUsername(username), mEmail(email), mIPAddress(ip), mPublicKey(key), mOnline(false), mConnectionID(connectionID) {}
    ~User() { delete[] mPublicKey; }

	void setOnline() { mOnline = true; }
	void setOffline() { mOnline = false; }
	string getUsername() { return mUsername; }
	string getEmail() { return mEmail; }
	string getIPAddress() { return mIPAddress; }
    int getConnectionID() { return mConnectionID; }
	unsigned char* getPublicKey() { return mPublicKey; }
	bool isOnline() { return mOnline; }
};

#endif
