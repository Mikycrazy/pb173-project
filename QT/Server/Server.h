#ifndef SERVER_H
#define SERVER_H

#include "User.h"
#include "Opcodes.h"
#include "NetworkManager.h"
#include "CryptoManager.h"
#include <vector>
#include <QDebug>
#include <time.h>
#include "logger.h"
#include <sstream>
#include <random>

using namespace std;

const int ID_LENGHT = 1;
const int RANDOM_BYTES_LENGTH = 10;
const int DATA_SIZE_LENGTH = 4;
const int INTERGRITY_HASH_SIZE = 32;
const std::string DATA_SPLITER = ";";

class Server : QObject
{
    Q_OBJECT

private:
    vector<User*> mUsers;
    unsigned char* mPrivateKey = NULL;
    unsigned char* mPublicKey = NULL;
    NetworkManager* mNetwork = NULL;
    CryptoManager* mCrypto = NULL;
public:

    /*
    * Konstruktor pre triedu Server.
    */
    Server();
    Server(quint16 port);
    ~Server();

    /**
    * Prihlasenie pouzivatela
    *
    * @param user	ukazatel na objekt uzivatela ktory ma byt prihlaseny
    */
    bool loginUser(User* user);

    /**
    * Overenie uzivatela u certifikacnej autority
    *
    * @param user	ukazatel na objekt uzivatela ktory ma byt overeny
    */
    bool verifyUser(User* user);

    /**
    * Odhlasenie pouzivatela
    *
    * @param user	ukazatel na objekt uzivatela ktory ma byt odhlaseny
    */
    bool logoutUser(User* user);

    /**
    * Odoslanie zoznamu online uzivatelov vybranemu pouzivatelovy.
    *
    * @param user	ukazatel na objekt uzivatela ktory ma byt zaslany zozam online uzivatelov
    */
    void sendOnlineList(User* user);

    /**
    * Odoslanie ziadosti o nadviazanie spojenia medzi dvoma uzivatelmi
    *
    * @param from	ukazatel na objekt uzivatela ktory ziada o vytvorenie spojenia
    * @param to		ukazatel na objekt uzivatela ktoremu je zaslana ziadost
    */
    bool sendConnectionRequest(User* from, User* to, unsigned char* data, int size);
    /**
    * Odoslanie ziadosti o nadviazanie spojenia medzi dvoma uzivatelmi
    *
    * @param from	ukazatel na objekt uzivatela ktory ziada o vytvorenie spojenia
    * @param to		ukazatel na objekt uzivatela ktoremu je zaslana ziadost
    */
    bool sendConnectionResponse(User* from, User* to, unsigned char* data, int size);

    int processPacket(unsigned char* packet, unsigned char** data, int size);

    /**
    * Zpracuje data a id pozadavku do podoby paketu k odeslani po siti
    *
    * @param id         id paketu, jestli jde o login/logout nebo neco dalsiho
    * @param data		data
    * @param packet		sem se ulozi packet, ktery je pripraven k odeslani
    * @param size		delka dat
    */
    int createPacket(unsigned char id, unsigned char* data, unsigned char **packet, int size);

    vector<User*> const& getUsers() const { return mUsers; }
public slots:

    /**
    * Zpracuje prisly packet, vyparsuje data a vykona potrebne akce
    *
    * @param packet         prisli packet
    * @param connectionID   id spojenia
    */
    void processPacket(int connectionID, unsigned char* packet, int size);

    void disconnectClient(int connectionID);

private:

    void processLoginUserPacket(int connectionID, unsigned char *data, int size);

    void processLogoutUserPacket(int connectionID, unsigned char *data, int size);

    /**
    * Zpracuje prisly packet, vyparsuje data a vykona potrebne akce
    *   vraci connection id
    */
    int processClientComunicationRequest(unsigned char *data, int size);


    /**
    * Zpracuje prisly packet, vyparsuje data a vykona potrebne akce
    *   vraci connection id
    */
    int processClientComunicationResponse(unsigned char *data, int size);

    User* getUserFromConnectionID(int connectionID);
};

#endif
