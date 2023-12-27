#ifndef FILE_H  // Add include guards to prevent multiple inclusion
#define FILE_H

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
 
#include "utils.h"1
#include "Totp.h"

typedef unsigned char BYTE;
using namespace std;

//size 56 bytes
class FileHeader {
public:
    BYTE identifier[4];
    BYTE createDate[10];
    BYTE modifyDate[10];
    BYTE totp[16];
    BYTE studentCount[4];
    BYTE teacherCount[4];
    BYTE studentStartByte[4];
    BYTE teacherStartByte[4];
};
// each person size 80 bytes
class Person {
public:
    BYTE id[8];
    BYTE name[32];
    BYTE birthday[8];
    BYTE joinDate[8];
    BYTE status[2];
    BYTE number[10];
    BYTE idNumber[12];
};


class Teacher : Person {};
class Student : Person {};

class FileManager {
public:
    FileHeader header;

    bool OpenFile();
    bool CreateFile();

    bool _createFileHeader(FileHeader& header);
    void printHeader(FileHeader header);

    Person createPerson(
        string& id,
        string& name,
        string& birthday,
        string& joinDate,
        string& status,
        string& number,
        string& idNumber);

    //Person readPersons(int pos, int number);
private:
    vector<Student> students;
    vector<Teacher> teachers;
    string filename;
    string filedir;
    string extension = "ds";
};
#endif 
