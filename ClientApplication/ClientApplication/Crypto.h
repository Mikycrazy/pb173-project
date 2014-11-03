#pragma once

const int AES_KEY_LENGTH = 128;
const int AES_IV_LENGTH = 16;

class CryptoManager
{
private:
	/*
	aes_context mAes;
	sha256_context mSha;
	pk_context mRsa;
	*/
public:
	CryptoManager();
	~CryptoManager();

	int encryptAES();
	int decryptAES();

	int encryptRSA();
	int decryptRSA();

	int generateKeysRSA();
	int generateKeyAES();

	int computeHash();

private:
	int addPadding();
	int removePadding();
};

