#pragma once
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <time.h>
#include "CryptoManager.h"
#include "NetworkManager.h"
#include "Opcodes.h"
#include "logger.h"

const int ID_LENGHT = 1;
const int RANDOM_BYTES_LENGTH = 10;
const int DATA_SIZE_LENGTH = 4;
const std::string DATA_SPLITER = ";";

const QString SERVER_ADDRESS = "127.0.0.1";
const quint16 SERVER_PORT = 13374;

class Client : public QObject
{
    Q_OBJECT

private:
	std::string mUsername;
	std::string mEmail;

	unsigned char* mAESkey;
    unsigned char* mAESIV;
	unsigned char* mPublicRSAKey;
	unsigned char* mPrivateRSAKey;

	unsigned char* mCertificate;

	bool mLoggedToServer;
	bool mConnectedToClient;

    QString mReceiverIP;
	int mReceiverPort;

    NetworkManager* mNetwork;

    unsigned char* mLastReicevedData;
    int mLastReicevedDataSize;

public:
	/**
	* Konstruktor pro tridu Client.
	* Nastavi username a email novemu clientovi.
	*
	* @param username		uzivatelke jmeno
	* @param email			email
	*/
    Client() : mLoggedToServer(false), mConnectedToClient(false) { ; }
	Client(std::string username, std::string email);
    ~Client() {}

	std::string Username() const { return mUsername; }
	void Username(std::string username)	{ mUsername = username; }

	std::string Email() const	{ return mEmail; }
	void Email(std::string email) { mEmail = email; }

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

    int processPacket(unsigned char* packet, unsigned char** data);

    bool isLogged();

    bool isConnected();

    void sendData(unsigned char *data, int size);

    unsigned char* getLastData() {return mLastReicevedData; }

    int getLastDataSize() { return mLastReicevedDataSize; }

    int sendDataToClient(QHostAddress address, quint16 port, unsigned char* data, int size);

private:
    int processServerCommunicationRequest(unsigned char *data, int size);

    int processServerCommunicationResponse(unsigned char *data, int size);

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

