// Cau tao file:
// 1. File header: 48 bytes
// 2 teacher list = 1/ 10 size of student list (input when create file)
// 3. Student list: user input
// person size = 80 bytes
// Delete item is just change status to "DL" not remove permanently
// counting variables is always go up, not go down when delete item
// Restore item is just change status to "AC"
// all input for searching is use by index (then calculate to bytes = index * 80) in file, not by other attributes
// idnex is shown when print student/teacher list. 
// Items which are deleted will not be shown, and index will not be changed, it may show 1->2->4 (3 was deleted)
// used TOTP for file security, time limit is 10s for 3 wrong password, time limit * 2 each 3 times wrong password
// used XOR for phone and id number with password, password is set by user. if forgot password, can't view private data

#ifndef FILE_H  
#define FILE_H

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>

#include "utils.h"
#include "Totp.h"

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
    bool _modifyCounterInHeader(bool type); //type: 0 teacher 1 student
    bool _modifyDateHeader();
    void printHeader();

    Person _createPerson(string& id,string& name,string& birthday,string& joinDate,string& status,string& number,string& idNumber);
    Person _readPerson(int pos);
    void _readPersons(bool type, int from, int to);
    bool _writePerson(Person& ps, int pos);
    
    // Second menu: add/ delete/ modify/ print person
    void addPerson(); //type: 0 teacher 1 student
    void printPersons(); // Print list of students/ teachers
    void deletePerson(); // Delete a  student/ teacher
    void modifyPerson(); // Modify a student/ teacher
    void modifyTOTPKey(); // Modify TOTP key
    void viewPrivateData(); // View private data of a student/ teacher using password that user had set
    void restorePerson(); //  Restore a deleted student/ teacher by their index in the file, not by their ID.
    
private:
    vector<Person> students;
    vector<Person> teachers;
    string filename = "";
    string filedir = "";
    string extension = "ds";
};
#endif 
