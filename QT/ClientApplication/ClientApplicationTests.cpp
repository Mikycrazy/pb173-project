#define UNIT_TEST
#ifndef UNIT_TEST

#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "Client.h"
#include "myserver.h"


const char IP_SERVER[] = "127.0.0.1";
const int PORT_SERVER = 13374;


TEST_CASE("SERVER")
{

    SECTION("is not avaible")
    {
        NetworkManager manager;
        REQUIRE_FALSE(manager.startConnection(IP_SERVER, PORT_SERVER));
    }
    SECTION("is avaible")
    {
        MyServer server;

        NetworkManager manager;
        REQUIRE(manager.startConnection(IP_SERVER, PORT_SERVER));

        server.Stop();
    }

}
TEST_CASE("PACKET CREATING")
{
    Client c;
    SECTION("Create login packet with 'aaaaaaaaaa' data ")
	{
        const int DATA_LENGTH = 10;
        unsigned char* data = new unsigned char[DATA_LENGTH];
        unsigned char* packet = NULL;
        unsigned char* data2 = NULL;

        memset(data, 97, DATA_LENGTH);

        int size = c.createPacket(LOGIN_REQUEST, data, &packet, DATA_LENGTH);
        int size2 = c.processPacket(packet, &data2);

        REQUIRE(size == DATA_LENGTH + 15);
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
    SECTION("Create login packet with zero data ")
    {
        const int DATA_LENGTH = 0;
        unsigned char* data = NULL;
        unsigned char* packet = NULL;
        unsigned char* data2 = NULL;

        int size = c.createPacket(LOGIN_REQUEST, data, &packet, DATA_LENGTH);
        int size2 = c.processPacket(packet, &data2);

        REQUIRE(size == DATA_LENGTH + 15);
        REQUIRE(size2 == DATA_LENGTH);

        delete data;
        delete packet;
        delete data2;
    }
    SECTION("Create logout packet with 'bbbbb' data")
	{
        const int DATA_LENGTH = 5;
        unsigned char* data = new unsigned char[DATA_LENGTH];
        unsigned char* packet = NULL;
        unsigned char* data2 = NULL;

        memset(data, 97, DATA_LENGTH);

        int size = c.createPacket(LOGOUT_REQUEST, data, &packet, DATA_LENGTH);
        int size2 = c.processPacket(packet, &data2);

        REQUIRE(size == DATA_LENGTH + 15);
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
    SECTION("Create logout packet with long data")
    {
        const int DATA_LENGTH = 1024;
        unsigned char* data = new unsigned char[DATA_LENGTH];
        unsigned char* packet = NULL;
        unsigned char* data2 = NULL;

        memset(data, 97, DATA_LENGTH);

        int size = c.createPacket(LOGOUT_REQUEST, data, &packet, DATA_LENGTH);
        int size2 = c.processPacket(packet, &data2);

        REQUIRE(size == DATA_LENGTH + 15);
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
    SECTION("Create get online list packet")
	{
        const int DATA_LENGTH = 10;
        unsigned char* data = new unsigned char[DATA_LENGTH];
        unsigned char* packet = NULL;
        unsigned char* data2 = NULL;

        memset(data, 97, DATA_LENGTH);

        int size = c.createPacket(GET_ONLINE_USER_LIST_REQUEST, data, &packet, DATA_LENGTH);
        int size2 = c.processPacket(packet, &data2);

        REQUIRE(size == DATA_LENGTH + 15);
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

TEST_CASE("SENDING DATA")
{
    SECTION("Send data to myself and expect to receive same data")
	{
        const int DATA_LENGTH = 10;
        unsigned char* data = new unsigned char[DATA_LENGTH];

        memset(data, 97, DATA_LENGTH);

        MyServer server;

        Client c("asdf","asdf");
        c.sendData(data, DATA_LENGTH);

        QByteArray data2 = server.getLastData();

        REQUIRE(data2.size() == DATA_LENGTH);

        if(data2.size() == DATA_LENGTH)
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
	}
}

TEST_CASE("RECIEVING DATA")
{
    MyServer server;
    SECTION("login succesful")
	{
        const int DATA_LENGTH = 10;
        unsigned char* data = new unsigned char[DATA_LENGTH];
        unsigned char* data2 = NULL;
        unsigned char* packet = NULL;

        for(int i = 0; i < DATA_LENGTH; i++)
            data[i] = char(i);

        Client c("asdf","asdf");
        c.createPacket(LOGIN_RESPONSE, data, &packet, DATA_LENGTH);
        c.sendData(packet, DATA_LENGTH);

        data2 = c.getLastData();
        int size = c.getLastDataSize();

        REQUIRE(size == DATA_LENGTH);

        if(size == DATA_LENGTH)
        {
            bool match = true;
            for(int i = 0; i < DATA_LENGTH; i++)
                if(data[i] != char((DATA_LENGTH - i) - 1))
                    match = false;

            REQUIRE(match);
        }
    }
	SECTION("login not succesful")
	{
        const int DATA_LENGTH = 10;
        unsigned char* data = new unsigned char[DATA_LENGTH];

        char failed[] = "Fail!";

        unsigned char* data2 = NULL;

        memset(data, 97, DATA_LENGTH);

        Client c("asdf","asdf");
        c.sendData(data, DATA_LENGTH);

        data2 = c.getLastData();
        int size = c.getLastDataSize();

        REQUIRE_FALSE(size == DATA_LENGTH);

        if(size == 5)
        {
            bool match = true;
            for(int i = 0; i < DATA_LENGTH; i++)
                if(data[i] != failed[i])
                    match = false;

            REQUIRE(match);
        }
	}
	SECTION("request list of users when logged in")
	{
        unsigned char tmp[] = "pepa:175.168.2.1;jarda:186.2.2.2;";
        const int DATA_LENGTH = sizeof(tmp);
        unsigned char* data = new unsigned char[DATA_LENGTH];
        unsigned char* data2 = NULL;
        unsigned char* packet = NULL;

        memcpy(data, tmp, DATA_LENGTH);

        Client c("asdf","asdf");
        int size = c.createPacket(GET_ONLINE_USER_LIST_RESPONSE, data, &packet, DATA_LENGTH);
        c.sendData(packet, size);

        int size2 = c.getLastDataSize();
        data2 = c.getLastData();

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
	}
}


TEST_CASE("CLIENT TO CLIENT")
{

    SECTION("Send data to client")
    {
        Client* client = new Client("test", "test@test");
        unsigned char* data2 = NULL;
        QHostAddress address("127.0.0.1");
        unsigned char data[5] = {'a', 'b', 'c', 'd', 'e'};

        client->sendDataToClient(address, 13375, data, 5);

        int size2 = client->getLastDataSize();
        data2 = client->getLastData();

        REQUIRE(size2 == 5);

        if(size2 == 5)
        {
            bool match = true;
            for(int i = 0; i < 5; i++)
            {
                if(data[i] != data2[i])
                    match = false;
            }

            REQUIRE(match);
        }
    }
    SECTION("Send ecrypted data to client")
    {

    }
}

#endif
