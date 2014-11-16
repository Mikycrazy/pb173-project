#include "mytcpclient.h"

bool MyTcpClient::startConnection(QString ipAddress, quint16 port)
{
    this->mSocket = new QTcpSocket();

    connect(mSocket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(mSocket, SIGNAL(readyRead()), this, SLOT(receiveData()));

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

bool MyTcpClient::sendData(unsigned char *data, int size)
{
    qDebug() << "Sending data of size" << size;

    mSocket->write((const char*)data, size);
    mSocket->flush();

    return mSocket->waitForBytesWritten(10000);
}

void MyTcpClient::disconnected()
{
    qDebug() << "Server disconnected!";
}

void MyTcpClient::receiveData()
{
    const int ID_LENGHT = 1;
    const int RANDOM_BYTES_LENGTH = 10;
    const int DATA_SIZE_LENGTH = 4;
    const std::string DATA_SPLITER = ";";

    QByteArray data = mSocket->readAll();
    qDebug() << "Received data from server:" << data.toHex();

    int id = 0;
    int dataSize = 0;

    if(sizeof(int) == 4)
    {
        id = data[0];
        dataSize = ( data[ID_LENGHT + RANDOM_BYTES_LENGTH + 3] << 24) | ( data[ ID_LENGHT + RANDOM_BYTES_LENGTH +2] << 16) | (data[ID_LENGHT + RANDOM_BYTES_LENGTH + 1] << 8) | ( data[ID_LENGHT + RANDOM_BYTES_LENGTH]);
        mLastReicevedData = new unsigned char [dataSize];
        memcpy(mLastReicevedData, &data[ID_LENGHT + RANDOM_BYTES_LENGTH + 4], dataSize);
    }
}
