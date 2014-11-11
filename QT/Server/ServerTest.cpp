

#ifndef UNIT_TEST
#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "Server.h"

const char IP_SERVER[] = "127.0.0.1";
const int PORT_SERVER = 13374;

std::string getLastLine(std::ifstream& in)
{
    std::string line;
    while (in >> std::ws && std::getline(in, line))
        ;

    return line;
}

TEST_CASE("LOGGING")
{

	SECTION("log in")
	{
        Server *server = new Server(13374);
        server->loginUser(new User("user", "user", "127.0.0.1", NULL, -1));


        std::ifstream file("log.txt");
        std::string line = getLastLine(file);
        if (file)
        {

        }
        else
            std::cout << "Unable to open file.\n";

        //char* ptr = strstr(line, "LOGIN");
        REQUIRE(line.length() > 0);
        //REQUIRE(ptr != NULL);
	}

	SECTION("log out")
	{
        Server *server = new Server(13374);
        User* user = new User("asdf", "asdf", "127.0.0.1", NULL, -1);

        server->loginUser(user);
        server->logoutUser(user);


        std::ifstream file("log.txt");
        std::string line = getLastLine(file);
        if (file)
        {

        }
        else
            std::cout << "Unable to open file.\n";

        //char* ptr = strstr(line, "LOGOUT");
        REQUIRE(line.length() > 0);
        //REQUIRE(ptr != NULL);
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
        const int DATA_LENGTH = 10;
        unsigned char* data = new unsigned char[DATA_LENGTH];
        unsigned char* data2 = NULL;
        int size = 0;


        Server* server = new Server(13374);
        std::cout << "Server Started" << std::endl;

        /*NetworkManager manager;
        manager.startConnection(IP_SERVER, PORT_SERVER);
        manager.sendData(data, DATA_LENGTH);*/

        //Dostat data ze serveru která přisla

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
TEST_CASE("RECIEVING DATA")
{
	SECTION("login request succesful - change mUsers")
	{
        const int DATA_LENGTH = 10;
        unsigned char* data = new unsigned char[DATA_LENGTH];
        unsigned char* packet = NULL;
        int size = 0;

        memset(data, 97, DATA_LENGTH);

        Server* server = new Server(13374);
        std::cout << "Server Started" << std::endl;

        /*NetworkManager manager;
        manager.startConnection(IP_SERVER, PORT_SERVER);

        server->createPacket(LOGIN_REQUEST, data, &packet, DATA_LENGTH);
        manager.sendData(packet, DATA_LENGTH);*/


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
    int userCount = 0;
    Server *server = new Server(13374);
    User* user = new User("asdf", "asdf", "127.0.0.1", NULL, -1);
	SECTION("add first user")
	{
        server->loginUser(user);
        int size = server->getUsers().size();
        userCount++;
        REQUIRE(size == userCount);
	}
	SECTION("add more users")
	{
        for(int i = 0; i < 5; i++)
            server->loginUser(user);

        userCount += 5;
        int size = server->getUsers().size();

        REQUIRE(size == 6);
	}
	SECTION("remove user")
	{
        server->logoutUser(user);

        int size = server->getUsers().size();
        userCount--;
        REQUIRE(size == userCount);
	}
    SECTION("remove 5 users")
	{
        for(int i = 0; i < 5; i++)
            server->logoutUser(user);

        int size = server->getUsers().size();
        userCount = 0;
        REQUIRE(size == userCount);
	}
	SECTION("remove from empy users list")
	{
        server->logoutUser(user);
        userCount = 0;
        int size = server->getUsers().size();
        REQUIRE(size == userCount);
	}
}

#endif
