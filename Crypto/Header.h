
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include "polarssl/aes.h"
#include "polarssl/sha256.h"

const int KEY_LENGTH = 16;
const int IV_LENGTH = 16;
const int HASH_LENGHT = 32;

int decrypt(aes_context *ctx, size_t length, unsigned char iv[IV_LENGTH], unsigned char key[KEY_LENGTH], const unsigned char *input, unsigned char *output);
int encrypt(aes_context *ctx, size_t length, unsigned char iv[IV_LENGTH], unsigned char key[KEY_LENGTH], const unsigned char *input, unsigned char *output);

int loadFile(const char * filename, std::string& output);
int saveFile(const char * filename, unsigned char* text, unsigned char hash[HASH_LENGHT + 1]);
int saveFile(const char * filename, unsigned char* text);

int addPadding(std::string& input_text);
int removePadding(unsigned char * array, int length);

int computeHash(sha256_context *ctx, const unsigned char * buffer, int buffer_size, unsigned char hash[HASH_LENGHT + 1]);
int compareHash(std::string hash_str, unsigned char hash[HASH_LENGHT + 1]);

int cpyStringToUnsignedCharArray(std::string str, unsigned char * array);