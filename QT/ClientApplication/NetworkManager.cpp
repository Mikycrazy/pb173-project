#include "NetworkManager.h"

NetworkManager::NetworkManager()
{
    try
    {
        this->mSocket = new QTcpSocket();
        this->mUdpSocket = new QUdpSocket();
    }
    catch(std::bad_alloc& exc)
    {
        qDebug() << exc.what();
    }

}

NetworkManager::~NetworkManager()
{
    delete mSocket;
    delete mUdpSocket;
}

bool NetworkManager::startConnection(QString ipAddress, quint16 port, quint16 UDPport)
{
    this->mUdpSocket->bind(QHostAddress::Any, UDPport);

    connect(mSocket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(mSocket, SIGNAL(readyRead()), this, SLOT(receiveData()));
    connect(mUdpSocket, SIGNAL(readyRead()), this, SLOT(receiveUdpData()));

    qDebug() << "Connecting to" << ipAddress << "at port" << port;

    mSocket->connectToHost(ipAddress, port);
    if (!mSocket->waitForConnected(10000))
    {
        qDebug() << "Error: " << mSocket->errorString();
        return false;
    }
    else
    {
        qDebug() << "Connected!";
        return true;
    }
}

bool NetworkManager::sendData(unsigned char *data, int size)
{
    qDebug() << "Sending data of size" << size;

    mSocket->write((const char*)data, size);
    mSocket->flush();

    return mSocket->waitForBytesWritten(10000);
}

void NetworkManager::disconnected()
{
    qDebug() << "Server disconnected!";
}

void NetworkManager::receiveData()
{
    QByteArray data = mSocket->readAll();
    //qDebug() << "Received data from server:" << data.toHex();

    emit this->networkReceivedData((unsigned char*)data.data(), data.length());
}

void NetworkManager::receiveUdpData()
{
    QByteArray data;
    QHostAddress sender;
    quint16 port;

    data.resize(mUdpSocket->pendingDatagramSize());
    mUdpSocket->readDatagram(data.data(), data.size(), &sender, &port);

    qDebug() << "Received UDP data from IP" << sender.toString() << "port" << port;

    emit this->networkReceivedData((unsigned char*)data.data(), data.length());
}

bool NetworkManager::sendUdpData(QHostAddress receiver, quint16 port, unsigned char* data, int size)
{
    qDebug() << "Sending data of size" << size << "to address" << receiver.toString();
    int i = mUdpSocket->writeDatagram((const char*)data, size, receiver, port);

    if(i == -1)
        return false;
    else
        return true;
}
