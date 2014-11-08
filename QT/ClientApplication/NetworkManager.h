#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <vector>
#include <QtNetwork>
#include <QObject>
#include <QString>
#include <QTcpSocket>
#include <QTcpServer>

using namespace std;

class NetworkManager
{
private:
    QString  mIPAddress;
    quint16  mPort;
    vector<int> connections;
    QTcpSocket mTcpClient;
    QTcpSocket* mPTcpClient;
    QTcpServer mTcpServer;


public:

    /**
    * Konstruktor pre triedu NetworkManager.
    * Nastavi ip a port na ktorom bude aplikacia ocakavat prichadzajuce spojenia
    *
    * @param address	server ip
    * @param port		server port
    */
    NetworkManager(QString address, quint16 port) : mIPAddress(address), mPort(port) {}
    NetworkManager(){}
    ~NetworkManager(){}

    /**
    * Prijme prichadzajuce spojenie, prida ho do zoznamu spojeni a vrati jeho ID
    *
    * @return        ID spojenia
    * @param port    listening port
    */
    int acceptConnection();


    /**
    * Zacne poslouchat na portu 8888
    *
    */
    void startListening(quint16 port);


    /**
    * Zahaji spojeni tcp spojeni s nastavenou adresou a portem
    *
    * @param address	server ip
    * @param port		server port
    * @return	ID spojenia
    */
    int startConnection(QString ipAddress, quint16 port);


    /**
    * Odosle data zadanemu spojeniu
    *
    * @param connectionID	id spojenia
    * @param data			data na odoslanie
    * @param size			velkost dat na odoslanie
    *
    * @return				true ak sa odoslanie podarilo, inak false
    */
    bool sendData(int connectionID, const char* data, int size);

    /**
    * Prijme data od vybraneho spojenia
    *
    * @param connectionID	id spojenia
    * @param data			prijate data
    *
    * @return				velkost prijatych dat
    */
    int receiveData(int connectionID, unsigned char* data);
};

#endif
