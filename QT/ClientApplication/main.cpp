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
            Sleep(10);
            qApp->processEvents();
            while(true)
            {
                Sleep(10);
                qApp->processEvents();
            }

            delete client2;
        }
        else if(!strcmp(argv[1], "-c"))
        {
            Client* client2 = new Client("test2", "test2@test2",12346);

            client2->login();

            while(!client2->isLogged())
            {
                Sleep(10);
                qApp->processEvents();

            }
            client2->getOnlineList();
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
            client2->setCypherPosition(10);
            client2->sendDataToClient(mReceiverIP, 12345,testData,5);
            client2->setCypherPosition(1);
            client2->sendDataToClient(mReceiverIP, 12345,testData,5);

            delete client2;
        }
        else if(!strcmp(argv[1], "-d"))
        {

        }
    }
    else
    {
      QHostAddress mReceiverIP;
        mReceiverIP= "127.0.0.1";
    std::cout << "Zadej username" << std::endl;
    std::string username, input;
    std::cin >> username;
    Client* client = new Client(username, username + "@test", 12345);
    client->login();

    while(!client->isLogged())
    {
        Sleep(10);
        qApp->processEvents();

    }
    client->getOnlineList();
    while(client->getStatus() != GET_ONLINE_USER_LIST_RESPONSE)
    {
        Sleep(10);
        qApp->processEvents();
    }
    for(int i = 0; i < client->OnlineList().size(); i++)
        std::cout << i <<". "  << client->OnlineList()[i]->getUsername() << std::endl;
    std::cout << "Zadej cislo klienta ke kteremu se chces pripojit" << std::endl;
    int id = 0;
    std::cin >> id;
    client->connectToClient(client->OnlineList()[id]->getConnectionID());
    while(client->getStatus() != SERVER_COMUNICATION_RESPONSE)
    {
        Sleep(10);
        qApp->processEvents();
    }
    std::cout << std::endl << "Zadejte data ktera chcete poslat" << std::endl;
    std::cin >> input;
    client->sendDataToClient(mReceiverIP, 12345,(unsigned char *)input.c_str(),input.size());
    while(client->getStatus() != CLIENT_COMMUNICATION_DATA)
    {
        Sleep(10);
        qApp->processEvents();
    }

    delete client;

    }

    return 0;//a.exec();
}
#endif
