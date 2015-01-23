#include "Client.h"

Client::Client(string username, string email, qint16 UDPport) : mUsername(username), mEmail(email), mLoggedToServer(false), mConnectedToClient(false)
{
    srand(time(NULL));

    try
    {
        mLastReicevedData = new unsigned char [10];
        mAESkey = new unsigned char[AES_KEY_LENGTH];
        mAESIV = new unsigned char[AES_IV_LENGTH];
        mCrypto = new CryptoManager();
    }
    catch(std::bad_alloc& exc)
    {
        Logger::getLogger()->Log(exc.what());
    }

    mLastReicevedDataSize = 10;
    memset(mLastReicevedData, 97, 10);

    this->UDPport = UDPport;
    this->initNetwork();

    memset(mAESkey,'b',AES_KEY_LENGTH);
    memset(mAESIV,'c',AES_IV_LENGTH);

    mStatus = 256;
    mCypherPosition = 0;
    mCypherLastPositionreceived = 0;

   // connect(this->mNetwork, SIGNAL(networkReceivedData(unsigned char*,int)), this, SLOT(processPacket(unsigned char*,int)));
}

Client::Client() : mLoggedToServer(false), mConnectedToClient(false)
{
    try
    {
        this->mCrypto = new CryptoManager();
        this->mNetwork = new NetworkManager();
    }
    catch(std::bad_alloc& exc)
    {
        Logger::getLogger()->Log(exc.what());
    }
}

Client::~Client()
{
    delete[] mAESkey;
    delete[] mAESIV;
    delete[] mPublicRSAKey;
    delete[] mPrivateRSAKey;
    delete[] mLastReicevedData;

    delete mNetwork;
    delete mCrypto;

    if(mOnlineList.size() > 0)
        for(auto i : mOnlineList)
            delete i;

}

void Client::initNetwork()
{
    try
    {
        this->mNetwork = new NetworkManager();
    }
    catch(std::bad_alloc& exc)
    {
        Logger::getLogger()->Log(exc.what());
    }


    this->mNetwork->startConnection(SERVER_ADDRESS, SERVER_PORT, this->UDPport);

    connect(this->mNetwork, SIGNAL(networkReceivedData(unsigned char*,int)), this, SLOT(processPacket(unsigned char*,int)));

}

int Client::login()
{
    unsigned char *data = NULL;
    int dataSize = mUsername.size() + mEmail.size() + DATA_SPLITER.size();

    try
    {
        data = new unsigned char [dataSize];
    }
    catch(std::bad_alloc& exc)
    {
        Logger::getLogger()->Log(exc.what());
    }



    if(data == NULL)
        Logger::getLogger()->Log("Allocation memmory Failed");

    memcpy(data, mUsername.c_str(), mUsername.size());
    memcpy(&data[mUsername.size()], DATA_SPLITER.c_str(), DATA_SPLITER.size());
    memcpy(&data[mUsername.size() + DATA_SPLITER.size()], mEmail.c_str(), mEmail.size());

    unsigned char *packet = NULL;
    int packetSize = this->createPacket(LOGIN_REQUEST,data,&packet,dataSize);
    Logger::getLogger()->Log("Client: "+ mUsername +" send LOGIN_REQUEST");
    this->mNetwork->sendData(packet, packetSize);

    delete[] data;
    delete[] packet;

    int counter = 0;
    std::chrono::milliseconds sleepTime(10);
    while(!this->isLogged() && ++counter < 200)
    {
        std::this_thread::sleep_for(sleepTime);
        qApp->processEvents();
    }
    if (counter == 200)
    {
        Logger::getLogger()->Log("Connection with server failed. Reconnecting...");
        this->initNetwork();
        return this->login();
    }

    return 0;
}
int Client::logout()
{
    if(!mLoggedToServer)
        return 1;

    unsigned char *data = NULL;
    int dataSize = mUsername.size() + mEmail.size() + DATA_SPLITER.size();
    try
    {
        data = new unsigned char [dataSize];
    }
    catch(std::bad_alloc& exc)
    {
        Logger::getLogger()->Log(exc.what());
    }

    if(data == NULL)
        Logger::getLogger()->Log("Allocation memmory Failed");

    memset(data, 0, dataSize);

    memcpy(data, mUsername.c_str(), mUsername.size());
    memcpy(&data[mUsername.size()], DATA_SPLITER.c_str(), DATA_SPLITER.size());
    memcpy(&data[mUsername.size() + DATA_SPLITER.size()], mEmail.c_str(), mEmail.size());

    unsigned char *packet = NULL;
    int packetSize = this->createPacket(LOGOUT_REQUEST,data,&packet,dataSize);

    this->mNetwork->sendData(packet, packetSize);
    Logger::getLogger()->Log("Client:"+ mUsername +" send LOGOUT_REQUEST");

    delete[] data;
    delete[] packet;

    return 0;
}

int Client::getOnlineList()
{
    if(!mLoggedToServer)
        return 1;

    unsigned char *data = NULL;
    int dataSize = mUsername.size();

    try
    {
        data = new unsigned char [dataSize];
    }
    catch(std::bad_alloc& exc)
    {
        Logger::getLogger()->Log(exc.what());
    }

    if(data == NULL)
        Logger::getLogger()->Log("Allocation memmory Failed");

    memset(data, 0, dataSize);


    memcpy(data, mUsername.c_str(), mUsername.size());

    unsigned char *packet = NULL;
    int packetSize = this->createPacket(GET_ONLINE_USER_LIST_REQUEST,data,&packet,dataSize);

    this->mNetwork->sendData(packet, packetSize);
    Logger::getLogger()->Log("Client:"+ mUsername +" send GET_ONLINE_USER_LIST_REQUEST");

    delete[] data;
    delete[] packet;
    return 0;
}

int Client::connectToClient(int connectionID)
{
    //if(!mLoggedToServer)
        //return 1;

    unsigned char *data = NULL;
    int dataSize = sizeof(connectionID) + AES_KEY_LENGTH / 2;
    try
    {
        data = new unsigned char [dataSize];
    }
    catch(std::bad_alloc& exc)
    {
        Logger::getLogger()->Log(exc.what());
    }

    if(data == NULL)
        Logger::getLogger()->Log("Allocation memmory Failed");

    memset(data, 0, dataSize);

    if(sizeof(connectionID) == 4)
    {
        data[0] = connectionID & 0x000000ff;
        data[1] = (connectionID & 0x0000ff00) >> 8;
        data[2] = (connectionID & 0x00ff0000) >> 16;
        data[3] = (connectionID & 0xff000000) >> 24;
    }
    else return 1;

    //key generation

    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<unsigned long long> dis;

    for(int i = sizeof(connectionID); i < dataSize; i++)
    {
        data[i] = dis(gen) % 256;
    }

    unsigned char *packet = NULL;
    int packetSize = this->createPacket(CLIENT_COMUNICATION_REQUEST,data,&packet,dataSize);

    this->mNetwork->sendData(packet, packetSize);
    Logger::getLogger()->Log("Client:"+ mUsername +" send CLIENT_COMUNICATION_REQUEST");

    delete[] data;
    delete[] packet;

    return 0;
}


int Client::acceptConnection(int connectionID, unsigned char* receivedKey)
{
    //if(!mLoggedToServer)
       // return 1;

    unsigned char *data = NULL;
    int dataSize = 1 + sizeof(dataSize) + AES_KEY_LENGTH + AES_IV_LENGTH;

    try
    {
        data = new unsigned char [dataSize];
    }
    catch(std::bad_alloc& exc)
    {
        Logger::getLogger()->Log(exc.what());
    }

    if(data == NULL)
        Logger::getLogger()->Log("Allocation memmory Failed");

    memset(data, 0, dataSize);

    qDebug() << "Datasize: " <<  dataSize;
    data[0] = 1;
    //int to byte
    if(sizeof(connectionID) == 4)
    {
        data[1] = connectionID & 0x000000ff;
        data[2] = (connectionID & 0x0000ff00) >> 8;
        data[3] = (connectionID & 0x00ff0000) >> 16;
        data[4] = (connectionID & 0xff000000) >> 24;
    }
    else
        return 1;

    memcpy( data + sizeof(connectionID) + 1, receivedKey,AES_KEY_LENGTH/2);

    //key generation


    for(int i = 1 + sizeof(connectionID) + AES_KEY_LENGTH / 2; i < 1 + sizeof(connectionID) + AES_KEY_LENGTH; i++)
        data[i] = rand() % 256;

    memcpy(mAESkey,  data + 1 + sizeof(connectionID), AES_KEY_LENGTH);

    // generate IV
    for(int i = 1 + sizeof(connectionID) + AES_KEY_LENGTH; i < dataSize; i++)
        data[i] = rand() % 256;

    memcpy(mAESIV,  &(data[1 + sizeof(connectionID) + AES_KEY_LENGTH]), AES_IV_LENGTH);

    mCrypto->computeHash(mAESkey,mAESkey,AES_KEY_LENGTH);

    unsigned char *packet = NULL;
    int packetSize = this->createPacket(CLIENT_COMUNICATION_RESPONSE,data,&packet,dataSize);

    QByteArray bdata((const char*)mAESkey, AES_KEY_LENGTH);
    this->mNetwork->sendData(packet, packetSize);
    Logger::getLogger()->Log("Client:"+ mUsername +" send CLIENT_COMUNICATION_RESPONSE");

    delete[] data;
    delete[] packet;
    return 0;
}

int Client::refuseConnection(int connectionID)
{
   //if(!mLoggedToServer)
      // return 1;

   unsigned char *data = NULL;
   int dataSize = 1 + sizeof(dataSize);
   try
   {
       data = new unsigned char [dataSize];
   }
   catch(std::bad_alloc& exc)
   {
       Logger::getLogger()->Log(exc.what());
   }

   if(data == NULL)
       Logger::getLogger()->Log("Allocation memmory Failed");


   data[0] = 0;
   //int to byte
   if(sizeof(connectionID) == 4)
   {
       data[1] = connectionID & 0x000000ff;
       data[2] = (connectionID & 0x0000ff00) >> 8;
       data[3] = (connectionID & 0x00ff0000) >> 16;
       data[4] = (connectionID & 0xff000000) >> 24;
   }
   else return 1;

   unsigned char *packet = NULL;
   int packetSize = this->createPacket(CLIENT_COMUNICATION_RESPONSE,data,&packet,dataSize);

   this->mNetwork->sendData(packet, packetSize);
   Logger::getLogger()->Log("Client: "+ mUsername +" send CLIENT_COMUNICATION_RESPONSE");

   delete[] data;
   delete[] packet;

   return 0;
}

int Client::sendDataToClient(QHostAddress address, quint16 port, unsigned char* data, int size)
{
    mReceiverIP = address;

    std::cout << "data: ";
    for(int i = 0; i < size; i++)
        std::cout<< data[i];
    unsigned char *packet = nullptr;
    unsigned char *stream = nullptr;

    try
    {
        stream = new unsigned char[size];
    }
    catch(std::bad_alloc& exc)
    {
        Logger::getLogger()->Log(exc.what());
    }

    mCrypto->getEncKeystream(stream, size, mCypherPosition);
    mCrypto->XORData(data, data, size, stream);

    int packetSize = this->createPacket(CLIENT_COMMUNICATION_DATA,data,&packet,size);
    mCypherPosition++;

    this->mNetwork->sendUdpData(address, port, packet, packetSize);
    Logger::getLogger()->Log("Client: "+ mUsername +" send CLIENT_COMUNICATION_DATA");
    std::cout << "data Cyp: ";
    for(int i = 0; i < size; i++)
        std::cout << std::hex << static_cast<int>(data[i]);
    std::cout <<  std::endl;

    std::cout << "stream: ";
    for(int i = 0; i < size; i++)
         std::cout << std::hex << static_cast<int>(stream[i]);
    std::cout << std::endl;

    delete[] stream;
    delete[] packet;

    return 0;
}

int Client::sendDataToClient(QHostAddress address, quint16 port, string filename)
{
    std::ifstream t;
    int length;
    int dataSize = 1024;
    t.open(filename);  // open input file
    if(t.is_open())
    {
        t.seekg(0, std::ios::end);    // go to the end
        length = t.tellg();           // report location (this is the length)
        t.seekg(0, std::ios::beg);    // go back to the beginning
        unsigned char *buffer;

        try
        {
            buffer = new unsigned char[length];    // allocate memory for a buffer of appropriate dimension
        }
        catch(std::bad_alloc& exc)
        {
            Logger::getLogger()->Log(exc.what());
        }


        t.read((char *)buffer, length);       // read the whole file into the buffer
        t.close();

        for(int i = 0; i < length; i+=dataSize)
        {
            if(length - i < dataSize)
                dataSize = length - i;
            sendDataToClient(address, port, &buffer[i], dataSize);
        }
    }
    return 0;
}


int Client::createPacket(unsigned char id, unsigned char *data, unsigned char **packet, int size)
{
    int newSize = ID_LENGHT + RANDOM_BYTES_LENGTH + sizeof(size) + size + INTERGRITY_HASH_SIZE;
    if(id == CLIENT_COMMUNICATION_DATA)
        newSize = ID_LENGHT + RANDOM_BYTES_LENGTH + sizeof(size) + sizeof(mCypherPosition) + size + INTERGRITY_HASH_SIZE;

    unsigned char* hash;

    try
    {
        *packet = new unsigned char[newSize];
        hash = new unsigned char[32];
    }
    catch(std::bad_alloc& exc)
    {
        Logger::getLogger()->Log(exc.what());
    }


    if(*packet == NULL)
        Logger::getLogger()->Log("Allocation memmory Failed");

    memset(*packet,97 , newSize);

    (*packet)[0] = id;

    for(int i = ID_LENGHT; i < ID_LENGHT + RANDOM_BYTES_LENGTH; i++)
    {
        (*packet)[i] = rand() % 256;
    }

    if(id == CLIENT_COMMUNICATION_DATA)
    {

        size += sizeof(mCypherPosition);
        if(sizeof(size) == 4)
        {
            (*packet)[ID_LENGHT + RANDOM_BYTES_LENGTH] = size & 0x000000ff;
            (*packet)[ID_LENGHT + RANDOM_BYTES_LENGTH + 1] = (size & 0x0000ff00) >> 8;
            (*packet)[ID_LENGHT + RANDOM_BYTES_LENGTH + 2] = (size & 0x00ff0000) >> 16;
            (*packet)[ID_LENGHT + RANDOM_BYTES_LENGTH + 3] = (size & 0xff000000) >> 24;
        }
        else return 1;
        std::cout << std::endl;
        if(sizeof(mCypherPosition) == 4)
        {
            (*packet)[ID_LENGHT + RANDOM_BYTES_LENGTH + sizeof(size)] = mCypherPosition & 0x000000ff;
            (*packet)[ID_LENGHT + RANDOM_BYTES_LENGTH + sizeof(size) + 1] = (mCypherPosition & 0x0000ff00) >> 8;
            (*packet)[ID_LENGHT + RANDOM_BYTES_LENGTH + sizeof(size) + 2] = (mCypherPosition & 0x00ff0000) >> 16;
            (*packet)[ID_LENGHT + RANDOM_BYTES_LENGTH + sizeof(size) + 3] = (mCypherPosition & 0xff000000) >> 24;
        }
        memcpy(((*packet) + (ID_LENGHT + RANDOM_BYTES_LENGTH + sizeof(size) + sizeof(mCypherPosition) )), data, size - sizeof(mCypherPosition));

        mCrypto->computeHash(*packet, hash, newSize - INTERGRITY_HASH_SIZE);

        memcpy(((*packet) + ID_LENGHT + RANDOM_BYTES_LENGTH + sizeof(size) + size), hash, INTERGRITY_HASH_SIZE);
        delete[] hash;
    }
    else
    {
        //int to byte
        if(sizeof(size) == 4)
        {
            (*packet)[ID_LENGHT + RANDOM_BYTES_LENGTH] = size & 0x000000ff;
            (*packet)[ID_LENGHT + RANDOM_BYTES_LENGTH + 1] = (size & 0x0000ff00) >> 8;
            (*packet)[ID_LENGHT + RANDOM_BYTES_LENGTH + 2] = (size & 0x00ff0000) >> 16;
            (*packet)[ID_LENGHT + RANDOM_BYTES_LENGTH + 3] = (size & 0xff000000) >> 24;
        }
        else return 1;


        memcpy(((*packet) + (ID_LENGHT + RANDOM_BYTES_LENGTH + sizeof(size))), data, size);


        mCrypto->computeHash(*packet, hash, newSize - INTERGRITY_HASH_SIZE);

        memcpy(((*packet) + ID_LENGHT + RANDOM_BYTES_LENGTH + sizeof(size) + size), hash, INTERGRITY_HASH_SIZE);


    }
    delete[] hash;
    return newSize;
}

void Client::processPacket(unsigned char* packet, int size)
{
    int id = 0;
    int dataSize = 0;

    if (size < ID_LENGHT + RANDOM_BYTES_LENGTH + DATA_SIZE_LENGTH + INTERGRITY_HASH_SIZE)
    {
        Logger::getLogger()->Log("Received packet with invalid size:" + size);
        return;
    }

    unsigned char* packethash;
    unsigned char* computedhash;

    try
    {
        packethash = new unsigned char[INTERGRITY_HASH_SIZE];
        computedhash = new unsigned char[INTERGRITY_HASH_SIZE];
    }
    catch(std::bad_alloc& exc)
    {
        Logger::getLogger()->Log(exc.what());
    }

    memcpy(packethash, packet + size - INTERGRITY_HASH_SIZE, INTERGRITY_HASH_SIZE);
    mCrypto->computeHash(packet, computedhash, size - INTERGRITY_HASH_SIZE);

    if(!mCrypto->compareHash(packethash, computedhash, INTERGRITY_HASH_SIZE))
    {
        Logger::getLogger()->Log("Hashes not matching!!");
    }

    delete[] packethash;
    delete[] computedhash;

    if(sizeof(int) == 4)
    {
        id = packet[0];
        dataSize = ( packet[ID_LENGHT + RANDOM_BYTES_LENGTH + 3] << 24 ) | ( packet[ ID_LENGHT + RANDOM_BYTES_LENGTH +2] << 16 ) | ( packet[ID_LENGHT + RANDOM_BYTES_LENGTH + 1] << 8 ) | (packet[ID_LENGHT + RANDOM_BYTES_LENGTH]);

        if (dataSize + ID_LENGHT + RANDOM_BYTES_LENGTH + DATA_SIZE_LENGTH + INTERGRITY_HASH_SIZE != size || dataSize < 0)
        {
            string s = "Received packet with invalid data size:" + dataSize;
            s +=  "- total packet size:" + size;
            Logger::getLogger()->Log(s);
            return;
        }

        unsigned char *data;

        try
        {
            data = new unsigned char [dataSize];
            mLastReicevedData = new unsigned char [dataSize];
        }
        catch(std::bad_alloc& exc)
        {
            Logger::getLogger()->Log(exc.what());
        }

        if(data == NULL || mLastReicevedData == NULL)
            Logger::getLogger()->Log("Allocation memmory Failed");

        memcpy(data, packet + ID_LENGHT + RANDOM_BYTES_LENGTH + 4, dataSize);
        memcpy(mLastReicevedData, &packet[ID_LENGHT + RANDOM_BYTES_LENGTH + 4], dataSize);
        mLastReicevedDataSize = dataSize;

        int accept = 0;
        int i = 0;
        QByteArray bdata((const char*)data, dataSize);
        unsigned char testData[5];
        for(i = 0; i < 5; i++)
            testData[i] = 'a';
        switch(id)
        {
         case LOGIN_RESPONSE:
            setStatus(LOGIN_RESPONSE);
            if(mUsername.compare((const char*)data))
            {
                mLoggedToServer = true;
                Logger::getLogger()->Log("Client: "+ mUsername +" got successful LOGIN_RESPONSE from server");
            }
            break;
         case LOGOUT_RESPONSE:
            setStatus(LOGOUT_RESPONSE);
            if(mUsername.compare((const char*)data))
            {
                mLoggedToServer = false;
                Logger::getLogger()->Log("Client: "+ mUsername +" got successful LOGOUT_RESPONSE from server");
            }
            break;
        case GET_ONLINE_USER_LIST_REQUEST:
            setStatus(GET_ONLINE_USER_LIST_REQUEST);
            Logger::getLogger()->Log("Client: "+ mUsername +" got GET_ONLINE_USER_LIST_REQUEST --- ERROR");
            break;
        case GET_ONLINE_USER_LIST_RESPONSE:
            setStatus(GET_ONLINE_USER_LIST_RESPONSE);
            Logger::getLogger()->Log("Client: "+ mUsername +" got GET_ONLINE_USER_LIST_RESPONSE from server");
            processGetOnlineListResponse(data, dataSize);
            break;
        case SERVER_COMUNICATION_REQUEST:
            setStatus(SERVER_COMUNICATION_REQUEST);
            Logger::getLogger()->Log("Client: "+ mUsername +" got SERVER_COMUNICATION_REQUEST");
            acceptConnection(processServerCommunicationRequest(data,dataSize),&data[4]);
            mCrypto->startCtrCalculation(mAESkey,mAESIV);

            std::cout << "aesKey: ";
            for(int i = 0; i < 16; i++)
                std::cout << std::hex << static_cast<int>(mAESkey[i]);

            std::cout << std::endl << "aesIV: ";
            for(int i = 0; i < 16; i++)
                std::cout << std::hex << static_cast<int>(mAESIV[i]);
            std::cout << std::endl;

            break;
        case SERVER_COMUNICATION_RESPONSE:
            setStatus(SERVER_COMUNICATION_RESPONSE);
            Logger::getLogger()->Log("Client: "+ mUsername +" got SERVER_COMUNICATION_RESPONSE");
            accept = processServerCommunicationResponse(data,dataSize);
            //second client refuse conection
            if(!accept)
            {

            }
            //second client accept conection
            else
            {

               Logger::getLogger()->Log("Client: "+ mUsername +" ACCEPT CONNECTION - START CALC");
               mCrypto->computeHash(mAESkey,mAESkey,AES_KEY_LENGTH);
               mCrypto->startCtrCalculation(mAESkey,mAESIV);
               std::cout << "aesKey: ";
               for(int i = 0; i < 16; i++)
                   std::cout << std::hex << static_cast<int>(mAESkey[i]);

               std::cout << std::endl << "aesIV: ";
               for(int i = 0; i < 16; i++)
                   std::cout << std::hex << static_cast<int>(mAESIV[i]);
               std::cout << std::endl;
            }
            break;
        case CLIENT_COMMUNICATION_DATA:
            setStatus(CLIENT_COMMUNICATION_DATA);
            Logger::getLogger()->Log("Client: "+ mUsername +" Received UDP data: ");
            std::cout << "dataCyp: ";
            for(int i = 0; i < dataSize; i++)
                std::cout << std::hex << static_cast<int>(data[i]);
            std::cout << std::endl;

            std::cout << "aesKey: ";
            for(int i = 0; i < 16; i++)
                std::cout << std::hex << static_cast<int>(mAESkey[i]);

            std::cout << std::endl << "aesIV: ";
            for(int i = 0; i < 16; i++)
                std::cout << std::hex << static_cast<int>(mAESIV[i]);
            std::cout << std::endl;

            processServerCommunicationData(&data, dataSize);

            break;
         default:
             break;
        }
        delete[] data;
    }
    else
    {
        return;
    }
}

int Client::processServerCommunicationRequest(unsigned char *data, int size)
{
    int connectionID = 0;
    if(size > 3)
    {
        connectionID = ( data[3] << 24) | ( data[2] << 16) | (data[1] << 8) | ( data[0]);

    }
    std::cout << connectionID ;
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

        memcpy(mAESkey, data + 5, AES_KEY_LENGTH);
        memcpy(mAESIV, data + 5 + AES_KEY_LENGTH, AES_IV_LENGTH);

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

    for(int i = 0; i < mOnlineList.size();i++)
       std::cout << " [" << i << "] - " << mOnlineList[i]->getUsername() << std::endl;

    return 0;
}

int Client::processServerCommunicationData(unsigned char **data, int size)
{
    std::cout << "enter proces communication data" << std::endl;
    unsigned char *oldData;

    try
    {
        oldData = new unsigned char[size];
    }
    catch(std::bad_alloc& exc)
    {
        Logger::getLogger()->Log(exc.what());
    }


    if(oldData == NULL)
        Logger::getLogger()->Log("Allocation memmory Failed");

    memcpy(oldData, *data, size);
    int cypherPosition = 0;
    if(size > 3)
    {
        cypherPosition = ( oldData[3] << 24) | ( oldData[2] << 16) | (oldData[1] << 8) | ( oldData[0]);

    }
    if(cypherPosition < mCypherLastPositionreceived)
    {
         Logger::getLogger()->Log("Packet dropped");
        delete[] oldData;
        return 1;
    }

    mCypherLastPositionreceived = cypherPosition;
    delete[] *data;
    size -= sizeof(mCypherPosition);

    try
    {
        *data = new unsigned char[size];
    }
    catch(std::bad_alloc& exc)
    {
        Logger::getLogger()->Log(exc.what());
    }


    memcpy(*data,oldData + sizeof(mCypherPosition), size );

    unsigned char *stream = new unsigned char[size];

    if(stream == NULL)
        Logger::getLogger()->Log("Allocation memmory Failed");


    mCrypto->getDecKeystream(stream, size, mCypherLastPositionreceived);
    mCrypto->XORData(*data,*data,size, stream);

    std::cout << "data: ";
    for(int i = 0; i < size; i++)
        std::cout << (*data)[i];
    std::cout <<  std::endl;

     std::cout <<"stream: ";
    for(int i = 0; i < size; i++)
         std::cout << std::hex << static_cast<int>(stream[i]);
    std::cout << std::endl;

    delete[] stream;
    delete[] oldData;
    return 0;
}

int Client::processPacket(unsigned char* packet, unsigned char** data, int size)
{
    if(packet == NULL)
     return -1;

    int id = 0;
    int dataSize = 0;

    unsigned char* packethash;
    unsigned char* computedhash;

    try
    {
        packethash = new unsigned char[INTERGRITY_HASH_SIZE];
        computedhash = new unsigned char[INTERGRITY_HASH_SIZE];
    }
    catch(std::bad_alloc& exc)
    {
        Logger::getLogger()->Log(exc.what());
    }

    memcpy(packethash, packet + size - INTERGRITY_HASH_SIZE, INTERGRITY_HASH_SIZE);
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

        try
        {
            *data = new unsigned char [dataSize];
        }
        catch(std::bad_alloc& exc)
        {
            Logger::getLogger()->Log(exc.what());
        }


        memcpy(*data, &packet[ID_LENGHT + RANDOM_BYTES_LENGTH + 4], dataSize);
    }
    delete[] packethash;
    delete[] computedhash;
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

int Client::getStatus()
{
    return mStatus;
}

void Client::setStatus(int status)
{
    mStatus = status;
}
 void  Client::setCypherPosition(int pos)
 {
    mCypherPosition = pos;
 }
