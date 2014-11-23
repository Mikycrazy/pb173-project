#include "CryptoManager.h"


CryptoManager::CryptoManager()
{
    aes_init(&mAes);

    mKeystreamStart = -1;
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

void CryptoManager::startCtrCalculation(unsigned char* key, unsigned char* counter)
{
    memcpy(mAesKey, key, AES_KEY_LENGTH / 8);
    memcpy(mCounterStart, counter, CTR_PART_LENGTH);
    mKeystreamThread = new std::thread(&CryptoManager::generateCtrKeystream, this);
}

void CryptoManager::generateCtrKeystream()
{
    aes_context aes;
    unsigned char nonce_counter[CTR_PART_LENGTH];
    std::chrono::milliseconds sleepTime(50);

    mKeystreamStart = mKeystreamEnd = 0;
    aes_setkey_enc(&aes, (const unsigned char*)mAesKey, AES_KEY_LENGTH);
    memcpy(nonce_counter, mCounterStart, CTR_PART_LENGTH);

    while (true)
    {
        aes_crypt_ecb(&aes, AES_ENCRYPT, nonce_counter, (unsigned char*)(mKeystream + mKeystreamEnd));

        if (mKeystreamStart == -1)
            mKeystreamStart = 0;

        for(int i = CTR_PART_LENGTH; i > 0; i--)
            if(++nonce_counter[i - 1] != 0)
                break;

        mKeystreamEnd += CTR_PART_LENGTH;
        if (mKeystreamEnd == KEYSTREAM_SIZE)
            mKeystreamEnd = 0;

        while(mKeystreamStart - mKeystreamEnd >= 0 && mKeystreamStart - mKeystreamEnd <= CTR_PART_LENGTH)
            std::this_thread::sleep_for(sleepTime);
    }
}

void CryptoManager::getKeystream(unsigned char* stream, int length, int from)
{
    std::chrono::milliseconds sleepTime(10);

    while(mKeystreamEnd - mKeystreamStart > 0 && mKeystreamEnd - mKeystreamStart <= length)
        std::this_thread::sleep_for(sleepTime);

    int start = from == -1 ? mKeystreamStart : from;
    if (start + length < KEYSTREAM_SIZE)
    {
        memcpy(stream, mKeystream + start, length);

        if (from == -1)
            mKeystreamStart += length;
    }
    else
    {
        int endDiff = KEYSTREAM_SIZE - start;
        memcpy(stream, mKeystream + start, endDiff);
        memcpy(stream + endDiff, mKeystream, length - endDiff);

        if (from == -1)
            mKeystreamStart = length - endDiff;
    }
}
