#include "File.h" 
#define _CRT_SECURE_NO_WARNINGS

// First menu: Open file
bool FileManager::openFile()
{
    string path;

    do {
        cout << "Input file path: ";
        cin >> path;
        if (endWith(path, "." + this->extension))
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
    //system("pause");
    //cout << "File name: " << filename << endl;
    //cout << "File directory: " << filedir << endl;
    file.read(reinterpret_cast<char*>(&header), sizeof(FileHeader));
    //TESTING
    //printHeader(this->header);
    // Authentication
    //authentication(byteArrayToString(header.totp, 16));

    // read teacher/student

    file.close();
    return true;
}
// First menu: Create file
bool FileManager::createFile() {
    string name;

    cout << "Input file name (ex. myFile): ";
    cin >> name;

    this->filename = name;

    fstream file(name + "." + this->extension, ios::out | ios::binary);  // Use 'path' instead of 'filename'
    if (!file.is_open()) {
        cout << "Can't open file" << endl;
        return false;
    }
    _createHeader(this->header);

    //cout << "File name: " << filename << endl;
    //cout << "File directory: " << filedir << endl;
    file.write(reinterpret_cast<char*>(&this->header), sizeof(FileHeader));
    file.close();
    return true;

}

// File header functions
bool FileManager::_createHeader(FileHeader& header) {
    header.identifier[0] = BYTE('D');
    header.identifier[1] = BYTE('S');

    auto currentTimePoint = std::chrono::system_clock::now();
    time_t currentTime = chrono::system_clock::to_time_t(currentTimePoint);
    for (int i = 0; i < 8; ++i) {
        header.createDate[i] = static_cast<BYTE>((currentTime >> (i * 8)) & 0xFF);
        header.modifyDate[i] = static_cast<BYTE>((currentTime >> (i * 8)) & 0xFF);
    }
    string key = generateRandomBase32String(16);
    memset(header.totp, 0, sizeof(header.totp)); // Initialize the array with zeros
    memcpy(header.totp, key.c_str(), key.size());
    std::cout << "Please write down your TOTP key: " << byteArrayToString(header.totp, 16) << std::endl;

    memset(header.studentCount, 0, sizeof(header.studentCount));
    memset(header.teacherCount, 0, sizeof(header.teacherCount));

    uint32_t teacherStart = 48;
    uint32_t studentStart = 248;

    memcpy(header.teacherStartByte, &teacherStart, sizeof(teacherStart));
    memcpy(header.studentStartByte, &studentStart, sizeof(studentStart));
    return true;
}
bool FileManager::_modifyCounterInHeader(bool type) {
    string path = (this->filedir == "\\") ? this->filename + ".ds" : this->filedir + this->filename + ".ds";
    fstream file(path , ios::in | ios::out | ios::binary);  // Use 'path' instead of 'filename'
    if (!file.is_open()) {
        cout << "Can't open file" << endl;
        return false;
    }
    if (type) {
        uint32_t count = byteArrayToUint32(this->header.studentCount);
        count++;
        memcpy(header.studentCount, &count, sizeof(count));
        //cout << "Student Count" << count << endl;
    }
	else {
		
        uint32_t count = byteArrayToUint32(this->header.teacherCount);
        count++;
        memcpy(header.teacherCount, &count, sizeof(count));
        //cout << "Teacher Count" << count << endl;
	}
    file.write(reinterpret_cast<char*>(&this->header), sizeof(FileHeader));
    file.close();
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

Person FileManager::_createPerson(string& id,string& name,string& birthday,string& joinDate,string& status,string& number,string& idNumber) {

    Person ps;

    // Using memset to initialize the entire ps object to zero
    memset(&ps, 0, sizeof(Person));

    // Using memcpy to copy the input strings into the corresponding members of ps
    memcpy(ps.id, id.c_str(), id.size());
    memcpy(ps.name, name.c_str(), name.size());
    memcpy(ps.birthday, birthday.c_str(), birthday.size());
    memcpy(ps.joinDate, joinDate.c_str(), joinDate.size());
    memcpy(ps.status, status.c_str(), status.size());
    memcpy(ps.number, number.c_str(), number.size());
    memcpy(ps.idNumber, idNumber.c_str(), idNumber.size());

    return ps;
}

Person FileManager::_readPerson(int pos) {
    Person ps;
    string path = (this->filedir == "\\") ? this->filename + ".ds" : this->filedir + this->filename + ".ds";
    cout << path << endl;

    fstream file(path, ios::in | ios::out | ios::binary);  // Use 'path' instead of 'filename'
    if (!file.is_open()) {
        cout << "Can't open file" << endl;
        return ps;
    }

    file.seekg(pos, std::ios::beg);
    file.read(reinterpret_cast<char*>(&ps), sizeof(ps));

    return ps;
}
void FileManager::_readPersons(bool type, int from, int to) {

    int fromByte, toByte;
    if (type)
    {
        fromByte = byteArrayToUint32(header.studentStartByte) + 80 * from;
        toByte = byteArrayToUint32(header.studentStartByte) + 80 * to;
    }
    else
    {
        fromByte = byteArrayToUint32(header.teacherStartByte) + 80 * from;
        toByte = byteArrayToUint32(header.teacherStartByte) + 80 * to;
    }
    vector<Person> pp;
    for (int i = fromByte; i <= toByte; i += 80) {
        Person ps = _readPerson(i);
        pp.push_back(ps);
    }
    if (type)
		this->students = pp;
	else
		this->teachers = pp;

}

bool FileManager::_writePerson(Person& ps, int pos) {
    string path = (this->filedir == "\\") ? this->filename + ".ds" : this->filedir + this->filename + ".ds";
    //cout << path << endl;
    fstream file(path, ios::in | ios::out | ios::binary);  // Use 'path' instead of 'filename'
    if (!file.is_open()) {
		cout << "Can't open file" << endl;
		return false;
	}

	file.seekp(pos, std::ios::beg);
	file.write(reinterpret_cast<char*>(&ps), sizeof(ps));
    return true;
}

// Meunu 2:
void FileManager::addPerson() {

    bool type = 0;
    cout << "Input type (0: teacher , 1: student): ";
    cin >> type;

    /*std::string id = "12345678";
    std::string name = "John Doe";
    std::string birthday = "19900101";
    std::string joinDate = "20220101";
    std::string status = "AT";
    std::string number = "0123456789";
    std::string idNumber = "083202005555";*/

    string id, name, birthday, joinDate, status = "AT", number, idNumber, password;
    cout << "Input id: ";
    cin >> id;
    cout << "Input name: ";
    cin >> name;
    cout << "Input birthday: ";
    cin >> birthday;
    cout << "Input join date: ";
    cin >> joinDate;
    cout << "Input phone number: ";
    cin >> number;
    cout << "Input id number: ";
    cin >> idNumber;
    cout << "Input password to encrypt phone and id number: ";
    cin >> password;
    
    number = xorStrings(password, number);
    idNumber = xorStrings(password, idNumber);

    // Call _createPerson with example data
    Person ps = _createPerson(id, name, birthday, joinDate, status, number, idNumber);

// Write the person to the file
    if (type)
    {
        int pos = byteArrayToUint32(header.studentStartByte) + (byteArrayToUint32(header.studentCount) * 80);
        if(_writePerson(ps, pos) && _modifyCounterInHeader(type))
			cout << "Student added" << endl;
        //cout << "pos" << pos << endl;
    }
    else
    {
        int pos = byteArrayToUint32(header.teacherStartByte) + (byteArrayToUint32(header.teacherCount) * 80);
        _writePerson(ps, pos);
        if(_writePerson(ps, pos) && _modifyCounterInHeader(type))
            cout << "Teacher added" << endl;
        //cout << "pos" << pos << endl;

    }
}

void FileManager::printPersons() {
	bool type = 0;
	cout << "Input type (0: teacher , 1: student): ";
	cin >> type;

	int from, to;
    int count = (type ? byteArrayToUint32(header.studentCount) : byteArrayToUint32(header.teacherCount));
    do {
        cout << "Input from: ";
        cin >> from;
        if (from < 1 || from > count) {
            cout << "Input should in range." << endl;
        }

    } while (from < 1 || from > count);
    do {
        cout << "Input to: ";
        cin >> to;
        if (to < 1 || to > count) {
            cout << "Input should in range." << endl;
        }

    } while (to < 1 || to > count);

    cout << (type ? "Student List" : "Teacher List") << std::endl;

	_readPersons(type, from - 1, to - 1);
    vector<Person> pp;
    if (type)
        pp = this->students;
    else
        pp = this->teachers;

    for (size_t i = 0; i < pp.size(); ++i) {
        auto& ps = pp[i];
        if (ps.status[0] == 'A')
        {

            cout << "Index: " << i + 1 << endl;  // Adding the index
            cout << "ID: " << byteArrayToString(ps.id, 8) << endl;
            cout << "Name: " << byteArrayToString(ps.name, 30) << endl;
            cout << "Birthday: " << byteArrayToString(ps.birthday, 8) << endl;
            cout << "Join Date: " << byteArrayToString(ps.joinDate, 8) << endl;
            cout << "Status: " << byteArrayToString(ps.status, 2) << endl;
            cout << "Number: " << byteArrayToString(ps.number, 10) << endl;
            cout << "ID Number: " << byteArrayToString(ps.idNumber, 12) << endl;
            cout << endl;
        }
    }
}

void FileManager::deletePerson() {
    int pos; 
    int posByte;
    bool type;

    cout << "Input type (0: teacher , 1: student): ";
    cin >> type;

    int count = (type ? byteArrayToUint32(header.studentCount) : byteArrayToUint32(header.teacherCount));
    do {
        cout << "Input index position: ";
        cin >> pos;

        if (pos < 1 || pos > count ) {
            cout << "Input should in range." << endl;
        }

    } while (pos < 1 || pos > count);

    pos--;
    if (type)
        posByte = byteArrayToUint32(header.studentStartByte) + 80 * pos;
    else
        posByte = byteArrayToUint32(header.teacherStartByte) + 80 * pos;

    Person ps = _readPerson(posByte);
    ps.status[0] = 'D';
    ps.status[1] = 'L';
    if (_writePerson(ps, posByte))
		cout << "Deleted successfully" << endl;
	else
		cout << "Delete failed" << endl;
    
}

void FileManager::modifyPerson() {
	int pos;
	int posByte;
	bool type;

	cout << "Input type (0: teacher , 1: student): ";
	cin >> type;
    int count = (type ? byteArrayToUint32(header.studentCount) : byteArrayToUint32(header.teacherCount));
    do {
        cout << "Input index position: ";
        cin >> pos;

        if (pos < 1 || pos > count) {
            cout << "Input should in range." << endl;
        }

    } while (pos < 1 || pos > count);

	pos--;
	if (type)
		posByte = byteArrayToUint32(header.studentStartByte) + 80 * pos;
	else
		posByte = byteArrayToUint32(header.teacherStartByte) + 80 * pos;

	Person ps = _readPerson(posByte);
	string id, name, birthday, joinDate, status, number, idNumber;
    cout << "Input id: ";
    cin >> id;
	cout << "Input name: ";
	cin >> name;
	cout << "Input birthday: ";
	cin >> birthday;
	cout << "Input join date: ";
	cin >> joinDate;
    cout << "Input status: ";
    cin >> status;

    if(id != "")  memset(&ps.id, 0, sizeof(Person));
    if(name != "")  memset(&ps.name, 0, sizeof(Person));
    if(birthday != "")  memset(&ps.birthday, 0, sizeof(Person));
    if(joinDate != "")  memset(&ps.joinDate, 0, sizeof(Person));
    if(status != "")  memset(&ps.status, 0, sizeof(Person));

    int t = 0;
    cout << "Input 1 if you want to change encrypted phone and id number. If not, input 0: ";
    cin >> t;
    if (t == 1)
    {
        cout << "Input phone number: ";
        cin >> number;
        cout << "Input id number: ";
        cin >> idNumber;

        string password;
        cout << "Input new password: ";
        cin >> password;

        number = xorStrings(password, number);
        idNumber = xorStrings(password, idNumber);

        memset(&ps.number, 0, sizeof(Person));
        memset(&ps.idNumber, 0, sizeof(Person));
    }

    memcpy(ps.id, id.c_str(), id.size());
    memcpy(ps.name, name.c_str(), name.size());
	memcpy(ps.birthday, birthday.c_str(), birthday.size());
	memcpy(ps.joinDate, joinDate.c_str(), joinDate.size());
	memcpy(ps.number, number.c_str(), number.size());
	memcpy(ps.idNumber, idNumber.c_str(), idNumber.size());

	if(_writePerson(ps, posByte))
		cout << "Modified successfully" << endl;
	else
		cout << "Modify failed" << endl;
}

void FileManager::modifyTOTPKey() {

    string path = (this->filedir == "\\") ? this->filename + ".ds" : this->filedir + this->filename + ".ds";
    fstream file(path, ios::in | ios::out | ios::binary);  // Use 'path' instead of 'filename'
    if (!file.is_open()) {
        cout << "Can't open file" << endl;
        return;
    }
    string key = generateRandomBase32String(16);
    memset(header.totp, 0, sizeof(header.totp)); // Initialize the array with zeros
    memcpy(header.totp, key.c_str(), key.size());
    std::cout << "Please write down your new TOTP key: " << byteArrayToString(header.totp, 16) << std::endl;

    file.seekp(0, std::ios::beg);
    file.write(reinterpret_cast<char*>(&this->header), sizeof(this->header));
    file.close();
}

void FileManager::viewPrivateData() {
    int pos;
    int posByte;
    bool type;

    cout << "Input type (0: teacher , 1: student): ";
    cin >> type;

    int count = (type ? byteArrayToUint32(header.studentCount) : byteArrayToUint32(header.teacherCount));
    do {
        cout << "Input index position: ";
        cin >> pos;

        if (pos < 1 || pos > count) {
            cout << "Input should in range." << endl;
        }

    } while (pos < 1 || pos > count);

    pos--;
    if (type)
        posByte = byteArrayToUint32(header.studentStartByte) + 80 * pos;
    else
        posByte = byteArrayToUint32(header.teacherStartByte) + 80 * pos;

    Person ps = _readPerson(posByte);
    
    string password;
    cout << "Input password to decrypt phone and id number: ";
    cin >> password;

    string number = xorStrings(password, byteArrayToString(ps.number, 10));
    string idNumber = xorStrings(password, byteArrayToString(ps.idNumber, 12));

    cout << "Index: " << pos << endl;  // Adding the index
    cout << "ID: " << byteArrayToString(ps.id, 8) << endl;
    cout << "Name: " << byteArrayToString(ps.name, 30) << endl;
    cout << "Birthday: " << byteArrayToString(ps.birthday, 8) << endl;
    cout << "Join Date: " << byteArrayToString(ps.joinDate, 8) << endl;
    cout << "Status: " << byteArrayToString(ps.status, 2) << endl;
    cout << "Number: " << number << endl;
    cout << "ID Number: " << idNumber << endl;
    cout << endl;
}

void FileManager::restorePerson() {
    int pos;
    int posByte;
    bool type;

    cout << "Input type (0: teacher , 1: student): ";
    cin >> type;

    int count = (type ? byteArrayToUint32(header.studentCount) : byteArrayToUint32(header.teacherCount));
    do {
        cout << "Input index position: ";
        cin >> pos;

        if (pos < 1 || pos > count) {
            cout << "Input should in range." << endl;
        }

    } while (pos < 1 || pos > count);

    pos--;
    if (type)
        posByte = byteArrayToUint32(header.studentStartByte) + 80 * pos;
    else
        posByte = byteArrayToUint32(header.teacherStartByte) + 80 * pos;

    Person ps = _readPerson(posByte);
    ps.status[0] = 'A';
    ps.status[1] = 'T';
    if (_writePerson(ps, posByte))
        cout << "Restore successfully" << endl;
    else
        cout << "Restore failed" << endl;
}