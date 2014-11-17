#include "CryptoManager.h"

/*

int addPadding(std::string& input_text)
{
    aes_context ctx;
    unsigned char key[AES_KEY_LENGTH];
    aes_setkey_enc(&ctx, key, 128);
    std::string str = input_text;
    int input_size = input_text.length();
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

int compareHash(std::string hash_str, unsigned char hash[HASH_LENGHT + 1])
{
    bool intergrity = true;

    for (size_t i = 0; i < 32; i++)
    {
        if (hash[i] != static_cast<unsigned char>(hash_str[i]))
            intergrity = false;
    }

    if (intergrity)
    {
        std::cout << "Hash...... OK" << std::endl;
    }
    else
    {
        std::cout << "Hash...... Failed" << std::endl;
    }

    return 0;
}


int encrypt(aes_context *ctx, size_t length, unsigned char iv[AES_IV_LENGTH], unsigned char key[AES_KEY_LENGTH], const unsigned char *input, unsigned char *output)
{
    //aes_setkey_enc(ctx, key, 128);
    return 0;//aes_crypt_cbc(ctx, AES_ENCRYPT, length, iv, input, output);
}
/*
int decrypt(aes_context *ctx, size_t length, unsigned char iv[AES_IV_LENGTH], unsigned char key[AES_KEY_LENGTH], const unsigned char *input, unsigned char *output)
{
    aes_setkey_dec(ctx, key, 128);
    return aes_crypt_cbc(ctx, AES_DECRYPT, length, iv, input, output);
}

int computeHash(sha256_context *ctx, const unsigned char * buffer, int buffer_size, unsigned char hash[HASH_LENGHT + 1])
{
    sha256_starts(ctx, 0);
    sha256_update(ctx, buffer, buffer_size);
    sha256_finish(ctx, hash);

    hash[32] = 0;

    return 0;
}

int removePadding(unsigned char * array, int length)
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

*/
