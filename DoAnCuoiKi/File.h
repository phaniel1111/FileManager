// Cau tao file:
// 1. File header: 48 bytes


#ifndef FILE_H  
#define FILE_H

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
 
#include "utils.h"
#include "Totp.h"
#include "md5.h"

typedef unsigned char BYTE;
using namespace std;

//size 48 bytes
class FileHeader {
public:
    BYTE identifier[4];
    BYTE createDate[4];
    BYTE modifyDate[4];
    BYTE totp[16];
    BYTE studentCount[4];
    BYTE teacherCount[4];
    BYTE studentStartByte[4];
    BYTE teacherStartByte[4];
    BYTE unsed[4];
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


class FileManager {
public:
    FileHeader header;
    // First menu: open/ create file
    bool openFile();
    bool createFile();

    bool _createHeader(FileHeader& header);
    bool _modifyCounterInHeader(bool type); //type 0 teacher 1 student
    void printHeader(FileHeader header);

    Person _createPerson(string& id,string& name,string& birthday,string& joinDate,string& status,string& number,string& idNumber);
    Person _readPerson(int pos);
    void _readPersons(bool type, int from, int to);
    bool _writePerson(Person& ps, int pos);
    
    // Second menu: add/ delete/ modify/ print person
    void addPerson(); //type 0 teacher 1 student
    void printPersons();
    void deletePerson();
    void modifyPerson();
    void modifyTOTPKey();

    
private:
    vector<Person> students;
    vector<Person> teachers;
    string filename = "";
    string filedir = "";
    string extension = "ds";
};
#endif 
