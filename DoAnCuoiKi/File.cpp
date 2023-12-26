#include "File.h"

//size 32 bytes
class FileHeader {
    char createDate[8];
    char modifyDate[8];
    char totp[16];
    unsigned int studentCount;
    unsigned int teacherCount;
    unsigned int studentStartByte;
    unsigned int teacherStartByte;
};
// each person size 80 bytes
class Person {
    char id[8];
    char name[32];
    char birthday[8];
    char joinDate[8];
    char status[2];
    char number[10];
    char idNumber[12];
};

class Student : Person {};

class Teacher : Person {};

class FileManager {
public:
    void OpenFile(const std::string& filename);
    void ReadFile();
    void WriteFile();
    void CloseFile();

private:
    vector<Student> students;
    vector<Teacher> teachers;
    FileHeader header;
    string filename;
};