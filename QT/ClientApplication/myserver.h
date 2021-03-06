#ifndef MYSERVER_H
#define MYSERVER_H

#include <QObject>
#include <QDebug>
#include <QTcpServer>
#include <QTcpSocket>
#include "Opcodes.h"

class MyServer : public QObject
{
    Q_OBJECT
public:
    MyServer();
    ~MyServer();
    QByteArray getLastData() {return *mData;}
    void Stop();
signals:

public slots:
    void connection();
private:
    QTcpServer* mServer;
    QByteArray* mData;
};

#endif // MYSERVER_H
