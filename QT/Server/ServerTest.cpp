//#define UNIT_TEST
#ifdef UNIT_TEST
#define CATCH_CONFIG_MAIN
#include <vector>
#include "catch.hpp"
#include "Server.h"
#include "mytcpclient.h"

const char IP_SERVER[] = "127.0.0.1";
const int PORT_SERVER = 13374;

std::string getLastLine(std::ifstream& in)
{
    std::string line;
    while (in >> std::ws && std::getline(in, line));

    return line;
}

TEST_CASE("LOGGING")
{
    Server server;
    SECTION("log in")
    {

        server.loginUser(new User("user", "user", "127.0.0.1", NULL, -1));
        std::string line;
        std::ifstream file;
        file.open("log.txt");
        if (file)
        {
            line = getLastLine(file);

        }
        else
            std::cout << "Unable to open file.\n";

        REQUIRE(line.length() > 0);
    }
    SECTION("log out")
    {
        User* user = new User("user", "user", "127.0.0.1", NULL, -1);
        server.loginUser(user);

        server.logoutUser(user);


        std::ifstream file;
        file.open("log.txt");
        std::string line;
        if (file)
        {
            line = getLastLine(file);
        }
        else
            std::cout << "Unable to open file.\n";

        REQUIRE(line.length() > 0);
    }
}

TEST_CASE("PACKET CREATING")
{
    Server s;
    SECTION("Create login response packet ")
    {
        const int DATA_LENGTH = 1;
        unsigned char* data = new unsigned char[DATA_LENGTH];
        unsigned char* packet = NULL;
        unsigned char* data2 = NULL;

        memset(data, 84, DATA_LENGTH);

        int size = s.createPacket(LOGIN_RESPONSE, data, &packet, DATA_LENGTH);
        int size2 = s.processPacket(packet, &data2, size);

        REQUIRE(size == DATA_LENGTH + ID_LENGHT + RANDOM_BYTES_LENGTH + 4 + INTERGRITY_HASH_SIZE);
        REQUIRE(size2 == DATA_LENGTH);

        if(size2 == 1)
        {
            REQUIRE(data2[0] == 84);
        }

        delete data;
        delete packet;
        delete data2;
    }
    SECTION("logout response packet")
    {
        const int DATA_LENGTH = 1;
        unsigned char* data = new unsigned char[DATA_LENGTH];
        unsigned char* packet = NULL;
        unsigned char* data2 = NULL;

        memset(data, 84, DATA_LENGTH);

        int size = s.createPacket(LOGOUT_RESPONSE, data, &packet, DATA_LENGTH);
        int size2 = s.processPacket(packet, &data2, size);

        REQUIRE(size == DATA_LENGTH + ID_LENGHT + RANDOM_BYTES_LENGTH + 4 + INTERGRITY_HASH_SIZE);
        REQUIRE(size2 == DATA_LENGTH);

        if(size2 == 1)
        {
            REQUIRE(data2[0] == 84);
        }

        delete data;
        delete packet;
        delete data2;
    }
    SECTION("get online list response packet")
    {
        const int DATA_LENGTH = 10;
        unsigned char* data = new unsigned char[DATA_LENGTH];
        unsigned char* packet = NULL;
        unsigned char* data2 = NULL;

        memset(data, 97, DATA_LENGTH);

        int size = s.createPacket(GET_ONLINE_USER_LIST_REQUEST, data, &packet, DATA_LENGTH);
        int size2 = s.processPacket(packet, &data2, size);

        REQUIRE(size == DATA_LENGTH + ID_LENGHT + RANDOM_BYTES_LENGTH + 4 + INTERGRITY_HASH_SIZE);
        REQUIRE(size2 == DATA_LENGTH);

        if(size2 == DATA_LENGTH)
        {
            bool match = true;
            for(int i = 0; i < DATA_LENGTH; i++)
            {
                if(data[i] != data2[i])
                    match = false;
            }

            REQUIRE(match);
        }

        delete data;
        delete packet;
        delete data2;
    }
}

TEST_CASE("RESPONDING")
{
    SECTION("Server is responding, user is able to connect")
    {
        Server server(PORT_SERVER + 1);

        QTcpSocket* socket = new QTcpSocket();
        socket->connectToHost(IP_SERVER, PORT_SERVER + 1);

        REQUIRE(socket->waitForConnected(10000));


    }
    SECTION("Server is responding, user is able to connect")
    {
        QTcpSocket* socket = new QTcpSocket();
        socket->connectToHost(IP_SERVER, PORT_SERVER + 3);

        REQUIRE_FALSE(socket->waitForConnected(10000));
    }
    SECTION("Server is responding, multiple clients connected")
    {
        Server server(PORT_SERVER + 4);

        vector<MyTcpClient*> clients;

        for(int i = 0; i < 10;i++)
            clients.push_back(new MyTcpClient());

        bool connected = true;
        for(int i = 0; i < 10;i++)
        {
            connected = clients.at(i)->startConnection(IP_SERVER, PORT_SERVER + 4);
            if(!connected)
                break;
        }

        REQUIRE(connected);

        for(int i = 0; i < 10;i++)
            delete clients.at(i);
    }
}

TEST_CASE("PROSSESSING RECEIVED DATA")
{
    Server server(PORT_SERVER + 5);

    SECTION("login request succesful - change mUsers")
    {
        int countbefore = server.getUsers().size();

        User* user = new User("test", "test@test.com", "127.0.0.1", NULL, -1);
        unsigned char *data = NULL;
        int dataSize = user->getUsername().size() +user->getEmail().size() + DATA_SPLITER.size();
        data = new unsigned char [dataSize];

        memcpy(data, user->getUsername().c_str(), user->getUsername().size());
        memcpy(&data[user->getUsername().size()], DATA_SPLITER.c_str(), DATA_SPLITER.size());
        memcpy(&data[user->getUsername().size() + DATA_SPLITER.size()], user->getEmail().c_str(), user->getEmail().size());

        unsigned char *packet = NULL;

        int size = server.createPacket(LOGIN_REQUEST, data, &packet, dataSize);


        MyTcpClient client;
        client.startConnection(IP_SERVER, PORT_SERVER + 5);
        client.sendData(packet, size);

        int countafter = server.getUsers().size();

        REQUIRE(countbefore != countafter);

        delete user;
        delete data;


    }
    SECTION("login request not succesful - mUsers not changed")
    {
        int countbefore = server.getUsers().size();

        const int DATA_LENGTH = 10;
        unsigned char* data = new unsigned char[DATA_LENGTH];

        memset(data, 97, DATA_LENGTH);

        MyTcpClient client;
        client.startConnection(IP_SERVER, PORT_SERVER + 5);
        client.sendData(data, DATA_LENGTH);

        int countafter = server.getUsers().size();

        REQUIRE(countbefore == countafter);

        delete data;
    }
    SECTION("request list of users when not logged in - request denied")
    {
        User* user = new User("test", "test@test.com", "127.0.0.1", NULL, -1);
        server.loginUser(user);

        unsigned char *data = NULL;
        int dataSize = user->getUsername().size();
        data = new unsigned char [dataSize];

        memcpy(data, user->getUsername().c_str(), user->getUsername().size());

        unsigned char *packet = NULL;
        int getpacketSize = server.createPacket(GET_ONLINE_USER_LIST_REQUEST,data,&packet,dataSize);


        MyTcpClient client;
        client.startConnection(IP_SERVER, PORT_SERVER + 5);
        client.sendData(packet, getpacketSize);

        REQUIRE(client.getLastDataSize() == 5);

        delete user;
        delete data;

    }
    SECTION("request list of users when logged in - response send")
    {
        User* user = new User("test", "test@test.com", "127.0.0.1", NULL, -1);
        server.loginUser(user);

        unsigned char *data = NULL;
        int dataSize = user->getUsername().size();
        data = new unsigned char [dataSize];

        memcpy(data, user->getUsername().c_str(), user->getUsername().size());

        unsigned char *packet = NULL;
        int getpacketSize = server.createPacket(GET_ONLINE_USER_LIST_REQUEST,data,&packet,dataSize);


        MyTcpClient client;
        client.startConnection(IP_SERVER, PORT_SERVER + 5);
        client.sendData(packet, getpacketSize);

        REQUIRE(client.getLastDataType() == GET_ONLINE_USER_LIST_RESPONSE);

        delete user;
        delete data;
    }
}

TEST_CASE("LOGIN/LOGOUT USERS")
{
    int userCount = 0;
    Server server;
    User* user = new User("asdf", "asdf", "127.0.0.1", NULL, -1);
    SECTION("add first user")
    {
        server.loginUser(user);
        int size = server.getUsers().size();
        userCount++;

        REQUIRE(size == userCount);
    }
    SECTION("add more users")
    {
        for(int i = 0; i < 5; i++)
            server.loginUser(user);

        userCount += 5;
        int size = server.getUsers().size();
        REQUIRE(size == userCount);
    }
    SECTION("remove user")
    {
        server.logoutUser(user);

        int size = server.getUsers().size();
        REQUIRE(size == userCount);
    }
    SECTION("remove 5 users")
    {
        for(int i = 0; i < 5; i++)
            server.logoutUser(user);

        int size = server.getUsers().size();
        userCount -= 5;
        REQUIRE(size == userCount);
    }

    delete user;
}

TEST_CASE("LOGIN/LOGOUT USERS and through NETWORK and GETTING ONLINE LIST")
{
    Server server(PORT_SERVER + 15);

    SECTION("Login one user and get online list")
    {
        const int DATA_LENGTH = 10;
        unsigned char* data = new unsigned char[DATA_LENGTH];
        unsigned char* data2 = NULL;
        memset(data, 97, DATA_LENGTH);

        unsigned char *loginpacket = NULL;
        int loginpacketSize = server.createPacket(LOGIN_REQUEST,data,&loginpacket,DATA_LENGTH);

        unsigned char *getpacket = NULL;
        int getpacketSize = server.createPacket(GET_ONLINE_USER_LIST_REQUEST,data,&getpacket,DATA_LENGTH);

        MyTcpClient client;
        client.startConnection(IP_SERVER, PORT_SERVER + 15);
        client.sendData(loginpacket, loginpacketSize);
        client.sendData(getpacket, getpacketSize);

        data2 = client.getLastData();

        REQUIRE(client.getLastDataType() == GET_ONLINE_USER_LIST_RESPONSE);
        REQUIRE(client.getLastDataSize() > 0);

        if(client.getLastDataSize() > 0)
        {
            int count = 0;
            for(int i = 0; i < client.getLastDataSize(); i++)
            {
                if(data2[i] == ';')
                    count++;
            }

            REQUIRE(count == 1);
        }

        delete data;
        delete loginpacket;
        delete getpacket;

    }
    SECTION("Login more users and get online list")
    {
        User* user = new User("test", "test@test.com", "127.0.0.1", NULL, -1);
        for(int i = 0; i < 5; i++)
            server.loginUser(user);

        const int DATA_LENGTH = 10;
        unsigned char* data = new unsigned char[DATA_LENGTH];
        unsigned char* data2 = NULL;
        memset(data, 97, DATA_LENGTH);

        unsigned char *getpacket = NULL;
        int getpacketSize = server.createPacket(GET_ONLINE_USER_LIST_REQUEST,data,&getpacket,DATA_LENGTH);

        MyTcpClient client;
        client.startConnection(IP_SERVER, PORT_SERVER + 15);
        client.sendData(getpacket, getpacketSize);

        data2 = client.getLastData();

        REQUIRE(client.getLastDataType() == GET_ONLINE_USER_LIST_RESPONSE);
        REQUIRE(client.getLastDataSize() > 0);

        if(client.getLastDataSize() > 0)
        {
            int count = 0;
            for(int i = 0; i < client.getLastDataSize(); i++)
            {
                if(data2[i] == ';')
                    count++;
            }

            REQUIRE(count == 5);
        }

        delete data;
        delete getpacket;

    }
}

TEST_CASE("CLIENT TO CLIENT")
{
    Server server(PORT_SERVER + 20);
    SECTION("Resending request packet to client right")
    {
        const int DATA_LENGTH = 10;
        unsigned char* data1 = new unsigned char[DATA_LENGTH];
        unsigned char* data2 = new unsigned char[DATA_LENGTH];

        memset(data1, 97, DATA_LENGTH);
        memset(data2, 98, DATA_LENGTH);

        unsigned char *loginpacket1 = NULL;
        int loginpacket1Size = server.createPacket(LOGIN_REQUEST,data1,&loginpacket1,DATA_LENGTH);

        unsigned char *loginpacket2 = NULL;
        int loginpacket2Size = server.createPacket(LOGIN_REQUEST,data2,&loginpacket2,DATA_LENGTH);

        MyTcpClient client1;
        client1.startConnection(IP_SERVER, PORT_SERVER + 20);

        MyTcpClient client2;
        client2.startConnection(IP_SERVER, PORT_SERVER + 20);

        client1.sendData(loginpacket1, loginpacket1Size);
        client2.sendData(loginpacket2, loginpacket2Size);

        unsigned char *compacket = NULL;
        int compacketSize = server.createPacket(CLIENT_COMUNICATION_REQUEST,data2,&compacket,DATA_LENGTH);

        client1.sendData(compacket, compacketSize);

        REQUIRE(client2.getLastDataType() == SERVER_COMUNICATION_REQUEST);

        delete data1;
        delete data2;
    }

    SECTION("Resending response packet to client right")
    {
        const int DATA_LENGTH = 10;
        unsigned char* data1 = new unsigned char[DATA_LENGTH];
        unsigned char* data2 = new unsigned char[DATA_LENGTH];

        memset(data1, 97, DATA_LENGTH);
        memset(data2, 98, DATA_LENGTH);

        unsigned char *loginpacket1 = NULL;
        int loginpacket1Size = server.createPacket(LOGIN_REQUEST,data1,&loginpacket1,DATA_LENGTH);

        unsigned char *loginpacket2 = NULL;
        int loginpacket2Size = server.createPacket(LOGIN_REQUEST,data2,&loginpacket2,DATA_LENGTH);

        MyTcpClient client1;
        client1.startConnection(IP_SERVER, PORT_SERVER + 20);

        MyTcpClient client2;
        client2.startConnection(IP_SERVER, PORT_SERVER + 20);

        client1.sendData(loginpacket1, loginpacket1Size);
        client2.sendData(loginpacket2, loginpacket2Size);

        unsigned char *compacket = NULL;
        int compacketSize = server.createPacket(CLIENT_COMUNICATION_RESPONSE,data1,&compacket,DATA_LENGTH);

        client1.sendData(compacket, compacketSize);

        REQUIRE(client1.getLastDataType() == SERVER_COMUNICATION_RESPONSE);

        delete data1;
        delete data2;
    }
}

#endif
