
#ifndef UNIT_TEST

#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "Client.h"
#include "testserver.h"

const char IP_SERVER[] = "127.0.0.1";
const int PORT_SERVER = 13374;
/*
TEST_CASE("SERVER")
{
    SECTION("is not avaible")
    {
        NetworkManager manager;
        REQUIRE_FALSE(manager.startConnection(IP_SERVER, PORT_SERVER));
    }
    SECTION("is avaible")
	{
        TestServer* server = new TestServer(13374);
        NetworkManager manager;

        REQUIRE(manager.startConnection(IP_SERVER, PORT_SERVER));

        delete server;
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
*/
TEST_CASE("SENDING DATA")
{
    SECTION("Send data to myself and expect to receive same data")
	{
        const int DATA_LENGTH = 10;
        unsigned char* data = new unsigned char[DATA_LENGTH];
        unsigned char* data2 = NULL;
        int size = 0;


        TestServer* server = new TestServer(13374);
        std::cout << "Server Started" << std::endl;

        NetworkManager manager;
        manager.startConnection(IP_SERVER, PORT_SERVER);
        manager.sendData(data, DATA_LENGTH);

        data2 = server->getData();

        REQUIRE(size == DATA_LENGTH);

        if(size == DATA_LENGTH)
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
        delete data2;
        delete server;
	}
}
/*
TEST_CASE("RECIEVING DATA")
{
	SECTION("login succesful")
	{
        Client* client = new Client("test", "test@test");

        TestServer* server = new TestServer(13374);

        client->login();

        REQUIRE(client->isLogged());
	}
	SECTION("login not succesful")
	{
        Client* client = new Client("", "test@test");

        TestServer* server = new TestServer(13374);

        client->login();

        REQUIRE_FALSE(client->isLogged());
	}
    SECTION("Denied to send request list of users when not logged in")
	{
        Client c;
        REQUIRE_FALSE(c.getOnlineList() == 0);
	}
	SECTION("request list of users when logged in")
	{
        unsigned char tmp[] = "pepa:175.168.2.1;jarda:186.2.2.2;";
        const int DATA_LENGTH = sizeof(tmp);
        unsigned char* data = new unsigned char[DATA_LENGTH];
        unsigned char* data2 = NULL;
        unsigned char* packet = NULL;

        memcpy(data, tmp, DATA_LENGTH);

        Client c;
        int size = c.createPacket(GET_ONLINE_USER_LIST_RESPONSE, data, &packet, DATA_LENGTH);

        TestServer* server = new TestServer(13374);

        NetworkManager manager;
        manager.startConnection(IP_SERVER, PORT_SERVER);
        manager.sendData(packet, size);

        int size2 = c.processPacket(packet, &data2);
        std::cout << DATA_LENGTH << std::endl;
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
*/
#endif
