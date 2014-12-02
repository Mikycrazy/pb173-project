#include "Server.h"

Server::Server(quint16 port)
{
    srand(time(NULL));

    this->mNetwork = new NetworkManager();
    this->mCrypto = new CryptoManager();

    connect(this->mNetwork, SIGNAL(receivedData(int,unsigned char*,int)), this, SLOT(processPacket(int,unsigned char*,int)));
    connect(this->mNetwork, SIGNAL(disconnect(int)), this, SLOT(disconnectClient(int)));
    this->mNetwork->startListening(port);
}

Server::Server()
{
    this->mNetwork = new NetworkManager();
    this->mCrypto = new CryptoManager();
}

Server::~Server()
{
    delete[] mPrivateKey;
    delete[] mPublicKey;

    delete mNetwork;
    delete mCrypto;

    if(mUsers.size() > 0)
        for(auto i : mUsers)
            delete i;
}

bool Server::loginUser(User* user)
{
    user->setOnline();
    mUsers.push_back(user);

    Logger::getLogger()->Log("Logged user " + user->getUsername());

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
    std::stringstream ss;
    string s;

    for(unsigned int i = 0; i < mUsers.size(); i++)
    {
        ss << mUsers[i]->getUsername() + ":";
        ss << mUsers[i]->getConnectionID() << ":";
        ss << mUsers[i]->getIPAddress() + ";";
        std::cout << ss.str() << std::endl;
        std::cout << mUsers[i]->getConnectionID() << std::endl;

    }
    s = ss.str();
    int dataSize = s.size();
    data = new unsigned char [dataSize];
    memcpy(data, s.c_str(), s.size());
    Logger::getLogger()->Log("send GET_ONLINE_USER_LIST_RESPONSE packet ");
    Logger::getLogger()->Log(s);

    unsigned char *packet = NULL;
    int packetSize = this->createPacket(GET_ONLINE_USER_LIST_RESPONSE,data,&packet,dataSize);

    this->mNetwork->sendData(user->getConnectionID(), packet, packetSize);
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

    //data sou spatnej maj bejt jine conid
    int conId = from->getConnectionID();
    if(sizeof(size) == 4)
    {
        data[0] = conId & 0x000000ff;
        data[1] = (conId & 0x0000ff00) >> 8;
        data[2] = (conId & 0x00ff0000) >> 16;
        data[3] = (conId & 0xff000000) >> 24;
    }

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
    int newSize = ID_LENGHT + RANDOM_BYTES_LENGTH + sizeof(size) + size + INTERGRITY_HASH_SIZE;

    *packet = new unsigned char[newSize];
    memset(*packet, 0 , newSize);

    (*packet)[0] = id;

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

    memcpy(((*packet) + (ID_LENGHT + RANDOM_BYTES_LENGTH + sizeof(size))), data, size);

    unsigned char* hash = new unsigned char[32];
    mCrypto->computeHash(*packet, hash, newSize - INTERGRITY_HASH_SIZE);

    memcpy(((*packet) + ID_LENGHT + RANDOM_BYTES_LENGTH + sizeof(size) + size), hash, INTERGRITY_HASH_SIZE);

    delete[] hash;

    return newSize;
}



void Server::processPacket(int connectionID, unsigned char* packet, int size)
{
    Logger::getLogger()->Log("Received packet from connection" + connectionID);

    int id = 0;
    int dataSize = 0;

    if (size < ID_LENGHT + RANDOM_BYTES_LENGTH + DATA_SIZE_LENGTH + INTERGRITY_HASH_SIZE)
    {

        Logger::getLogger()->Log("Received packet with invalid size:" + size);
        return;
    }

    unsigned char* packethash = new unsigned char[INTERGRITY_HASH_SIZE];
    memcpy(packethash, packet + size - INTERGRITY_HASH_SIZE, INTERGRITY_HASH_SIZE);

    unsigned char* computedhash = new unsigned char[INTERGRITY_HASH_SIZE];
    mCrypto->computeHash(packet, computedhash, size - INTERGRITY_HASH_SIZE);

    if(!mCrypto->compareHash(packethash, computedhash, INTERGRITY_HASH_SIZE))
    {
        Logger::getLogger()->Log("Hashes not matching!!");
        //return;
    }

    if(sizeof(int) == 4)
    {
        id = packet[0];
        dataSize = ( packet[ID_LENGHT + RANDOM_BYTES_LENGTH + 3] << 24) | ( packet[ ID_LENGHT + RANDOM_BYTES_LENGTH +2] << 16) | (packet[ID_LENGHT + RANDOM_BYTES_LENGTH + 1] << 8) | ( packet[ID_LENGHT + RANDOM_BYTES_LENGTH]);

        if (dataSize + ID_LENGHT + RANDOM_BYTES_LENGTH + DATA_SIZE_LENGTH + INTERGRITY_HASH_SIZE != size || dataSize < 0)
        {
            string s = "Received packet with invalid data size:" + dataSize;
            s +=  "- total packet size:" + size;
            Logger::getLogger()->Log(s);
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
            this->processLoginUserPacket(connectionID, data, dataSize);
            for(auto i : mUsers)
                std::cout << i->getUsername() << std::endl;
            break;
        case LOGOUT_REQUEST:
            this->processLogoutUserPacket(connectionID, data, dataSize);
            Logger::getLogger()->Log("Got LOGOUT_REQUEST packet" + from->getUsername());
            break;
        case  GET_ONLINE_USER_LIST_REQUEST:
            from = getUserFromConnectionID(connectionID);
            Logger::getLogger()->Log("Got GET_ONLINE_USER_LIST_REQUEST packetfrom: " + from->getUsername());
            sendOnlineList(from);
            break;
        case CLIENT_COMUNICATION_REQUEST:

            from = getUserFromConnectionID(connectionID);
            to = getUserFromConnectionID(processClientComunicationRequest(data,dataSize));
            Logger::getLogger()->Log("Got CLIENT_COMUNICATION_REQUEST packet from: "+ from->getUsername() + " to: "+ to->getUsername());
            sendConnectionRequest(from, to, data, dataSize);
            break;
        case CLIENT_COMUNICATION_RESPONSE:

            from = getUserFromConnectionID(connectionID);
            to = getUserFromConnectionID(processClientComunicationResponse(data,dataSize));
            Logger::getLogger()->Log("Got CLIENT_COMUNICATION_RESPONSE packet: "+ from->getUsername() + " to: "+ to->getUsername());
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

int Server::processPacket(unsigned char* packet, unsigned char** data, int size)
{
    if(packet == NULL)
     return -1;

    int id = 0;
    int dataSize = 0;

    unsigned char* packethash = new unsigned char[INTERGRITY_HASH_SIZE];
    memcpy(packethash, packet + size - INTERGRITY_HASH_SIZE, INTERGRITY_HASH_SIZE);

    unsigned char* computedhash = new unsigned char[INTERGRITY_HASH_SIZE];
    mCrypto->computeHash(packet, computedhash, size - INTERGRITY_HASH_SIZE);

    std::cout << "Hash: ";
    for (int i = 0; i < INTERGRITY_HASH_SIZE; i++) {
      printf("%02X", packethash[i]);
    }
    std::cout << std::endl;

    std::cout << "Hash: ";
    for (int i = 0; i < INTERGRITY_HASH_SIZE; i++) {
      printf("%02X", computedhash[i]);
    }
    std::cout << std::endl;


    if(sizeof(int) == 4)
    {
        id = packet[0];
        dataSize = ( packet[ID_LENGHT + RANDOM_BYTES_LENGTH + 3] << 24) | ( packet[ ID_LENGHT + RANDOM_BYTES_LENGTH +2] << 16) | (packet[ID_LENGHT + RANDOM_BYTES_LENGTH + 1] << 8) | ( packet[ID_LENGHT + RANDOM_BYTES_LENGTH]);
        *data = new unsigned char [dataSize];
        memcpy(*data, &packet[ID_LENGHT + RANDOM_BYTES_LENGTH + 4], dataSize);
    }
    return dataSize;
}

void Server::disconnectClient(int connectionID)
{
    for(std::vector<User*>::const_iterator it = mUsers.begin(); it != mUsers.end(); it++)
    {
        if((*it)->getConnectionID() == connectionID)
        {
            mUsers.erase(it);
            break;
        }
    }
}
