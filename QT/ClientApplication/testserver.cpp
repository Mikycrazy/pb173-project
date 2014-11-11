#include "testserver.h"

TestServer::TestServer(quint16 port)
{
    this->mNetwork = new TestServerNetworkManager();
    connect(this->mNetwork, SIGNAL(receivedData(int,unsigned char*,int)), this, SLOT(processPacket(int,unsigned char*,int)));
    this->mNetwork->startListening(port);


}

void TestServer::processPacket(int connectionID, unsigned char* packet, int size)
{

    std::cout << "Zpracovavaní" << std::endl;

    const int ID_LENGHT = 1;
    const int RANDOM_BYTES_LENGTH = 10;
    const int DATA_SIZE_LENGTH = 4;

    qDebug() << "Received packet from connection" << connectionID;

    int id = 0;
    int dataSize = 0;

    if (size < ID_LENGHT + RANDOM_BYTES_LENGTH + DATA_SIZE_LENGTH)
    {
        qDebug() << "Received packet with invalid size:" << size;
        return;
    }

    //desifrovani zkontrolovani hashu atd. tady bude
    if(sizeof(int) == 4)
    {
        id = packet[0];
        dataSize = ( packet[ID_LENGHT + RANDOM_BYTES_LENGTH + 3] << 24) | ( packet[ ID_LENGHT + RANDOM_BYTES_LENGTH +2] << 16) | (packet[ID_LENGHT + RANDOM_BYTES_LENGTH + 1] << 8) | ( packet[ID_LENGHT + RANDOM_BYTES_LENGTH]);

        if (dataSize + ID_LENGHT + RANDOM_BYTES_LENGTH + DATA_SIZE_LENGTH != size || dataSize < 0)
        {
            qDebug() << "Received packet with invalid data size:" << dataSize << "- total packet size:" << size;
            return;
        }

        mLastData = new unsigned char [dataSize];
        memcpy(mLastData, &packet[ID_LENGHT + RANDOM_BYTES_LENGTH + 4], dataSize);

        std::cout << mLastData << std::endl;

    }
}


