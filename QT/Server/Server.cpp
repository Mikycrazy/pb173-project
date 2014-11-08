#include "Server.h"

bool Server::loginUser(User* user)
{
    mUsers.push_back(user);

    unsigned char *data = NULL;
    int dataSize = user->getUsername().size();
    data = new unsigned char [dataSize];
    //printf("dataSize: %d \n", dataSize);

    memcpy(data, user->getUsername().c_str(), user->getUsername().size());

    unsigned char *packet = NULL;
    int packetSize = this->createPacket(LOGIN_RESPONSE,data,&packet,dataSize);

    //bude nasledovat sifrovani a poslani pres sit

    delete[] data;

    return true;
}
bool Server::logoutUser(User* user)
{
    for(unsigned int i = 0; i < mUsers.size(); i++)
    {
         if(user->getUsername().compare(mUsers[i]->getUsername()))
         {
             mUsers.erase(mUsers.begin() + i);

             unsigned char *data = NULL;
             int dataSize = user->getUsername().size();
             data = new unsigned char [dataSize];
             //printf("dataSize: %d \n", dataSize);

             memcpy(data, user->getUsername().c_str(), user->getUsername().size());

             unsigned char *packet = NULL;
             int packetSize = this->createPacket(LOGOUT_RESPONSE,data,&packet,dataSize);

             //bude nasledovat sifrovani a poslani pres sit

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
        s += mUsers[i]->getIPAddress() + ";";
    }

    int dataSize = s.size();
    data = new unsigned char [dataSize];
    memcpy(data, s.c_str(), s.size());

    unsigned char *packet = NULL;
    int packetSize = this->createPacket(GET_ONLINE_USER_LIST_RESPONSE,data,&packet,dataSize);
}

int Server::createPacket(unsigned char id, unsigned char *data, unsigned char **packet, int size)
{
    int newSize = ID_LENGHT + RANDOM_BYTES_LENGTH + sizeof(size) + size;
    *packet = new unsigned char[newSize];
    //casem pribude hash

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
    memcpy(&((*packet)[ID_LENGHT + RANDOM_BYTES_LENGTH + sizeof(size)]),data, size);

    return newSize;
}

int Server::processPacket(unsigned char* packet, unsigned char** data)
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
              break;
         case LOGOUT_REQUEST:
             break;
         case LOGOUT_RESPONSE:
             break;
        case  GET_ONLINE_USER_LIST_REQUEST:
            break;
        case GET_ONLINE_USER_LIST_RESPONSE:
            break;
         default:
             break;

        }
    }
    return dataSize;

}
