#include "Header.h"

int main()
{
	FileManager fileManager;
	
    int choice;
    bool t;
    do {
        cout << "1. Open file" << endl;
        cout << "2. Create file" << endl;
        cout << "3. Exit" << endl;  // Assuming option 3 is for exiting the loop

        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            t = fileManager.openFile();
            !t ? choice = 0 : choice = 1;
            break;
        case 2:
            t = fileManager.createFile();
            !t ? choice = 0 : system("pause");
            break;
        case 3:
            cout << "Exiting the program." << endl;
            break;
        default:
            cout << "Invalid choice. Please enter a valid option." << endl;
        }
        system("cls");
        if (choice == 1 || choice == 2) {
            break;
        }

    } while (choice != 3);  // Repeat the loop until the user chooses to exit

    do {
		cout << "1. Add student/teacher to the list" << endl;
        cout << "2. Print list of students/teachers to the list" << endl;
        cout << "3. Delete student/teacher from the list" << endl;
        cout << "4. Modify student/teacher from the list" << endl;
        cout << "5. Modify TOTP key" << endl;
		
        cout << "6. Exit" << endl;  // Assuming option 3 is for exiting the loop

		cout << "Enter your choice: ";
		cin >> choice;

		switch (choice) {
		case 1:
			fileManager.addPerson();
			break;
        case 2:
            fileManager.printPersons();
            break;
        case 3:
            fileManager.deletePerson();
            break;
        case 4:
            fileManager.modifyPerson();
            break;
        case 5:
            fileManager.modifyTOTPKey();
            break;
		case 6:
			cout << "Exiting the program." << endl;
			break;
		default:
			cout << "Invalid choice. Please enter a valid option." << endl;
		}
        system("pause");
        system("cls");

	} while (choice != 6);  // Repeat the loop until the user chooses to exit

	system("pause");
	return 0;
}