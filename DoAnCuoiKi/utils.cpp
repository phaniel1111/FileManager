#include "utils.h"

bool endWith(string str, string suffix) {
    return (str.length() >= suffix.length()) && (str.compare(str.length() - suffix.length(), suffix.length(), suffix) == 0);
}

void processFilePath(const string& filePath, string& fileDir, string& fileName) {
    // Find the last occurrence of the directory separator
    size_t lastSeparator = filePath.find_last_of("\\/");

    // Extract the directory
    fileDir = (lastSeparator != std::string::npos) ? filePath.substr(0, lastSeparator) : "";

    // Extract the filename
    size_t lastDot = filePath.find_last_of('.');
    if (lastSeparator != std::string::npos && lastDot != std::string::npos && lastDot > lastSeparator) {
        fileName = filePath.substr(lastSeparator + 1, lastDot - lastSeparator - 1);
    }
    else {
        // If there's no dot after the last separator, or if the dot is before the separator, use the entire filename
        fileName = (lastSeparator != std::string::npos) ? filePath.substr(lastSeparator + 1) : filePath;
    }
}

// Function to convert byte array to readable string
string byteArrayToString(const unsigned char* array, size_t size) {
    std::string result;
    for (size_t i = 0; i < size; ++i) {
        result += static_cast<char>(array[i]);
    }
    return result;
}

// Function to convert byte array to uint32_t
uint32_t byteArrayToUint32(const unsigned char* array) {
    uint32_t result;
    std::memcpy(&result, array, sizeof(result));
    return result;
}

// Function to convert byte array to time_t
time_t byteArrayToTimeT(const unsigned char* date) {
    time_t restoredTime = 0;
    for (int i = 0; i < 10; ++i) {
        restoredTime |= static_cast<time_t>(date[i]) << (i * 8);
    }
    return restoredTime;
}