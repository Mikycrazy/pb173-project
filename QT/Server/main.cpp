#include <QCoreApplication>
#include "Server.h"

#ifndef UNIT_TEST
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Server *server = new Server(13374);

    return a.exec();
}
#endif
