#include <QCoreApplication>
#include "NetworkManager.h"

#ifdef TEST
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    NetworkManager server;
    //server.startListening(8888);

    return a.exec();
}

#endif
