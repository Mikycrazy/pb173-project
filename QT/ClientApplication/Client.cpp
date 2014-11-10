#include "Client.h"

Client::Client(string username, string email) : mUsername(username), mEmail(email), mLoggedToServer(false), mConnectedToClient(false)
{
    srand(time(NULL));

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
        memcpy(data, &packet[ID_LENGHT + RANDOM_BYTES_LENGTH + 4], dataSize);

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
            break;
         default:
             break;

        }
    }
}

bool Client::isLogged()
{
    return mLoggedToServer;
}

bool Client::isConnected()
{
    return mConnectedToClient;
}
