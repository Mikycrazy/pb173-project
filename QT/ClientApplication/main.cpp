#include <QCoreApplication>
#include "NetworkManager.h"
#include "Client.h"
#include "Windows.h"


#ifndef UNIT_TEST
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Client* client = new Client("test", "test@test");

    client->login();
    while(!client->isLogged())
    {
        Sleep(10);
        qApp->processEvents();
    }
    client->logout();

    return a.exec();
}
#endif
