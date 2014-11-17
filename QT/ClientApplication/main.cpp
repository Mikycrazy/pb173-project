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

    Client* client = new Client("test", "test@test");

    //client->login();

    unsigned char key[64];
    client->getOnlineList();
    //client->acceptConnection(1,key);

    QHostAddress address("127.0.0.1");
    unsigned char data[5] = {'a', 'b', 'c', 'd', 'e'};
    client->sendDataToClient(address, 13375, data, 5);

    /*
    while(!client->isLogged())
    {
        Sleep(10);
        qApp->processEvents();
    }
    client->logout();*/

    return a.exec();
}
#endif
