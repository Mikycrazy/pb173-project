#pragma once
#include <string>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include "Crypto/polarssl/aes.h"
#include "Crypto/polarssl/sha256.h"



const int AES_KEY_LENGTH = 128;
const int AES_IV_LENGTH = 16;
const int HASH_LENGHT = 32;

class CryptoManager
{
private:
    aes_context mAes;
	sha256_context mSha;
    //pk_context mRsa;
public:
    CryptoManager();
    ~CryptoManager() { ; }

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
	* zasifruje plainData
	*
	*
	* @param plainData		data urcena k zasifrovani
	* @param encryptedData	sem se ulozi zasifrovana data
	* @param publicKey		klic
	* @param size			delka dat

	*/
	int encryptRSA(unsigned char* publicKey, unsigned char* plainData, unsigned char* encryptedData, int size);


	/**
	* desifruje encryptedData
	*
	*
	* @param encryptedData	data urcena k desifrovani
	* @param decryptedData	sem se ulozi desifrovana data
	* @param privateKey		klic
	* @param size			delka dat

	*/
	int decryptRSA(unsigned char* privateKey, unsigned char* encryptedData, unsigned char* decryptedData, int size);

	/**
	* vygeneruje Dvojici RSA klicu
	*
	* @param publicKey	
	* @param privateKey		
	* @param size		

	*/
	int generateKeysRSA(unsigned char* privateKey, unsigned char* publicKey, int size);

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
	int computeHash(unsigned char* data, unsigned char* hash, int size);

	/**
	* ziska public key daneho uzivatele
	*
	* @param name
	* @param publicKey	tady vraci public klic daneho uzivatele
	*/
	void getPublicKey(const std::string& name, unsigned char* publicKey);

    std::string PrepairCounterForEncryption(int counter);

    int XORData(unsigned char* input, unsigned char *output, int size, unsigned char* key);

private:
    int addPadding(std::string &input_text);
	int removePadding(unsigned char* data, int size);
};
