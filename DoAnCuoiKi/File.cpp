#include "File.h" 
#define _CRT_SECURE_NO_WARNINGS


bool FileManager::OpenFile()
{
    string path;

    do {
        cout << "Input file path: ";
        cin >> path;
        if (endWith(path, this->extension))
            break;
        else
            cout << "Invalid file extension" << endl;
    } while (1);

    fstream file(path, ios::in | ios::binary);  // Use 'path' instead of 'filename'
    if (!file.is_open()) {
        cout << "Can't open file" << endl;
        return false;
    }

    processFilePath(path, filedir, filename);
    //cout << "File name: " << filename << endl;
    //cout << "File directory: " << filedir << endl;
    file.read(reinterpret_cast<char*>(&header), sizeof(FileHeader));
    //TESTING
    //printHeader(this->header);
    // Authentication
    authentication(byteArrayToString(header.totp, 16));
    file.close();
    return true;
}

bool FileManager::CreateFile() {
    string name;

    cout << "Input file name (ex. myFile): ";
    cin >> name;

    name += "." + this->extension;

    fstream file(name, ios::out | ios::binary);  // Use 'path' instead of 'filename'
    if (!file.is_open()) {
        cout << "Can't open file" << endl;
        return false;
    }
    _createFileHeader(this->header);

    //cout << "File name: " << filename << endl;
    //cout << "File directory: " << filedir << endl;
    file.write(reinterpret_cast<char*>(&this->header), sizeof(FileHeader));
    file.close();
    return true;

}

bool FileManager::_createFileHeader(FileHeader& header) {
    header.identifier[0] = BYTE('D');
    header.identifier[1] = BYTE('S');

    auto currentTimePoint = std::chrono::system_clock::now();
    time_t currentTime = chrono::system_clock::to_time_t(currentTimePoint);
    for (int i = 0; i < 10; ++i) {
        header.createDate[i] = static_cast<BYTE>((currentTime >> (i * 8)) & 0xFF);
        header.modifyDate[i] = static_cast<BYTE>((currentTime >> (i * 8)) & 0xFF);
    }
    string key = generateRandomBase32String(16);
    memset(header.totp, 0, sizeof(header.totp)); // Initialize the array with zeros
    memcpy(header.totp, key.c_str(), key.size());
    std::cout << "Please write down your TOTP key: " << byteArrayToString(header.totp, 16) << std::endl;

    memset(header.studentCount, 0, sizeof(header.studentCount));
    memset(header.teacherCount, 0, sizeof(header.teacherCount));

    uint32_t teacherStart = 56;
    uint32_t studentStart = 256;

    memcpy(header.teacherStartByte, &teacherStart, sizeof(teacherStart));
    memcpy(header.studentStartByte, &studentStart, sizeof(studentStart));
    return true;
}

void FileManager::printHeader(FileHeader header) {

    time_t t1 = byteArrayToTimeT(header.createDate);
    time_t t2 = byteArrayToTimeT(header.modifyDate);
    char timeBuffer1[26];
    char timeBuffer2[26];
    ctime_s(timeBuffer1, sizeof(timeBuffer1), &t1);
    ctime_s(timeBuffer2, sizeof(timeBuffer2), &t2);
    std::cout << "Identifier: " << byteArrayToString(header.identifier, 4) << std::endl;
    std::cout << "Create Date: " << timeBuffer1;
    std::cout << "Modify Date: " << timeBuffer1;
    std::cout << "TOTP: " << byteArrayToString(header.totp, 16) << std::endl;
    std::cout << "Student Count: " << byteArrayToUint32(header.studentCount) << std::endl;
    std::cout << "Teacher Count: " << byteArrayToUint32(header.teacherCount) << std::endl;
    std::cout << "Student Start Byte: " << byteArrayToUint32(header.studentStartByte) << std::endl;
    std::cout << "Teacher Start Byte: " << byteArrayToUint32(header.teacherStartByte) << std::endl;
}

Person FileManager::createPerson(
    string& id,
    string& name,
    string& birthday,
    string& joinDate,
    string& status,
    string& number,
    string& idNumber) {

    Person ps;

    // Using memset to initialize the entire ps object to zero
    memset(&ps, 0, sizeof(Person));

    // Using memcpy to copy the input strings into the corresponding members of ps
    strncpy_s(reinterpret_cast<char*>(ps.id), sizeof(ps.id), id.c_str(), _TRUNCATE);
    strncpy_s(reinterpret_cast<char*>(ps.name), sizeof(ps.name), name.c_str(), _TRUNCATE);
    strncpy_s(reinterpret_cast<char*>(ps.birthday), sizeof(ps.birthday), birthday.c_str(), _TRUNCATE);
    strncpy_s(reinterpret_cast<char*>(ps.joinDate), sizeof(ps.joinDate), joinDate.c_str(), _TRUNCATE);
    strncpy_s(reinterpret_cast<char*>(ps.status), sizeof(ps.status), status.c_str(), _TRUNCATE);
    strncpy_s(reinterpret_cast<char*>(ps.number), sizeof(ps.number), number.c_str(), _TRUNCATE);
    strncpy_s(reinterpret_cast<char*>(ps.idNumber), sizeof(ps.idNumber), idNumber.c_str(), _TRUNCATE);

    return ps;
}