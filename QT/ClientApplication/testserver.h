#ifndef TESTSERVER_H
#define TESTSERVER_H

#include "Opcodes.h"
#include "testservernetworkmanager.h"
#include <vector>
#include <QDebug>
#include <time.h>

using namespace std;

class TestServer : QObject
{
    Q_OBJECT

private:
    unsigned char* mPrivateKey;
    unsigned char* mPublicKey;
    TestServerNetworkManager* mNetwork;
    unsigned char* mLastData;

public:

    /**
    * Konstruktor pre triedu Server.
    */
    TestServer(quint16 port);
    ~TestServer() {}

    unsigned char* getData() {return mLastData;}


public slots:

    void processPacket(int connectionID, unsigned char* packet, int size);

private:

    int createPacket(unsigned char id, unsigned char* data, unsigned char **packet, int size);


};

#endif // TESTSERVER_H
