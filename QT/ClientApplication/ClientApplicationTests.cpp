#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "Client.h"

TEST_CASE("SERVER")
{

	SECTION("is avaible")
	{

	}
	SECTION("is not avaible")
	{

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
        REQUIRE(strcmp(reinterpret_cast<const char*>(data), reinterpret_cast<const char*>(data2)) == 0);

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
        REQUIRE(strcmp(reinterpret_cast<const char*>(data), reinterpret_cast<const char*>(data2)) == 0);

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
        REQUIRE(strcmp(reinterpret_cast<const char*>(data), reinterpret_cast<const char*>(data2)) == 0);

        delete data;
        delete packet;
        delete data2;
    }
	SECTION("get online list packet")
	{

	}
}

TEST_CASE("SENDING DATA")
{
	SECTION("send data match")
	{

	}
	SECTION("send data dont match")
	{

	}
}
TEST_CASE("RECIEVING DATA")
{
	SECTION("login succesful")
	{

	}
	SECTION("login not succesful")
	{

	}
	SECTION("request list of users when not logged in")
	{

	}
	SECTION("request list of users when logged in")
	{

	}
}
