#pragma once

#include "totp/bytes.h"
#include "totp/otp.h"

#include <iostream>
#include <windows.h>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <ctime>

using namespace CppTotp;

std::string normalizedBase32String(const std::string& unnorm);
bool isValidTOTP(const std::string& key, std::string strCode);
std::string generateRandomBase32String(size_t size);