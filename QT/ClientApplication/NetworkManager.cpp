#include "NetworkManager.h"

int NetworkManager::startConnection(QString ipAddress, quint16 port)
{
   // connect(&mTcpClient, SIGNAL(connected()),this, SLOT(startTransfer()));
    mTcpClient.connectToHost(mIPAddress, mPort);
    return 0;
}

bool NetworkManager::sendData(int connectionID, unsigned char *data, int size)
{
    bool b = false;
    mTcpClient.write(reinterpret_cast<const char*>(data),size);

    return b;
}
void NetworkManager::startListening( quint16 port)
{

    mTcpServer.listen(QHostAddress::Any, port);
}
int NetworkManager::acceptConnection()
{
    mPTcpClient = mTcpServer.nextPendingConnection();
    return 0;
}

int NetworkManager::receiveData(int connectionID, unsigned char *data)
{
    int sizeData = 0;
    return sizeData;
}
