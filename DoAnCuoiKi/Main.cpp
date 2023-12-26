#include "Header.h"
#include "File.cpp"

#include "totp/libcppotp/bytes.h"
#include "totp/libcppotp/otp.h"

#include <iostream>
#include <windows.h>
#include <cctype>
#include <cstdio>

using namespace CppTotp;


std::string normalizedBase32String(const std::string& unnorm)
{
	std::string ret;

	for (char c : unnorm)
	{
		if (c == ' ' || c == '\n' || c == '-')
		{
			// skip separators
		}
		else if (std::islower(c))
		{
			// make uppercase
			char u = std::toupper(c);
			ret.push_back(u);
		}
		else
		{
			ret.push_back(c);
		}
	}

	return ret;
}

int main(void)
{
	// read the key
	std::string key;

	key = "JBSWY3DPEHPK3PXP";

	std::string normalizedKey = normalizedBase32String(key);
	Bytes::ByteString qui = Bytes::fromUnpaddedBase32(normalizedKey);

	while (1)
	{
		uint32_t p = totp(qui, time(NULL), 0, 30, 6);
		printf("%06u (%2us remain)\r", p, 30 - (time(NULL) % 30));
		fflush(stdout);
		Sleep(1000);
	}

	return 0;
}