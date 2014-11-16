#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "Header.h"

TEST_CASE("File managment")
{
	SECTION("File.txt is able to open")
	{
		char path[] = "file.txt";
		std::string out;

		REQUIRE(loadFile(path, out) == 0);
	}

	SECTION("wrong File.txt is not able to open")
	{
		char path[] = "wrong file.txt";
		std::string out;

		REQUIRE_FALSE(loadFile(path, out) == 0);
	}

	SECTION("Save file")
	{
		char path[] = "file2.txt";

		unsigned char* text = new unsigned char[20];
		memset(text, 97, 20);

		REQUIRE(saveFile(path, text) == 0);
	}
}

TEST_CASE("Add Padding")
{
	SECTION("Test string is 16 chars long. Padding is full of zeroes")
	{
		std::string text = "1111111111111111";
		addPadding(text);

		std::string padding = text.substr(text.length() - 16, 16);

		REQUIRE(text.length() == 32);
		REQUIRE(padding == "0000000000000000");
	}

	SECTION("Test string should be 32 chars length. Pading is full of 05")
	{
		std::string text = "Hello World";
		addPadding(text);

		std::string padding = text.substr(text.length() - 16, 16);

		REQUIRE(text.length() == 32);
		REQUIRE(padding == "0505050505050505");

	}

	SECTION("Add Padding and remove padding", "First add padding and after that remove. Input and output should be same. All possible casses")
	{
		std::string text = "";
		std::string text1 = text;
		for (size_t i = 1; i < 17; i++)
		{
			text = text1;
			text += "1";
			text1 = text;


			addPadding(text);
			unsigned char * data = new unsigned char[text.length()];
			cpyStringToUnsignedCharArray(text, data);
			removePadding(data, text.length());

			std::string text2(reinterpret_cast<char*>((data)));
			REQUIRE(text1 == text2);
		}
	}

}

TEST_CASE("Encrypted file was corrupted", "Length of file must be devided by 16")
{


	char path[] = "corrupted encrypted file.txt";
	std::string out;
	loadFile(path, out);

	int pom = out.length() % 16;

	REQUIRE_FALSE(pom == 0);
}

TEST_CASE("Decryption and Encryption")
{
	SECTION("Wrong key was used")
	{
		aes_context aes;

		std::string text1 = "hello world";

		unsigned char key1[KEY_LENGTH];
		unsigned char key2[KEY_LENGTH];

		unsigned char iv1[IV_LENGTH] = { 0xb6, 0x58, 0x9f, 0xc6, 0xab, 0x0d, 0xc8, 0x2c, 0xf1, 0x20, 0x99, 0xd1, 0xc2, 0xd4, 0x0a, 0xb9 };
		unsigned char iv2[IV_LENGTH];

		unsigned char* input = new unsigned char[33];
		unsigned char* output = new unsigned char[33];

		memset(key1, 1, KEY_LENGTH);
		memset(key2, 0, KEY_LENGTH);
		memcpy(iv2, iv1, IV_LENGTH);

		addPadding(text1);
		cpyStringToUnsignedCharArray(text1, input);

		encrypt(&aes, 32, iv1, key1, input, output);
		decrypt(&aes, 32, iv2, key2, output, input);

		input[32] = 0;

		std::string text2(reinterpret_cast<char*>((input)));


		REQUIRE_FALSE(text1.compare(text2) == 0);

		delete input;
		delete output;
	}

	SECTION("Right key was used")
	{
		aes_context aes;

		std::string text1 = "hello world";

		unsigned char key1[KEY_LENGTH];

		unsigned char iv1[IV_LENGTH] = { 0xb6, 0x58, 0x9f, 0xc6, 0xab, 0x0d, 0xc8, 0x2c, 0xf1, 0x20, 0x99, 0xd1, 0xc2, 0xd4, 0x0a, 0xb9 };
		unsigned char iv2[IV_LENGTH];

		unsigned char* input = new unsigned char[33];
		unsigned char* output = new unsigned char[33];

		memset(key1, 1, KEY_LENGTH);
		memcpy(iv2, iv1, IV_LENGTH);

		addPadding(text1);
		cpyStringToUnsignedCharArray(text1, input);

		encrypt(&aes, 32, iv1, key1, input, output);
		decrypt(&aes, 32, iv2, key1, output, input);

		input[32] = 0;

		std::string text2(reinterpret_cast<char*>((input)));


		REQUIRE(text1.compare(text2) == 0);

		delete input;
		delete output;
	}
}

TEST_CASE("Encryption test vectors")
{
	aes_context aes;

	SECTION("Key = aaaaaaaaaaaaaaaa, Text = abcdefghijklmopq, IV= b6589fc6ab0dc82cf12099d1c2d40ab9")
	{

		const int LENGTH = 16;

		std::string text1 = "abcdefghijklmopq";

		unsigned char cipher_text[17] = { 0x47, 0x3e, 0xa8, 0x75, 0x33, 0x19, 0x0b, 0x98, 0x62, 0x66, 0x97, 0x23, 0x4d, 0x9c, 0xaf, 0xbd, 0x00 };

		unsigned char key1[KEY_LENGTH];
		unsigned char iv1[IV_LENGTH] = { 0xb6, 0x58, 0x9f, 0xc6, 0xab, 0x0d, 0xc8, 0x2c, 0xf1, 0x20, 0x99, 0xd1, 0xc2, 0xd4, 0x0a, 0xb9 };

		unsigned char* input = new unsigned char[LENGTH + 1];
		unsigned char* output = new unsigned char[LENGTH + 1];

		memset(key1, 97, KEY_LENGTH);
		memset(input, 0, LENGTH + 1);
		memset(output, 0, LENGTH + 1);

		cpyStringToUnsignedCharArray(text1, input);
		encrypt(&aes, LENGTH, iv1, key1, input, output);

		bool match = true;
		for (size_t i = 0; i < LENGTH + 1; i++)
		{
			if (output[i] != cipher_text[i])
			{
				match = false;
				break;
			}
		}

		REQUIRE(match);

		delete input;
		delete output;
	}

	SECTION("Key = 1111111111111111, Text = Aenean lorem nibh, iaculis non molestie quis, ullamcorper vel eros. Donec metus., IV= b6589fc6ab0dc82cf12099d1c2d40ab9")
	{
		const int LENGTH = 80;

		std::string text1 = "Aenean lorem nibh, iaculis non molestie quis, ullamcorper vel eros. Donec metus.";

		unsigned char cipher_text[LENGTH + 1] = {	0x44, 0x1c, 0xf6, 0x17, 0x8e, 0x31, 0x16, 0xcc, 0xb4, 0x51, 0xf6, 0xaa, 0x11, 0x34, 0xa8, 0xa9,
											0x76, 0xa5, 0x3c, 0xfc, 0x12, 0xed, 0xb5, 0xe5, 0x54, 0xc8, 0x8c, 0xb9, 0x88, 0x28, 0x0d, 0xa5,
											0xb4, 0x40, 0x0a, 0x6e, 0xc1, 0xca, 0x8e, 0x7a, 0x73, 0xc6, 0xa9, 0x64, 0xc6, 0xbd, 0x49, 0x02,
											0x01, 0xf6, 0xef, 0x5b, 0x8a, 0x9f, 0x32, 0x98, 0x5c, 0x38, 0xd1, 0x6f, 0xd1, 0x54, 0x0a, 0xf5,
											0x7b, 0x87, 0x98, 0xb0, 0x82, 0x3a, 0xa9, 0x55, 0x9b, 0x81, 0xa7, 0x29, 0xe0, 0x43, 0x12, 0xe3, 0x00 };

		unsigned char key1[KEY_LENGTH];
		unsigned char iv1[IV_LENGTH] = { 0x4F, 0x02, 0x1D, 0xB2, 0x43, 0xBC, 0x63, 0x3D, 0x71, 0x78, 0x18, 0x3A, 0x9F, 0xA0, 0x71, 0xE8 };

		unsigned char* input = new unsigned char[LENGTH + 1];
		unsigned char* output = new unsigned char[LENGTH + 1];

		memset(key1, 49, KEY_LENGTH);
		memset(input, 0, LENGTH + 1);
		memset(output, 0, LENGTH + 1);

		cpyStringToUnsignedCharArray(text1, input);
		encrypt(&aes, LENGTH, iv1, key1, input, output);

		bool match = true;
		for (size_t i = 0; i < LENGTH + 1; i++)
		{
			if (output[i] != cipher_text[i])
			{
				match = false;
				break;
			}
		}

		REQUIRE(match);

		delete input;
		delete output;
	}
}

TEST_CASE("Hash test vectors")
{
	sha256_context sha;

	SECTION("text = abcdefghijklmopq")
	{

		std::string text = "abcdefghijklmopq";
		unsigned char hash[33];
		unsigned char expected_hash[32] = { 0x0e, 0x68, 0x2e, 0xa2, 0x17, 0x67, 0x72, 0x5e, 0x06, 0x89, 0x2b, 0xed, 0x71, 0xb3, 0xbe, 0xa8, 0xb8, 0xa8, 0x7b, 0x18, 0x87, 0x86, 0x0e, 0x86, 0x78, 0x37, 0x2b, 0xda, 0xe4, 0x80, 0x1f, 0xaa };
		unsigned char* buffer = new unsigned char[text.length()];

		cpyStringToUnsignedCharArray(text, buffer);

		computeHash(&sha, buffer, text.length(), hash);

		bool match = true;
		for (size_t i = 0; i < 32; i++)
		{
			if (hash[i] != expected_hash[i])
			{
				match = false;
				break;
			}
		}
		REQUIRE(match);
	}

	SECTION("Empty string")
	{

		std::string text = "";
		unsigned char hash[33];
		unsigned char expected_hash[32] = { 0xe3, 0xb0, 0xc4, 0x42, 0x98, 0xfc, 0x1c, 0x14, 0x9a, 0xfb, 0xf4, 0xc8, 0x99, 0x6f, 0xb9, 0x24, 0x27, 0xae, 0x41, 0xe4, 0x64, 0x9b, 0x93, 0x4c, 0xa4, 0x95, 0x99, 0x1b, 0x78, 0x52, 0xb8, 0x55 };
		unsigned char* buffer = new unsigned char[text.length()];

		cpyStringToUnsignedCharArray(text, buffer);

		computeHash(&sha, buffer, text.length(), hash);

		bool match = true;
		for (size_t i = 0; i < 32; i++)
		{
			if (hash[i] != expected_hash[i])
			{
				match = false;
				break;
			}
		}
		REQUIRE(match);
	}

	SECTION("Long input")
	{
		std::string text = "abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu";

		unsigned char hash[33];
		unsigned char expected_hash[32] = { 0xcf, 0x5b, 0x16, 0xa7, 0x78, 0xaf, 0x83, 0x80, 0x03, 0x6c, 0xe5, 0x9e, 0x7b, 0x04, 0x92, 0x37, 0x0b, 0x24, 0x9b, 0x11, 0xe8, 0xf0, 0x7a, 0x51, 0xaf, 0xac, 0x45, 0x03, 0x7a, 0xfe, 0xe9, 0xd1 };
		unsigned char* buffer = new unsigned char[text.length()];

		cpyStringToUnsignedCharArray(text, buffer);

		computeHash(&sha, buffer, text.length(), hash);

		bool match = true;
		for (size_t i = 0; i < 32; i++)
		{
			if (hash[i] != expected_hash[i])
			{
				match = false;
				break;
			}
		}
		REQUIRE(match);
	}
}