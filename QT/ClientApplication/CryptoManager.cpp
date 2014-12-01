#include "CryptoManager.h"


CryptoManager::CryptoManager()
{
    aes_init(&mAes);

    mEncKeystream = new char[KEYSTREAM_SIZE];
    mDecKeystream = new char[KEYSTREAM_SIZE];

    mEncKeystreamStart = -1;
    mDecKeystreamStart = -1;
}

CryptoManager::~CryptoManager()
{
    delete[] mEncKeystream;
    delete[] mDecKeystream;
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

int CryptoManager::computeHash(unsigned char* data, unsigned char hash[32], int size)
{
    sha256_starts(&mSha, 0);
    sha256_update(&mSha, data, size);
    sha256_finish(&mSha, hash);

    return 0;
}

bool CryptoManager::compareHash(unsigned char *hash1, unsigned char *hash2, int size)
{
    for(int i = 0; i < size; i++)
    {
        if(hash1[i] != hash2[i])
            return false;
    }
    return true;
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

void CryptoManager::startCtrCalculation(unsigned char* key, unsigned char* counter)
{
    mAesKey = new unsigned char[AES_KEY_LENGTH / 8];
    memcpy(mAesKey, key, AES_KEY_LENGTH / 8);
    memcpy(mCounterStart, counter, CTR_PART_LENGTH);
    mEncKeystreamThread = new std::thread(&CryptoManager::generateEncCtrKeystream, this);
    mDecKeystreamThread = new std::thread(&CryptoManager::generateDecCtrKeystream, this);
}

void CryptoManager::generateCtrKeystream(unsigned char* stream, int* start, int* end)
{
    aes_context aes;
    unsigned char nonce_counter[CTR_PART_LENGTH];
    std::chrono::milliseconds sleepTime(50);

    *end = 0;
    aes_setkey_enc(&aes, (const unsigned char*)mAesKey, AES_KEY_LENGTH);
    memcpy(nonce_counter, mCounterStart, CTR_PART_LENGTH);

    while (true)
    {
        aes_crypt_ecb(&aes, AES_ENCRYPT, nonce_counter, (unsigned char*)(stream + *end));

        if (*start == -1)
            *start = 0;

        for(int i = CTR_PART_LENGTH; i > 0; i--)
            if(++nonce_counter[i - 1] != 0)
                break;

        *end += CTR_PART_LENGTH;
        if (*end == KEYSTREAM_SIZE)
            *end = 0;

        while(*start - *end >= 0 && *start - *end <= CTR_PART_LENGTH)
            std::this_thread::sleep_for(sleepTime);
    }
}

void CryptoManager::generateEncCtrKeystream()
{
    this->generateCtrKeystream((unsigned char*)mEncKeystream, &mEncKeystreamStart, &mEncKeystreamEnd);
}

void CryptoManager::generateDecCtrKeystream()
{
    this->generateCtrKeystream((unsigned char*)mDecKeystream, &mDecKeystreamStart, &mDecKeystreamEnd);
}

void CryptoManager::getKeystream(unsigned char* stream, int* start, int* end, unsigned char* output, int length, int from)
{
    std::chrono::milliseconds sleepTime(10);

    while(*end - *start > 0 && *end - *start <= length)
        std::this_thread::sleep_for(sleepTime);

    int begin = from == -1 ? *start : from;
    if (begin + length < KEYSTREAM_SIZE)
    {
        memcpy(output, stream + begin, length);

            *start += length;
    }
    else
    {
        int endDiff = KEYSTREAM_SIZE - begin;
        memcpy(output, stream + begin, endDiff);
        memcpy(output + endDiff, stream, length - endDiff);

            *start = length - endDiff;
    }
}

void CryptoManager::getEncKeystream(unsigned char* stream, int length, int from)
{
    this->getKeystream((unsigned char*)mEncKeystream, &mEncKeystreamStart, &mEncKeystreamEnd, stream, length, from);
}

void CryptoManager::getDecKeystream(unsigned char* stream, int length, int from)
{
    this->getKeystream((unsigned char*)mDecKeystream, &mDecKeystreamStart, &mDecKeystreamEnd, stream, length, from);
}
