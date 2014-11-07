#include <QCoreApplication>
#include "NetworkManager.h"
#include "Client.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
   /* NetworkManager client;
    //unsigned char* l;
    client.startConnection("127.0.0.1", 8888);
    client.sendData(1,"ahoj",4);*/
    Client c("pepa", "pepa@seznam.cz");
    c.login();
   /* unsigned char data[5];
    unsigned char *packet = NULL;
    for(int i = 0; i < 5; i++)
        data[i] = 'a';
    int packetSize = c.packetCreator(1,data,&packet,5);

    for(int i = 0; i < packetSize; i++)
       printf("%c",packet[i]);*/

    return 0;
}
