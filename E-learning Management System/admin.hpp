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

    
};

#endif // ADMIN_HPP
