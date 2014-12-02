#pragma once
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <iomanip>
#include <time.h>
#include "CryptoManager.h"
#include "NetworkManager.h"
#include "Opcodes.h"
#include "logger.h"
#include "User.h"
#include <vector>


const int ID_LENGHT = 1;
const int RANDOM_BYTES_LENGTH = 10;
const int DATA_SIZE_LENGTH = 4;
const int CYPHER_BLOCK_LENGTH = 16;
const int INTERGRITY_HASH_SIZE = 32;
const std::string DATA_SPLITER = ";";

const QString SERVER_ADDRESS = "127.0.0.1";
const quint16 SERVER_PORT = 13374;

class Client : public QObject
{
    Q_OBJECT

private:
	std::string mUsername;
	std::string mEmail;

    unsigned char* mAESkey = nullptr;
    unsigned char* mAESIV = nullptr;
    unsigned char* mPublicRSAKey = nullptr;
    unsigned char* mPrivateRSAKey = nullptr;

    std::vector<User*> mOnlineList;

    bool mLoggedToServer = nullptr;
    bool mConnectedToClient = nullptr;

    QHostAddress mReceiverIP;
    int mReceiverPort;

    NetworkManager* mNetwork;
    CryptoManager* mCrypto;

    unsigned char* mLastReicevedData = nullptr;
    int mLastReicevedDataSize;

    int mStatus;
    int mCypherPosition;
    int mCypherLastPositionRecieved;
    int UDPport;

public:

	/**
	* Konstruktor pro tridu Client.
	* Nastavi username a email novemu clientovi.
	*
	* @param username		uzivatelke jmeno
	* @param email			email
	*/
    Client();
    Client(std::string username, std::string email, qint16 UDPport);
    ~Client();

	std::string Username() const { return mUsername; }
	void Username(std::string username)	{ mUsername = username; }

	std::string Email() const	{ return mEmail; }
	void Email(std::string email) { mEmail = email; }

    std::vector<User*> OnlineList() const	{ return mOnlineList; }
    void Email( std::vector<User*> onlineList) { mOnlineList = onlineList; }

	/**
	* Vygenerovani novych rsa klicu.
	*/
	int generateNewRSAKeys();
	/**
	* Vygenerovani noveho AES klice.
	*/
	int generateNewAESKey();
	/**
	* Odesle pozadavek na prihlaseni na server.
	*/
	int login();
	/**
	* Odesle pozadavek na odhlaseni na server.
	*/
	int logout();
    /**
    * Odesle pozadavek na list online uzivatelu na server
    */
    int getOnlineList();

	/**
	* Odesle pozadavek na vydani certifikatu uzivatele, na certifikacni autoritu
	*
	* @param username		uzivatelske jmeno 
	*/
	int getCertificat(std::string username);
	/**
	* Odesle pozadavek k registraci s username a email na certifikacni autoritu.
	*/
	int registerToCA();

	/**
	* Odesle pozadavek ke komunikaci ke klientovi
	*
    * @param connectionID		connectionID
	*/
    int connectToClient(int connectionID);

    /**
    * Odesle kladnou odpoved na pozadavek ke komunikaci ke klientovi
    *
    * @param connectionID		connectionID
    */
    int acceptConnection(int connectionID, unsigned char* recievedKey);

    /**
    * Odesle zapornou odpoved na pozadavek ke komunikaci ke klientovi
    *
    * @param connectionID		connectionID
    */
    int refuseConnection(int connectionID);

	/**
	* Zahaji komunikaci s klientem
	*
	* @param data		data
	*/
	int startCommunication(unsigned char* data);

    /**
    * Zpracuje data a id pozadavku do podoby paketu k odeslani po siti
    *
    * @param id         id paketu, jestli jde o login/logout nebo neco dalsiho
    * @param data		data
    * @param packet		sem se ulozi packet, ktery je pripraven k odeslani
    * @param size		delka dat
    */

    int createPacket(unsigned char id, unsigned char* data, unsigned char **packet, int size);

    int processPacket(unsigned char* packet, unsigned char** data, int size);

    bool isLogged();

    bool isConnected();

    void sendData(unsigned char *data, int size);

    unsigned char* getLastData() {return mLastReicevedData; }

    int getLastDataSize() { return mLastReicevedDataSize; }

    int sendDataToClient(QHostAddress address, quint16 port, unsigned char* data, int size);

    int sendDataToClient(QHostAddress address, quint16 port, std::string filename);

    int getStatus();
    void setStatus(int status);
    void setCypherPosition(int pos);

private:
    int processGetOnlineListResponse(unsigned char *data, int size);

    int processServerCommunicationRequest(unsigned char *data, int size);

    int processServerCommunicationResponse(unsigned char *data, int size);

    int processServerCommunicationData(unsigned char **data, int size);
    void initNetwork();

public slots:

    /**
    * Zpracuje prisly packet
    *
    * @param packet		prisli packet
    *
    * @return vrati velikost dat
    */
    void processPacket(unsigned char* packet, int size);
};

