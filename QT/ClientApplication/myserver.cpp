#include "myserver.h"
#include "logger.h"

MyServer::MyServer()
{
    try
    {
        mServer = new QTcpServer(this);
        mData = new QByteArray(10,'a');
    }
    catch(std::bad_alloc& exc)
    {
        Logger::getLogger()->Log(exc.what());
    }


    connect(mServer, SIGNAL(newConnection()), this, SLOT(connection()));

    if(!mServer->listen(QHostAddress::Any, 13374))
    {
        qDebug() << "Server could not start!!";
    }
    else
    {
        qDebug() << "Server started";
    }


}

MyServer::~MyServer()
{
    delete mServer;
    delete mData;
}

void MyServer::Stop()
{
    mServer->close();
}


QByteArray reverse(const QByteArray& array)
{
    QByteArray reverse;
    reverse.reserve(array.size());
    for(int i = array.size(); i >= 0; --i)
        reverse.append(array.at(i));

    return reverse;
}

void MyServer::connection()
{
    qDebug() << "Connection started";

    QTcpSocket* socket = mServer->nextPendingConnection();

    try
    {
        mData = new QByteArray(socket->readAll());
    }
    catch(std::bad_alloc& exc)
    {
        Logger::getLogger()->Log(exc.what());
    }


    if(mData->at(0) == GET_ONLINE_USER_LIST_RESPONSE)
    {
        socket->write((*mData));
    }
    else if(mData->at(0) == LOGIN_RESPONSE)
    {
        socket->write(reverse(*mData));
    }
    else
    {
        socket->write("Fail!");
    }

    socket->flush();
    socket->waitForBytesWritten(3000);

    socket->abort();
}


