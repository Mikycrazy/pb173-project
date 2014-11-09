#include <QCoreApplication>
#include "NetworkManager.h"
#include "Client.h"



#ifndef UNIT_TEST
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Client* client = new Client("test", "test@test");
    client->login();

    return a.exec();
}
#endif
