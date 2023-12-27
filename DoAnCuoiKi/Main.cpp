#include "Header.h"

int main()
{
	FileManager fileManager;
	
    int choice;
    do {
        cout << "1. Open file" << endl;
        cout << "2. Create file" << endl;
        cout << "3. Exit" << endl;  // Assuming option 3 is for exiting the loop

        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            fileManager.OpenFile();
            break;
        case 2:
            fileManager.CreateFile();
            break;
        case 3:
            cout << "Exiting the program." << endl;
            break;
        default:
            cout << "Invalid choice. Please enter a valid option." << endl;
        }

        if (choice == 1 || choice == 2) {
            break;
        }

    } while (choice != 3);  // Repeat the loop until the user chooses to exit


	system("pause");
	return 0;
}