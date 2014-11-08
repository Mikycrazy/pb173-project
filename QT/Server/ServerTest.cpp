#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "Server.h"

TEST_CASE("LOGGING")
{

	SECTION("log in")
	{

	}

	SECTION("log out")
	{

	}

	SECTION("get online list")
	{

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
        int size2 = s.processPacket(packet, &data2);

        REQUIRE(size == DATA_LENGTH + 15);
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
        int size2 = s.processPacket(packet, &data2);

        REQUIRE(size == DATA_LENGTH + 15);
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
        int size2 = s.processPacket(packet, &data2);

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
	SECTION("send data match")
	{

	}
	SECTION("send data dont match")
	{

	}
}
TEST_CASE("RECIEVING DATA")
{
	SECTION("login request succesful - change mUsers")
	{

	}
	SECTION("login request not succesful - mUsers not changed")
	{

	}
	SECTION("request list of users when not logged in - request denied")
	{

	}
	SECTION("request list of users when logged in - response send")
	{

	}
}
TEST_CASE("LOGIN/LOGOUT USERS")
{
	int userCount;
	SECTION("add first user")
	{
		
	}
	SECTION("add more users")
	{
		
	}
	SECTION("remove user")
	{

	}
	SECTION("remove all users")
	{

	}
	SECTION("remove from empy users list")
	{

	}
}
