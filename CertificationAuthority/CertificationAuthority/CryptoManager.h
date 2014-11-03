#include <string>

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

	int encryptAES(unsigned char* plainData, unsigned char* encryptedData, unsigned char* IV, int size);
	int decryptAES(unsigned char* encryptedData, unsigned char* decryptedData, unsigned char* IV, int size);

	int encryptRSA();
	int decryptRSA();

	int generateKeysRSA(unsigned char* privateKey, unsigned char* publicData, int size);
	int generateKeyAES(unsigned char* key);
	int mergeKeysAES(unsigned char* firstKey, unsigned char* secondKey, unsigned char* resultKey, int size);

	int computeHash(unsigned char* data, unsigned char* hash, int size);

	void getPublicKey(std::string name, unsigned char* publicKey);

private:
	int addPadding(unsigned char* data, int size);
	int removePadding(unsigned char* data, int size);
};