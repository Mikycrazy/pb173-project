#include <QCoreApplication>
#include "NetworkManager.h"

#ifndef UNIT_TEST
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Server *server = new Server();

    NetworkManager network(server);
    network.startListening(13374);

    return a.exec();
}
#endif
