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
	SECTION("login packet")
	{
        unsigned char* data = new unsigned char[10];
        unsigned char* packet = NULL;
        memset(data, 97, 10);
        int size = c.packetCreator(LOGIN_REQUEST, data, &packet, 10);

        REQUIRE(size == 25);
	}
	SECTION("logout packet")
	{

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
