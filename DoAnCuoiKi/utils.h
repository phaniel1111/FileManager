#ifndef UTILS_H  
#define UTILS_H

#include <string>
#include<iostream>
#include <iomanip>

using namespace std;

bool endWith(string str,string suffix);
void processFilePath(const string& filePath, string& fileDir, string& fileName);
string byteArrayToString(const unsigned char* array, size_t size);
uint32_t byteArrayToUint32(const unsigned char* array);
time_t byteArrayToTimeT(const unsigned char* array);


#endif
