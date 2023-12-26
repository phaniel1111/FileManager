#include "Header.h"
#include "File.cpp"
#include "Totp.h"

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