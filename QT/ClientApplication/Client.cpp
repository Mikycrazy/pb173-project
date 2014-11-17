#include "Client.h"

Client::Client(string username, string email) : mUsername(username), mEmail(email), mLoggedToServer(false), mConnectedToClient(false)
{
    srand(time(NULL));

    mLastReicevedDataSize = 10;
    mLastReicevedData = new unsigned char [10];
    memset(mLastReicevedData, 97, 10);

    this->mNetwork = new NetworkManager();
    this->mNetwork->startConnection(SERVER_ADDRESS, SERVER_PORT);

    connect(this->mNetwork, SIGNAL(networkReceivedData(unsigned char*,int)), this, SLOT(processPacket(unsigned char*,int)));
}

int Client::login()
{
    unsigned char *data = NULL;
    int dataSize = mUsername.size() + mEmail.size() + DATA_SPLITER.size();
    data = new unsigned char [dataSize];

    memcpy(data, mUsername.c_str(), mUsername.size());
    memcpy(&data[mUsername.size()], DATA_SPLITER.c_str(), DATA_SPLITER.size());
    memcpy(&data[mUsername.size() + DATA_SPLITER.size()], mEmail.c_str(), mEmail.size());

    unsigned char *packet = NULL;
    int packetSize = this->createPacket(LOGIN_REQUEST,data,&packet,dataSize);
    Logger::getLogger()->Log("Client:"+ mUsername +" send LOGIN_REQUEST");
    //bude nasledovat sifrovani a poslani pres sit
    this->mNetwork->sendData(packet, packetSize);

    delete[] data;
    return 0;
}
int Client::logout()
{
    if(!mLoggedToServer)
        return 1;

    unsigned char *data = NULL;
    int dataSize = mUsername.size() + mEmail.size() + DATA_SPLITER.size();
    data = new unsigned char [dataSize];

    memcpy(data, mUsername.c_str(), mUsername.size());
    memcpy(&data[mUsername.size()], DATA_SPLITER.c_str(), DATA_SPLITER.size());
    memcpy(&data[mUsername.size() + DATA_SPLITER.size()], mEmail.c_str(), mEmail.size());

    unsigned char *packet = NULL;
    int packetSize = this->createPacket(LOGOUT_REQUEST,data,&packet,dataSize);

    //bude nasledovat sifrovani a poslani pres sit
    this->mNetwork->sendData(packet, packetSize);
    Logger::getLogger()->Log("Client:"+ mUsername +" send LOGOUT_REQUEST");

    delete[] data;
    return 0;
}

int Client::getOnlineList()
{
    if(!mLoggedToServer)
        return 1;

    unsigned char *data = NULL;
    int dataSize = mUsername.size();
    data = new unsigned char [dataSize];

    memcpy(data, mUsername.c_str(), mUsername.size());

    unsigned char *packet = NULL;
    int packetSize = this->createPacket(GET_ONLINE_USER_LIST_REQUEST,data,&packet,dataSize);

    //bude nasledovat sifrovani a poslani pres sit
    this->mNetwork->sendData(packet, packetSize);
    Logger::getLogger()->Log("Client:"+ mUsername +" send GET_ONLINE_USER_LIST_REQUEST");

    delete[] data;
    return 0;
}

int Client::connectToClient(int connectionID)
{
    if(!mLoggedToServer)
        return 1;

    unsigned char *data = NULL;
    int dataSize = sizeof(connectionID) + AES_KEY_LENGTH/2;
    data = new unsigned char [dataSize];
    //int to byte
    if(sizeof(connectionID) == 4)
    {
        data[0] = connectionID & 0x000000ff;
        data[1] = (connectionID & 0x0000ff00) >> 8;
        data[2] = (connectionID & 0x00ff0000) >> 16;
        data[3] = (connectionID & 0xff000000) >> 24;
    }

    //generovani klice
    for(int i = sizeof(connectionID); i < dataSize; i++)
    {
        data[i] = rand() % 256;
    }

    unsigned char *packet = NULL;
    int packetSize = this->createPacket(CLIENT_COMUNICATION_REQUEST,data,&packet,dataSize);

    //bude nasledovat sifrovani a poslani pres sit
    this->mNetwork->sendData(packet, packetSize);
    Logger::getLogger()->Log("Client:"+ mUsername +" send CLIENT_COMUNICATION_REQUEST");

    delete[] data;
    return 0;
}


int Client::acceptConnection(int connectionID, unsigned char* recievedKey)
{
     qDebug() << "enter acceptConection";
    //if(!mLoggedToServer)
       // return 1;

    unsigned char *data = NULL;
    int dataSize = 1 + sizeof(dataSize) + AES_KEY_LENGTH + AES_IV_LENGTH;
    data = new unsigned char [dataSize];
    qDebug() << "datasize: " <<  dataSize;
    data[0] = 1;
    //int to byte
    if(sizeof(connectionID) == 4)
    {
        data[1] = connectionID & 0x000000ff;
        data[2] = (connectionID & 0x0000ff00) >> 8;
        data[3] = (connectionID & 0x00ff0000) >> 16;
        data[4] = (connectionID & 0xff000000) >> 24;
    }

    memcpy( &(data[5]), recievedKey,AES_KEY_LENGTH/2);
    //generovani klice
    for(int i = 1 + sizeof(connectionID) + AES_KEY_LENGTH/2; i < 1 + sizeof(connectionID) + AES_KEY_LENGTH; i++)
        data[i] = rand() % 256;


    mAESkey = new unsigned char[AES_KEY_LENGTH];
    memcpy(mAESkey,  &(data[5]), AES_KEY_LENGTH);

    // generovani IV
    for(int i = 1 + sizeof(connectionID) + AES_KEY_LENGTH; i < dataSize; i++)
        data[i] = rand() % 256;

    mAESIV = new unsigned char[AES_IV_LENGTH];
    memcpy(mAESkey,  &(data[1 + sizeof(connectionID) + AES_KEY_LENGTH]), AES_KEY_LENGTH);

    unsigned char *packet = NULL;
    int packetSize = this->createPacket(CLIENT_COMUNICATION_RESPONSE,data,&packet,dataSize);

    //bude nasledovat sifrovani a poslani pres sit
    this->mNetwork->sendData(packet, packetSize);
    Logger::getLogger()->Log("Client:"+ mUsername +" send CLIENT_COMUNICATION_RESPONSE");

    delete[] data;
    return 0;

}

int Client::refuseConnection(int connectionID)
{
    qDebug() << "enter refuseConnection";
   //if(!mLoggedToServer)
      // return 1;

   unsigned char *data = NULL;
   int dataSize = 1 + sizeof(dataSize);
   data = new unsigned char [dataSize];
   qDebug() << "datasize: " <<  dataSize;
   data[0] = 0;

   data[0] = 1;
   //int to byte
   if(sizeof(connectionID) == 4)
   {
       data[1] = connectionID & 0x000000ff;
       data[2] = (connectionID & 0x0000ff00) >> 8;
       data[3] = (connectionID & 0x00ff0000) >> 16;
       data[4] = (connectionID & 0xff000000) >> 24;
   }

   unsigned char *packet = NULL;
   int packetSize = this->createPacket(CLIENT_COMUNICATION_RESPONSE,data,&packet,dataSize);

   //bude nasledovat sifrovani a poslani pres sit
   this->mNetwork->sendData(packet, packetSize);
   Logger::getLogger()->Log("Client:"+ mUsername +" send CLIENT_COMUNICATION_RESPONSE");

   delete[] data;

   return 0;
}

int Client::sendDataToClient(QHostAddress address, quint16 port, unsigned char* data, int size)
{
    unsigned char *packet = NULL;
    int packetSize = this->createPacket(CLIENT_COMMUNICATION_DATA,data,&packet,size);

    //bude nasledovat sifrovani a poslani pres sit
    this->mNetwork->sendUdpData(address, port, packet, packetSize);
    Logger::getLogger()->Log("Client:"+ mUsername +" send CLIENT_COMUNICATION_DATA");

    return 0;
}

int Client::createPacket(unsigned char id, unsigned char *data, unsigned char **packet, int size)
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

    memcpy(&((*packet)[ID_LENGHT + RANDOM_BYTES_LENGTH + sizeof(size)]), data, size);

    return newSize;
}

void Client::processPacket(unsigned char* packet, int size)
{
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
            /*stringstream s;
            s << "Received packet with invalid data size:" << dataSize << "- total packet size:" << size;*/
            //string log = "Received packet with invalid data size:" + "a";
           // log += "aa";
            string s = "Received packet with invalid data size:" + dataSize;
            s +=  "- total packet size:" + size;
            Logger::getLogger()->Log(s);
            qDebug() << "Received packet with invalid data size:" << dataSize << "- total packet size:" << size;
            return;
        }

        unsigned char *data = new unsigned char [dataSize];
        //mLastReicevedData = new unsigned char [dataSize];
        memcpy(data, &packet[ID_LENGHT + RANDOM_BYTES_LENGTH + 4], dataSize);
        //memcpy(mLastReicevedData, &packet[ID_LENGHT + RANDOM_BYTES_LENGTH + 4], dataSize);
        int accept = 0;
        int i = 0;
        QByteArray bdata((const char*)data, dataSize);
        switch(id)
        {
         case LOGIN_RESPONSE:
            if(mUsername.compare((const char*)data))
            {
                mLoggedToServer = true;
                Logger::getLogger()->Log("Login successful");
                qDebug() << "Login successful";

            }
            break;
         case LOGOUT_RESPONSE:
            if(mUsername.compare((const char*)data))
            {
                mLoggedToServer = false;
                Logger::getLogger()->Log("Logout successful");
                qDebug() << "Logout successful";
            }
            break;
        case GET_ONLINE_USER_LIST_REQUEST:
            Logger::getLogger()->Log("GET_ONLINE_USER_LIST_REQUEST");
            for (int i = 0; i < dataSize; i++)
            {
                if(data[i] == ';')
                    qDebug() << "";
                else
                    qDebug() << data[i];
            }
            break;
        case GET_ONLINE_USER_LIST_RESPONSE:
            Logger::getLogger()->Log("GET_ONLINE_USER_LIST_RESPONSE");
             qDebug() << "Received online list:" << bdata;
            processGetOnlineListResponse(data, dataSize);
            //tohle tadz pak nebude ale ted nwm kam to dat
             this->connectToClient(this->OnlineList()[0]->getConnectionID());

            break;
        case SERVER_COMUNICATION_REQUEST:
             Logger::getLogger()->Log("SERVER_COMUNICATION_REQUEST");
                acceptConnection(processServerCommunicationRequest(data,dataSize),&data[4]);

                break;
        case SERVER_COMUNICATION_RESPONSE:
             Logger::getLogger()->Log(" SERVER_COMUNICATION_RESPONSE");
                accept = processServerCommunicationResponse(data,dataSize);
                //druhy klient zamitnul spojeni
                if(!accept)
                {

                }
                //druhy klient prijal spojeni
                else
                {

                }
                break;
        case CLIENT_COMMUNICATION_DATA:
            qDebug() << "Received UDP data:" << bdata;
            break;
         default:
             break;

        }
        delete[] data;
    }
}

int Client::processServerCommunicationRequest(unsigned char *data, int size)
{
    int connectionID = 0;
    if(size > 3)
    {
        connectionID = ( data[3] << 24) | ( data[2] << 16) | (data[1] << 8) | ( data[0]);

    }
    return connectionID;
}
int Client::processServerCommunicationResponse(unsigned char *data, int size)
{
    char accept = data[0];
    if(accept == 1)
    {
        int connectionID  = 0;
        if(size > 3)
            connectionID  = ( data[4] << 24) | ( data[3] << 16) | (data[2] << 8) | ( data[1]);
        int i, j;
        for(i = 5, j = 0; i < size; i++,j++)
        {
            if(data[i] == ';')
                break;
            mAESkey[j] = data[i];
        }
        for(j = 0; i < size; i++,j++)
            mAESIV[j] = data[i];


    }

    return accept;
}
int Client::processGetOnlineListResponse(unsigned char *data, int size)
{
    QString stringData = "";
    for (int i = 0; i < size; i++)
        stringData.append(data[i]);

    QStringList parts = stringData.split(';');


    for(int i = 0; i < parts.size();i++)
    {
       QStringList u = parts[i].split(':');
       if(u.size() > 2)
            mOnlineList.push_back(new User(u[0].toStdString(),"" , "", NULL, std::stoi(u[1].toStdString())));
    }
    return 0;
}

int Client::processPacket(unsigned char* packet, unsigned char** data)
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
    }
    return dataSize;
}

bool Client::isLogged()
{
    return mLoggedToServer;
}

bool Client::isConnected()
{
    return mConnectedToClient;
}

void Client::sendData(unsigned char *data, int size)
{
    this->mNetwork->sendData(data, size);
}
