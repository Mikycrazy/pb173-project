#pragma once

#include <stdio.h>
#include <iostream>
#include "CertificationAuthority.h"

int main(void)
{
	char * oldS = "1234567890";
	std::string s = "1234567890";
	std::cout << s << std::endl;
	s[2] = 0;
	std::cout << s << std::endl;
	int b = strcmp(oldS,s.c_str());
	std::cout << b << std::endl;
	std::cin >> s;
	return 0;
}
