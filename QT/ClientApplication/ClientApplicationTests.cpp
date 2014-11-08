#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "Client.h"

TEST_CASE("SERVER")
{

	SECTION("is avaible")
	{
        unsigned char* packet = NULL;
        unsigned char* data2 = NULL;

        Client c("pepa", "pepa@seznam.cz");
        c.login();

        NetworkManager receiver;

        receiver.startListening(8888);
        receiver.acceptConnection();
        receiver.receiveData(1, packet);
        int size = c.processPacket(packet, &data2);

        REQUIRE(size == 1);

        if(size == 1)
        {
            REQUIRE(data2[0] == 1);
        }

        REQUIRE(c.isLogged());
	}
	SECTION("is not avaible")
	{
        Client c("pepa", "pepa@seznam.cz");
        REQUIRE_FALSE(c.login());

        REQUIRE_FALSE(c.isLogged());
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
        unsigned char* data2 = NULL;

        NetworkManager sender;

        sender.startConnection("127.0.0.1", 8888);
        sender.sendData(1, data, DATA_LENGTH);

        NetworkManager receiver;

        receiver.startListening(8888);
        receiver.acceptConnection();
        int size = receiver.receiveData(1, data2);

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
	}
}

TEST_CASE("RECIEVING DATA")
{
	SECTION("login succesful")
	{
        const int DATA_LENGTH = 1;
        unsigned char* data = new unsigned char[DATA_LENGTH];
        unsigned char* data2 = NULL;
        unsigned char* packet = NULL;

        Client c;
        c.createPacket(LOGIN_RESPONSE, data, &packet, DATA_LENGTH);

        NetworkManager server;

        server.startConnection("127.0.0.1", 8888);
        server.sendData(1, data, DATA_LENGTH);


        NetworkManager receiver;

        receiver.startListening(8888);
        receiver.acceptConnection();
        receiver.receiveData(1, packet);

        int size = c.processPacket(packet, &data2);
        REQUIRE(size == 1);

        if(size == 1)
        {
            REQUIRE(data2[0] == 1);
        }
	}
	SECTION("login not succesful")
	{
        const int DATA_LENGTH = 1;
        unsigned char* data = new unsigned char[DATA_LENGTH];
        unsigned char* data2 = NULL;
        unsigned char* packet = NULL;

        memset(data, 70, DATA_LENGTH);

        Client c;
        c.createPacket(LOGIN_RESPONSE, data, &packet, DATA_LENGTH);

        NetworkManager server;

        server.startConnection("127.0.0.1", 8888);
        server.sendData(1, data, DATA_LENGTH);


        NetworkManager receiver;

        receiver.startListening(8888);
        receiver.acceptConnection();
        receiver.receiveData(1, packet);

        int size = c.processPacket(packet, &data2);
        REQUIRE(size == 1);

        if(size == 1)
        {
            REQUIRE(data2[0] == 1);
        }
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
        c.createPacket(GET_ONLINE_USER_LIST_RESPONSE, data, &packet, DATA_LENGTH);

        NetworkManager server;

        server.startConnection("127.0.0.1", 8888);
        server.sendData(1, data, DATA_LENGTH);


        NetworkManager receiver;

        receiver.startListening(8888);
        receiver.acceptConnection();
        receiver.receiveData(1, packet);

        int size = c.processPacket(packet, &data2);

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
	}
}

