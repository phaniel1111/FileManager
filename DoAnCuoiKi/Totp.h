#ifndef TOTP_H  // Add include guards to prevent multiple inclusion
#define TOTP_H

#include "totp/bytes.h"
#include "totp/otp.h"

#include <iostream>
#include <windows.h>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <time.h>


using namespace CppTotp;

std::string normalizedBase32String(const std::string& unnorm);
bool isValidTOTP(const std::string& key, std::string strCode);
std::string generateRandomBase32String(size_t size);
bool authentication(std::string key);
#endif