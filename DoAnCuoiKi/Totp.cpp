#include "Totp.h"

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

bool isValidTOTP(const std::string& key, std::string strCode)
{
	if(strCode.length() != 6)
		return false;
	uint32_t result = std::stoi(strCode);

	std::string normalizedKey = normalizedBase32String(key);
	Bytes::ByteString qui = Bytes::fromUnpaddedBase32(normalizedKey);

	uint32_t p = totp(qui, time(NULL), 0, 30, 6);

	if (p == result)
		return true;

	return false;
}

std::string generateRandomBase32String(size_t size) {
	const std::string charset = "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567";
	std::string result;

	srand(static_cast<unsigned>(time(0)));  // Seed the random number generator

	for (size_t i = 0; i < size; ++i) {
		result += charset[rand() % charset.size()];
	}

	return result;
}

/* Example
int main()
{
	string code;
	string key = generateRandomBase32String(16);
	cout << key << endl;
	do {
		cout << "Input code: ";
		cin >> code;
	} while (!isValidTOTP(key, code));
	cout << "Authenticated!";

	return 0;
}

*/

bool authentication(std::string key) {

	std::string code;

	int t = 3;
	int index = 10;
	int timelock = index;

	do {
		std::cout << "Input code: ";
		std::cin >> code;
		if (isValidTOTP(key, code)) break;

		t -= 1;
		std::cout << "Wrong " << 3 - t << " time(s)." << std::endl;
		if (t == 0) {
			while (timelock)
			{
				printf("Please try again in %2ld seconds \r", timelock -=1);
				fflush(stdout);
				Sleep(1000);
			}
			t = 3;
			index *= 2;
			timelock = index;
			system("cls");
		}
	} while (1);
	std::cout << "Authenticated!";
	return true;
};

