#ifndef CRYPTOMANAGER_H
#define CRYPTOMANAGER_H

#include <string>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <thread>
#include "Crypto/polarssl/aes.h"
#include "Crypto/polarssl/sha256.h"


const int AES_KEY_LENGTH = 128;
const int AES_IV_LENGTH = 16;
const int CTR_PART_LENGTH = 16;
const int HASH_LENGHT = 32;
const int KEYSTREAM_SIZE = 10000 * 16;

class CryptoManager
{
private:
    aes_context mAes;
	sha256_context mSha;
    std::thread *mEncKeystreamThread = nullptr;
    std::thread *mDecKeystreamThread = nullptr;
    char* mEncKeystream = nullptr;
    char* mDecKeystream = nullptr;
    char mCounterStart[CTR_PART_LENGTH];
    int mEncKeystreamStart, mEncKeystreamEnd;
    int mDecKeystreamStart, mDecKeystreamEnd;
    unsigned char *mAesKey = nullptr;
public:
    CryptoManager();
    ~CryptoManager();

	/**
	* zasifruje plainData
	*
	*
	* @param plainData		data urcena k zasifrovani
	* @param encryptedData	sem se ulozi zasifrovana data
	* @param key			klic
	* @param IV				iniciacni vektor
	* @param size			delka dat

	*/
    int encryptAES(unsigned char* plainData, unsigned char* encryptedData, unsigned char *key, unsigned char *IV, int size);

	/**
	* desifruje encryptedData
	*
	*
	* @param encryptedData	zasifrovana data
	* @param decryptedData	sem se ulozi desifrovana data
	* @param key			klic
	* @param IV				iniciacni vektor
	* @param size			delka dat
	*/
	int decryptAES(unsigned char* encryptedData, unsigned char* decryptedData, unsigned char* key, unsigned char* IV, int size);

	/**
	* vygeneruje AES klic
	*
	* @param key

	*/
	int generateKeyAES(unsigned char* key);

	/**
	* na zaklade dvou AES klici vygeneru vysledny
	*
	* @param firstKey
	* @param secondKey
	* @param resultKey
	* @param size
	*/
	int mergeKeysAES(unsigned char* firstKey, unsigned char* secondKey, unsigned char* resultKey, int size);

	/**
	* spocita hash danych dat
	*
	* @param data
	* @param hash
	* @param size
	*/
    int computeHash(unsigned char* data, unsigned char hash[], int size);

    bool compareHash(unsigned char* hash1, unsigned char* hash2, int size);

	/**
	* ziska public key daneho uzivatele
	*
	* @param name
	* @param publicKey	tady vraci public klic daneho uzivatele
	*/
	void getPublicKey(const std::string& name, unsigned char* publicKey);

    std::string PrepairCounterForEncryption(int counter);

    int XORData(unsigned char* input, unsigned char *output, int size, unsigned char* key);

    void startCtrCalculation(unsigned char* key, unsigned char* counter);

    void getEncKeystream(unsigned char* stream, int length, int from = -1);

    void getDecKeystream(unsigned char* stream, int length, int from = -1);

private:
    void getKeystream(unsigned char* stream, int* start, int* end, unsigned char* output, int length, int from);
    void generateCtrKeystream(unsigned char* stream, int* start, int* end);
    void generateEncCtrKeystream();
    void generateDecCtrKeystream();
};
#endif
