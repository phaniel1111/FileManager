#include "utils.h"

bool endWith(string str, string suffix) {
    return (str.length() >= suffix.length()) && (str.compare(str.length() - suffix.length(), suffix.length(), suffix) == 0);
}

void processFilePath(const string& filePath, string& fileDir, string& fileName) {
    // Find the last occurrence of the directory separator
    size_t lastSeparator = filePath.find_last_of("\\/");

    // Extract the directory
    fileDir = (lastSeparator != string::npos) ? filePath.substr(0, lastSeparator) : "";
    fileDir += "\\";
    // Extract the filename
    size_t lastDot = filePath.find_last_of('.');
    if (lastSeparator != string::npos && lastDot != string::npos && lastDot > lastSeparator) {
        fileName = filePath.substr(lastSeparator + 1, lastDot - lastSeparator - 1);
    }
    else {
        // If there's no dot after the last separator, or if the dot is before the separator, use the entire filename
        fileName = (lastSeparator != string::npos) ? filePath.substr(lastSeparator + 1) : filePath;
    }
    if (endWith(fileName, ".ds"))
    {
        fileName = fileName.substr(0, fileName.size() - 3);
    }
}

// Function to convert byte array to readable string
string byteArrayToString(const unsigned char* array, size_t size) {
    string result;
    for (size_t i = 0; i < size; ++i) {
        result += static_cast<char>(array[i]);
    }
    return result;
}

// Function to convert byte array to uint32_t
uint32_t byteArrayToUint32(const unsigned char* array) {
    uint32_t result;
    memcpy(&result, array, sizeof(result));
    return result;
}

// Function to convert byte array to time_t
time_t byteArrayToTimeT(const unsigned char* date) {
    time_t restoredTime = 0;
    size_t length = sizeof(date) / sizeof(date[0]);
    for (int i = 0; i < 4; ++i) {
        restoredTime |= static_cast<time_t>(date[i]) << (i * 8);
    }
    return restoredTime;
}

string xorStrings(const string& str1, const string& str2) {
    string result;

    // Adjust the lengths of the strings if they are not equal
    string adjustedStr1 = str1;
    string adjustedStr2 = str2;

    if (str1.length() < str2.length()) {
        // Duplicate str1 to match the length of str2
        while (adjustedStr1.length() < str2.length()) {
            adjustedStr1 += str1;
        }
        adjustedStr1 = adjustedStr1.substr(0, str2.length());
    }
    else if (str1.length() > str2.length()) {
        // Take a substring of str1 to match the length of str2
        adjustedStr1 = str1.substr(0, str2.length());
    }

    // Perform XOR on each pair of characters
    for (size_t i = 0; i < adjustedStr1.length(); ++i) {
        result += static_cast<char>(adjustedStr1[i] ^ adjustedStr2[i]);
    }

    return result;
}
