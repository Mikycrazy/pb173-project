#include "CryptoManager.h"


CryptoManager::CryptoManager()
{
    aes_init(&mAes);
}

int CryptoManager::addPadding(std::string& input_text)
{
    std::string str (input_text);
    int input_size = str.length();
    int size_for_encryption;
    int pom;

    if (input_size % 16 != 0)
    {
        size_for_encryption = input_size + (16 - (input_size % 16)) + 16;
        pom = (16 - (input_size % 16));
    }
    else
    {
        size_for_encryption = input_size + 16;
        pom = 0;
    }

    if (input_size % 2 == 1)
    {
        input_size++;
        str += "0";
    }

    while (input_size != size_for_encryption)
    {
        std::stringstream ss;

        if (pom < 10)
            str += "0";
        ss << pom;
        str += ss.str();

        input_size += 2;
    }

    input_text = str;

    return 0;
}

std::string CryptoManager::PrepairCounterForEncryption(int counter)
{
    std::stringstream ss;
    ss << counter;

    int sizeofpadding = 16 - ss.str().length();

    for(int i = 0; i < sizeofpadding; i++)
    {
        ss << 'x';
    }

    return ss.str();
}


int CryptoManager::encryptAES(unsigned char* plainData, unsigned char* encryptedData, unsigned char* key, unsigned char* IV, int size)
{
    aes_setkey_enc(&mAes, key, 128);
    aes_crypt_cbc(&mAes, AES_ENCRYPT, size, IV, plainData, encryptedData);
    return 0;
}

int CryptoManager::decryptAES(unsigned char* encryptedData, unsigned char* decryptedData, unsigned char* key, unsigned char* IV, int size)
{
    aes_setkey_dec(&mAes, key, 128);
    return aes_crypt_cbc(&mAes, AES_DECRYPT, size, IV, encryptedData, decryptedData);
}

int CryptoManager::XORData(unsigned char* input, unsigned char *output, int size, unsigned char* key)
{
    for(int i = 0; i < size; i++)
    {
        output[i] = input[i] ^ key[i];
    }

    return 0;
}

int CryptoManager::computeHash(unsigned char* data, unsigned char* hash, int size)
{
    /*sha256_starts(ctx, 0);
    sha256_update(ctx, buffer, buffer_size);
    sha256_finish(ctx, hash);

    hash[32] = 0;
*/
    return 0;
}

int CryptoManager::removePadding(unsigned char * array, int length)
{
    char pom[2];

    memcpy(pom, array + length - 2, 2);
    int padding = atoi(pom);

    size_t output_size = length - 16 - padding;

    array[output_size] = 0;

    return 0;
}

int cpyStringToUnsignedCharArray(std::string str, unsigned char * array)
{
    for (size_t i = 0; i < str.length(); i++)
    {
        array[i] = str.at(i);
    }

    return 0;
}


