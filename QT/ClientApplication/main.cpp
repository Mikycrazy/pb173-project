#include <QCoreApplication>
#include "NetworkManager.h"
#include "Client.h"
#include "Windows.h"

//#define UNIT_TEST
#ifndef UNIT_TEST
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    if(argc == 2)
    {
        QHostAddress mReceiverIP;
        mReceiverIP= "127.0.0.1";
        unsigned char testData[5];
        for(int i = 0; i < 5; i++)
            testData[i] = 'a';

        if(!strcmp(argv[1], "-l"))
        {
            Client* client2 = new Client("test1", "test1@test1",12345);

            client2->login();
        }
        else if(!strcmp(argv[1], "-c"))
        {
            Client* client2 = new Client("test2", "test2@test2",12346);

            client2->login();

            while(!client2->isLogged())
            {
                Sleep(10);
                qApp->processEvents();
                client2->getOnlineList();
            }
            while(client2->getStatus() != GET_ONLINE_USER_LIST_RESPONSE)
            {
                Sleep(10);
                qApp->processEvents();
            }
            client2->connectToClient(client2->OnlineList()[0]->getConnectionID());
            while(client2->getStatus() != SERVER_COMUNICATION_RESPONSE)
            {
                Sleep(10);
                qApp->processEvents();
            }
            client2->sendDataToClient(mReceiverIP, 12345,testData,5);
        }
        else if(!strcmp(argv[1], "-d"))
        {
            Client c;

            const int DATA_LENGTH = 10;
            unsigned char* data = new unsigned char[DATA_LENGTH];
            unsigned char* packet = NULL;
            unsigned char* data2 = NULL;

            memset(data, 97, DATA_LENGTH);

            int size = c.createPacket(LOGIN_REQUEST, data, &packet, DATA_LENGTH);
            int size2 = c.processPacket(packet, &data2, size);
        }
    }
    else
    {

    Client* client = new Client("test", "test@test", 12345);

    client->login();

    Sleep(500);
   /* Client* client2 = new Client("test2", "test2@test2", 12346);

    client2->login();

    while(!client2->isLogged())
    {
    Sleep(100);
    qApp->processEvents();
    client2->getOnlineList();
    }
    bool b = true;
    while(client2->isLogged())
    {
    Sleep(100);
    qApp->processEvents();

    }*/
   // int conID = client2->OnlineList()[0]->getConnectionID();
   // client2->connectToClient(conID);

    //Client* client = new Client("test", "test@test");

    //client->login();

    //unsigned char key[64];


    //QHostAddress address("127.0.0.1");
    //unsigned char data[5] = {'a', 'b', 'c', 'd', 'e'};
    //client->sendDataToClient(address, 13375, data, 5);


    /*while(!client->isLogged())
    {
        Sleep(10);
        qApp->processEvents();
        client->getOnlineList();
    }*/
    //Sleep(10);
    //std::cout <<  client->OnlineList().size() << std::endl;
   // int conID = client->OnlineList()[0]->getConnectionID();
   // client->connectToClient(conID);
    //client->logout();

    }
    return a.exec();
}
#endif
