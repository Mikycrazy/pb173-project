
#include "Header.h"

int main(int argc, char *argv[])
{
	int mode;

	aes_context aes;
	sha256_context sha;

	aes_init(&aes);
	sha256_init(&sha);

	std::string str;

	unsigned char key[KEY_LENGTH] = "0";
	unsigned char iv[IV_LENGTH] = { 0xb6, 0x58, 0x9f, 0xc6, 0xab, 0x0d, 0xc8, 0x2c, 0xf1, 0x20, 0x99, 0xd1, 0xc2, 0xd4, 0x0a, 0xb9 };
	unsigned char iv2[IV_LENGTH];
	unsigned char* input = nullptr;
	unsigned char* output = nullptr;
	unsigned char hash[HASH_LENGHT + 1];

	int ret = 1;

	memset(key, 1, KEY_LENGTH);
	memcpy(iv2, iv, IV_LENGTH);

	std::cout << "1 - Encrytion		2 - Decryption" << std::endl;
	std::cin >> mode;

	if (mode == 1)
	{
		if (loadFile("file.txt", str))
			return 1;

		size_t input_size = str.length();

		if (input_size == 0)
			return 1;

		size_t size_for_encryption = input_size + (16 - (input_size % 16)) + 16;

		input = new unsigned char[size_for_encryption + 1];
		output = new unsigned char[size_for_encryption + 1];

		memset(input, 0, size_for_encryption + 1);
		memset(output, 0, size_for_encryption + 1);

		//Padding data
		addPadding(str);
		cpyStringToUnsignedCharArray(str, input);

		//Encrypting data
		encrypt(&aes, size_for_encryption, iv, key, input, output);

		//Hashing data
		computeHash(&sha, output, size_for_encryption, hash);

		//Save cipher and hash
		saveFile("encrypted file.txt", output, hash);
	}
	else if (mode == 2)
	{
		if (loadFile("encrypted file.txt", str))
			return 1;

		size_t input_size = str.length();

		std::string hash_str = str.substr(input_size - 32, 32);
		std::string cipher = str.substr(0, input_size - 32);

		size_t cipher_size = cipher.length();

		input = new unsigned char[cipher_size + 1];
		output = new unsigned char[cipher_size + 1];

		memset(input, 0, cipher_size + 1);
		memset(output, 0, cipher_size + 1);

		cpyStringToUnsignedCharArray(cipher, input);

		//Computing Hash
		computeHash(&sha, input, cipher_size, hash);
		compareHash(hash_str, hash);

		//Decrypting data
		decrypt(&aes, cipher_size, iv2, key, input, output);

		//Remove Padding
		removePadding(output, cipher_size);

		//Save decrypted string
		saveFile("decrypted file.txt", output);
	}

	//Cleaning
	memset(key, 0, KEY_LENGTH);
	memset(&aes, 0, sizeof(aes_context));
	memset(&sha, 0, sizeof(sha256_context));

	delete input;
	delete output;
}

int loadFile(const char * filename, std::string& output)
{
	std::fstream file(filename, std::ios::in);
	std::string all;

	if (!file.good())
		return 1;

	while (file.good())
	{
		std::string radek;
		std::getline(file, radek);
		all += radek + "\n";
	}

	if (!all.empty())
		all.erase(std::prev(all.end()));

	file.close();
	output = all;

	return 0;
}

int saveFile(const char * filename, unsigned char* text, unsigned char hash[HASH_LENGHT + 1])
{
	std::ofstream file;
	file.open(filename);

	if (!file)
	{
		return -1;
	}

	file << text;
	file << hash;
	file.close();

	return 0;
}

int saveFile(const char * filename, unsigned char* text)
{
	std::ofstream file;
	file.open(filename);

	if (!file)
	{
		return -1;
	}

	file << text;
	file.close();

	return 0;
}

int addPadding(std::string& input_text)
{
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
		if (pom < 10)
			str += "0" + std::to_string(pom);
		else
			str += std::to_string(pom);

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

int encrypt(aes_context *ctx, size_t length, unsigned char iv[IV_LENGTH], unsigned char key[KEY_LENGTH], const unsigned char *input, unsigned char *output)
{
	aes_setkey_enc(ctx, key, 128);
	return aes_crypt_cbc(ctx, AES_ENCRYPT, length, iv, input, output);
}

int decrypt(aes_context *ctx, size_t length, unsigned char iv[IV_LENGTH], unsigned char key[KEY_LENGTH], const unsigned char *input, unsigned char *output)
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
