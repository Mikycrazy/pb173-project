#include <QCoreApplication>
#include "NetworkManager.h"
#include "Client.h"
#include "Windows.h"
//#include "polarssl/aes.h"

//#define UNIT_TEST
#ifndef UNIT_TEST
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    if(argc == 2)
    {
        if(!strcmp(argv[1], "-l"))
        {
            Client* client2 = new Client("test2", "test2@test2");

            client2->login();
        }
        else if(!strcmp(argv[1], "-c"))
        {
            Client* client2 = new Client("test2", "test2@test2");

            client2->login();

            while(!client2->isLogged())
            {
                Sleep(10);
                qApp->processEvents();
                client2->getOnlineList();
            }
        }
    }


    /*
    unsigned char key[16];
    unsigned char nonce_counter[16] = {0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,250};

    CryptoManager manager;
    manager.startCtrCalculation(key, nonce_counter);

    unsigned char stream[50];

    manager.getEncKeystream(stream, 50);
    for (int i = 0; i < 50; i++) std::cout << (int)(stream[i]) << " ";
    std::cout << endl;

    for (int i = 0; i < 200000; i++)
        manager.getEncKeystream(stream, 50);
    for (int i = 0; i < 50; i++) std::cout << (int)(stream[i]) << " ";
    std::cout << endl;

    manager.getDecKeystream(stream, 50);
    for (int i = 0; i < 50; i++) std::cout << (int)(stream[i]) << " ";
    */

    /*Client* client = new Client("test", "test@test");

    client->login();

    Sleep(500);*/
    Client* client2 = new Client("test2", "test2@test2");

    client2->login();

    while(!client2->isLogged())
    {
        Sleep(10);
        qApp->processEvents();
        client2->getOnlineList();
    }

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
    }
    //Sleep(10);
    //std::cout <<  client->OnlineList().size() << std::endl;
   // int conID = client->OnlineList()[0]->getConnectionID();
   // client->connectToClient(conID);
    //client->logout();
/*
    CryptoManager manager;
    unsigned char key[16] = "0";
    unsigned char iv[16] = { 0xb6, 0x58, 0x9f, 0xc6, 0xab, 0x0d, 0xc8, 0x2c, 0xf1, 0x20, 0x99, 0xd1, 0xc2, 0xd4, 0x0a, 0xb9 };
    unsigned char iv2[16];

    memcpy(iv2, iv, 16);

    for(int i = 0; i < 10; i++)
    {
        unsigned char* data;
        unsigned char* cypher = new unsigned char[16];
        unsigned char data2[] = "Hello World";
        unsigned char* cypher2 = new unsigned char[16];

        memset(cypher2, 0, 16);


        std::string s = manager.PrepairCounterForEncryption(i);
        data = (unsigned char*)s.c_str();

        manager.encryptAES(data, cypher, key, iv, 16);


        manager.XORData(data2, cypher2, 11, cypher);


        char printstr[22] = {00};

        for(int x = 0; x < 11; x++)
            sprintf(printstr+(x*2), "%02x", cypher[x]);
        printstr[21] = '\0';

        std::cout << data2 << "  --  " << printstr << "  --  ";

        manager.XORData(cypher2, data, 11,  cypher);

        std::cout << data << std::endl;




    }
*/
    return a.exec();
}
#endif
