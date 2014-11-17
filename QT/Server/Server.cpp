#include "Server.h"

Server::Server(quint16 port)
{
    srand(time(NULL));

    this->mNetwork = new NetworkManager();
    connect(this->mNetwork, SIGNAL(receivedData(int,unsigned char*,int)), this, SLOT(processPacket(int,unsigned char*,int)));
    this->mNetwork->startListening(port);
}

Server::Server()
{

}

bool Server::loginUser(User* user)
{
    user->setOnline();
    mUsers.push_back(user);

    Logger::getLogger()->Log("Logged user" + user->getUsername());
    qDebug() << "Logged user" << (user->getUsername().c_str());

    unsigned char *data = NULL;
    int dataSize = user->getUsername().size();
    data = new unsigned char [dataSize];

    memcpy(data, user->getUsername().c_str(), user->getUsername().size());

    unsigned char *packet = NULL;
    int packetSize = this->createPacket(LOGIN_RESPONSE,data,&packet,dataSize);

    //bude nasledovat sifrovani a poslani pres sit
    this->mNetwork->sendData(user->getConnectionID(), packet, packetSize);
    delete[] data;

    return true;
}

bool Server::logoutUser(User* user)
{
    for(unsigned int i = 0; i < mUsers.size(); i++)
    {
         if(user->getConnectionID() == mUsers[i]->getConnectionID())
         {
             mUsers.erase(mUsers.begin() + i);

             unsigned char *data = NULL;
             int dataSize = user->getUsername().size();
             data = new unsigned char [dataSize];

             memcpy(data, user->getUsername().c_str(), user->getUsername().size());

             unsigned char *packet = NULL;
             int packetSize = this->createPacket(LOGOUT_RESPONSE,data,&packet,dataSize);

             //bude nasledovat sifrovani a poslani pres sit
             this->mNetwork->sendData(user->getConnectionID(), packet, packetSize);

             Logger::getLogger()->Log("Logout user" + user->getUsername());
             qDebug() << "Logout user" << (user->getUsername().c_str());
             delete[] data;

             return true;
             break;
         }
    }
    return false;
}

void Server::sendOnlineList(User* user)
{
    unsigned char *data = NULL;
    string s;



    for(unsigned int i = 0; i < mUsers.size(); i++)
    {
        s += mUsers[i]->getUsername() + ":";
        s += mUsers[i]->getConnectionID() + ":";
        s += mUsers[i]->getIPAddress() + ";";
    }
    int dataSize = s.size();
    data = new unsigned char [dataSize];
    memcpy(data, s.c_str(), s.size());

    unsigned char *packet = NULL;
    int packetSize = this->createPacket(GET_ONLINE_USER_LIST_RESPONSE,data,&packet,dataSize);
}

bool Server::sendConnectionRequest(User *from, User *to, unsigned char *data, int size)
{
    /*unsigned char *newData = NULL;
    int newDataSize = from->getIPAddress().size() + DATA_SPLITER.size() + size;
    data = new unsigned char [newDataSize];


    memcpy(newData, from->getIPAddress().c_str(), from->getIPAddress().size());
    memcpy(&newData[from->getIPAddress().size()], DATA_SPLITER.c_str(), DATA_SPLITER.size());
    memcpy(&newData[from->getIPAddress().size() + DATA_SPLITER.size()],data, size);
*/
    unsigned char *packet = NULL;
    int packetSize = this->createPacket(SERVER_COMUNICATION_REQUEST,data,&packet,size);

    //bude nasledovat sifrovani a poslani pres sit
    this->mNetwork->sendData(to->getConnectionID(), packet, packetSize);
    //delete[] data;

    return true;
}

bool Server::sendConnectionResponse(User *from, User *to, unsigned char *data, int size)
{
    /*unsigned char *newData = NULL;
    int newDataSize = from->getIPAddress().size() + DATA_SPLITER.size() + size;
    data = new unsigned char [newDataSize];


    memcpy(newData, from->getIPAddress().c_str(), from->getIPAddress().size());
    memcpy(&newData[from->getIPAddress().size()], DATA_SPLITER.c_str(), DATA_SPLITER.size());
    memcpy(&newData[from->getIPAddress().size() + DATA_SPLITER.size()],data, size);
*/
    unsigned char *packet = NULL;
    int packetSize = this->createPacket(SERVER_COMUNICATION_RESPONSE,data,&packet,size);

    //bude nasledovat sifrovani a poslani pres sit
    this->mNetwork->sendData(to->getConnectionID(), packet, packetSize);
  //  delete[] data;

    return true;
}

int Server::createPacket(unsigned char id, unsigned char *data, unsigned char **packet, int size)
{
    int newSize = ID_LENGHT + RANDOM_BYTES_LENGTH + sizeof(size) + size;
    *packet = new unsigned char[newSize];//casem pribude hash

    (*packet)[0] = id;
    //tady asi pak bude treba lepsi random
    for(int i = ID_LENGHT; i < ID_LENGHT + RANDOM_BYTES_LENGTH; i++)
    {
        (*packet)[i] = rand() % 256;
    }

    //int to byte
    if(sizeof(size) == 4)
    {
        (*packet)[ID_LENGHT + RANDOM_BYTES_LENGTH] = size & 0x000000ff;
        (*packet)[ID_LENGHT + RANDOM_BYTES_LENGTH + 1] = (size & 0x0000ff00) >> 8;
        (*packet)[ID_LENGHT + RANDOM_BYTES_LENGTH + 2] = (size & 0x00ff0000) >> 16;
        (*packet)[ID_LENGHT + RANDOM_BYTES_LENGTH + 3] = (size & 0xff000000) >> 24;
    }
    memcpy(&((*packet)[ID_LENGHT + RANDOM_BYTES_LENGTH + sizeof(size)]),data, size);

    return newSize;
}



void Server::processPacket(int connectionID, unsigned char* packet, int size)
{
    Logger::getLogger()->Log("Received packet from connection" + connectionID);
    qDebug() << "Received packet from connection" << connectionID;

    int id = 0;
    int dataSize = 0;

    if (size < ID_LENGHT + RANDOM_BYTES_LENGTH + DATA_SIZE_LENGTH)
    {
        Logger::getLogger()->Log("Received packet with invalid size:" + size);
        qDebug() << "Received packet with invalid size:" << size;
        return;
    }

    //desifrovani zkontrolovani hashu atd. tady bude
    if(sizeof(int) == 4)
    {
        id = packet[0];
        dataSize = ( packet[ID_LENGHT + RANDOM_BYTES_LENGTH + 3] << 24) | ( packet[ ID_LENGHT + RANDOM_BYTES_LENGTH +2] << 16) | (packet[ID_LENGHT + RANDOM_BYTES_LENGTH + 1] << 8) | ( packet[ID_LENGHT + RANDOM_BYTES_LENGTH]);

        if (dataSize + ID_LENGHT + RANDOM_BYTES_LENGTH + DATA_SIZE_LENGTH != size || dataSize < 0)
        {
            string s = "Received packet with invalid data size:" + dataSize;
            s +=  "- total packet size:" + size;
            Logger::getLogger()->Log(s);
            qDebug() << "Received packet with invalid data size:" << dataSize << "- total packet size:" << size;
            return;
        }

        unsigned char *data = new unsigned char [dataSize];
        memcpy(data, &packet[ID_LENGHT + RANDOM_BYTES_LENGTH + 4], dataSize);
        User * from = NULL;
        User* to = NULL;
        switch(id)
        {
         case LOGIN_REQUEST:
              Logger::getLogger()->Log("Got LOGIN_REQUEST packet");
              qDebug() << "Got LOGIN_REQUEST packet";
              this->processLoginUserPacket(connectionID, data, dataSize);
              break;
         case LOGOUT_REQUEST:
             Logger::getLogger()->Log("Got LOGOUT_REQUEST packet");
             qDebug() << "Got LOGOUT_REQUEST packet";
             this->processLogoutUserPacket(connectionID, data, dataSize);
             break;
        case  GET_ONLINE_USER_LIST_REQUEST:
            Logger::getLogger()->Log("Got GET_ONLINE_USER_LIST_REQUEST packet");
            from = getUserFromConnectionID(connectionID);
            sendOnlineList(from);
            break;
        case CLIENT_COMUNICATION_REQUEST:
            Logger::getLogger()->Log("Got CLIENT_COMUNICATION_REQUEST packet");
            from = getUserFromConnectionID(connectionID);
            to = getUserFromConnectionID(processClientComunicationRequest(data,dataSize));
            sendConnectionRequest(from, to, data, dataSize);
            break;
        case CLIENT_COMUNICATION_RESPONSE:
            Logger::getLogger()->Log("Got CLIENT_COMUNICATION_RESPONSE packet");
            from = getUserFromConnectionID(connectionID);
            to = getUserFromConnectionID(processClientComunicationResponse(data,dataSize));
            sendConnectionResponse(from, to, data, dataSize);
            break;
         default:
             break;

        }
        delete[] data;
    }

}

void Server::processLoginUserPacket(int connectionID, unsigned char *data, int size)
{
    QString stringData = "";
    for (int i = 0; i < size; i++)
        stringData.append(data[i]);
    QStringList parts = stringData.split(';');

    if (parts.length() != 2)
    {
        Logger::getLogger()->Log("Received invalid login request packet, number of parts:" + parts.length());
        qDebug() << "Received invalid login request packet, number of parts:" << parts.length();
        return;
    }

    User* newUser = new User(parts[0].toStdString(), parts[1].toStdString(), "", NULL, connectionID);
    this->loginUser(newUser);
}

void Server::processLogoutUserPacket(int connectionID, unsigned char *data, int size)
{
    QString stringData = "";
    for (int i = 0; i < size; i++)
        stringData.append(data[i]);
    QStringList parts = stringData.split(';');

    if (parts.length() != 2)
    {
        Logger::getLogger()->Log("Received invalid login request packet, number of parts:" + parts.length());
        qDebug() << "Received invalid logout request packet, number of parts:" << parts.length();
        return;
    }

    User* user = new User(parts[0].toStdString(), parts[1].toStdString(), "", NULL, connectionID);
    this->logoutUser(user);
}
int Server::processClientComunicationRequest(unsigned char *data, int size)
{
    int connectionID = 0;
    if(size > 3)
        connectionID = ( data[3] << 24) | ( data[2] << 16) | (data[1] << 8) | ( data[0]);
    return connectionID;
}

int Server::processClientComunicationResponse(unsigned char *data, int size)
{
    int connectionID = 0;
    if(size > 4)
        connectionID = ( data[4] << 24) | ( data[3] << 16) | (data[2] << 8) | ( data[1]);
    return connectionID;
}

User* Server::getUserFromConnectionID(int connectionID)
{
    for(int i = 0; i < mUsers.size(); i++)
    {
        if(mUsers[i]->getConnectionID() == connectionID)
             return mUsers[i];
    }
    return NULL;
}

int Server::processPacket(unsigned char* packet, unsigned char** data)
{
    if(packet == NULL)
        return -1;

    int id = 0;
    int dataSize = 0;

    if(sizeof(int) == 4)
    {
        id = packet[0];
        dataSize = ( packet[ID_LENGHT + RANDOM_BYTES_LENGTH + 3] << 24) | ( packet[ ID_LENGHT + RANDOM_BYTES_LENGTH +2] << 16) | (packet[ID_LENGHT + RANDOM_BYTES_LENGTH + 1] << 8) | ( packet[ID_LENGHT + RANDOM_BYTES_LENGTH]);
        *data = new unsigned char [dataSize];
         memcpy(*data, &packet[ID_LENGHT + RANDOM_BYTES_LENGTH + 4], dataSize);

        return dataSize;
    }
}
