#include   "Client.h"

Client::Client() : mUsername(), mEmail(), mLoggedToServer(false), mConnectedToClient(false)
{

}

Client::Client(string username, string email) : mUsername(username), mEmail(email), mLoggedToServer(false), mConnectedToClient(false)
{

}
Client::~Client()
{

}

int Client::login()
{
    unsigned char *data = NULL;
    int dataSize = mUsername.size() + mEmail.size();
    data = new unsigned char [dataSize];
    //printf("dataSize: %d \n", dataSize);

    memcpy(data, mUsername.c_str(), mUsername.size());
    memcpy(&data[mUsername.size()], mEmail.c_str(), mEmail.size());

    unsigned char *packet = NULL;
    int packetSize = this->createPacket(LOGIN_REQUEST,data,&packet,dataSize);

   /* printf("packet size %d \n", packetSize);
    for(int i = 0; i < packetSize; i++)
       printf("%c",packet[i]);*/

    //bude nasledovat sifrovani a poslani pres sit

    mLoggedToServer = true;
    delete[] data;
    return 0;
}
int Client::logout()
{
    if(!mLoggedToServer)
        return 1;

    unsigned char *data = NULL;
    int dataSize = mUsername.size() + mEmail.size();
    data = new unsigned char [dataSize];
    printf("dataSize: %d \n", dataSize);

    memcpy(data, mUsername.c_str(), mUsername.size());
    memcpy(&data[mUsername.size()], mEmail.c_str(), mEmail.size());

    unsigned char *packet = NULL;
    int packetSize = this->createPacket(LOGOUT_REQUEST,data,&packet,dataSize);


    printf("packet size %d \n", packetSize);
    for(int i = 0; i < packetSize; i++)
       printf("%c",packet[i]);

    //bude nasledovat sifrovani a poslani pres sit

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
    printf("dataSize: %d \n", dataSize);

    memcpy(data, mUsername.c_str(), mUsername.size());

    unsigned char *packet = NULL;
    int packetSize = this->createPacket(GET_ONLINE_USER_LIST_REQUEST,data,&packet,dataSize);


    printf("packet size %d \n", packetSize);
    for(int i = 0; i < packetSize; i++)
       printf("%c",packet[i]);

    //bude nasledovat sifrovani a poslani pres sit

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

        /* inverzni operace byte[4] to int
        int pom = ( (*packet)[ID_LENGHT + RANDOM_BYTES_LENGTH + 3] << 24) | ( (*packet)[ ID_LENGHT + RANDOM_BYTES_LENGTH +2] << 16) | ( (*packet)[ID_LENGHT + RANDOM_BYTES_LENGTH + 1] << 8) | ( (*packet)[ID_LENGHT + RANDOM_BYTES_LENGTH]);
          printf("sizePom: %d \n", pom);*/
    }

    memcpy(&((*packet)[ID_LENGHT + RANDOM_BYTES_LENGTH + sizeof(size)]), data, size);

    return newSize;
}

int Client::processPacket(unsigned char* packet, unsigned char** data)
{
    int id = 0;
    int dataSize = 0;
    //desifrovani zkontrolovani hashu atd. tady bude
    if(sizeof(int) == 4)
    {
        id = packet[0];
        dataSize = ( packet[ID_LENGHT + RANDOM_BYTES_LENGTH + 3] << 24) | ( packet[ ID_LENGHT + RANDOM_BYTES_LENGTH +2] << 16) | (packet[ID_LENGHT + RANDOM_BYTES_LENGTH + 1] << 8) | ( packet[ID_LENGHT + RANDOM_BYTES_LENGTH]);
        *data = new unsigned char [dataSize];
        memcpy(*data, &packet[ID_LENGHT + RANDOM_BYTES_LENGTH + 4], dataSize);

        switch(id)
        {
         case LOGIN_REQUEST:
              break;
         case LOGIN_RESPONSE:
            mLoggedToServer = true;
              break;
         case LOGOUT_REQUEST:
             break;
         case LOGOUT_RESPONSE:
            mLoggedToServer = false;
             break;
        case GET_ONLINE_USER_LIST_REQUEST:
            break;
        case GET_ONLINE_USER_LIST_RESPONSE:
            break;
         default:
             break;

        }
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
