#ifndef ADMIN_HPP
#define ADMIN_HPP

#include <iostream>
#include <sstream>
#include <fstream>
#include "../utils/chechfile.h"
#include <string>
using namespace std;

class Admin {
private:
    string name;
    string password;

public:
    // Default constructor
    Admin() = default;

    // Constructor with parameters
    Admin(string name, string password) {
        this->name = name;
        this->password = password;
    }

    // Login function
    bool login(const string& username, const string& password) {
        ifstream file("../admin.csv"); // Open the file
        checkfile(file);           // Check if the file opened successfully

        string line, user, pass;
        int linecount = 0;

        while (getline(file, line)) {
            linecount++;
            stringstream ss(line);
            getline(ss, user, ','); // Read username
            getline(ss, pass, ','); // Read password

            if (user == username && pass == password) {
                return true;
            }
        }

        return false; // Return false if no match is found
    }

    void admin_menu() {
        int admin_choice;
        do {
            cout << "\nAdmin Menu" << endl;
            cout << "Press 1 to Add User" << endl;
            cout << "Press 2 to Delete User" << endl;
            cout << "Press 3 to Logout" << endl;
            cout << "Enter your choice: ";
            cin >> admin_choice;

            switch (admin_choice) {
                case 1:
                    cout << "Add User functionality not implemented yet." << endl;
                    break;
                case 2:
                    cout << "Delete User functionality not implemented yet." << endl;
                    break;
                case 3:
                    cout << "Logging out..." << endl;
                    return; // Exit the admin menu
                default:
                    cout << "Invalid choice. Please try again." << endl;
            }
        } while (true);
    }

    void add_student (){
        
    }
};

#endif // ADMIN_HPP
