#ifndef VALIDATE
#define VALIDATE
#include <vector>
using namespace std;


// Validate if a user exists with the correct role
bool isValidUser(const string &username, const string &role) {
    ifstream file("users.csv");
    string line;
    while (getline(file, line)) {
        vector<string> data = split(line, ',');
        if (data[0] == username && data[1] == role) {
            return true;
        }
    }
    file.close();
    return false;
}

// Validate if a course exists
bool isValidCourse(const string &courseName) {
    ifstream file("courses.csv");
    string line;
    while (getline(file, line)) {
        vector<string> data = split(line, ',');
        if (data[0] == courseName) {
            return true;
        }
    }
    file.close();
    return false;
}

// Validate if an assignment exists for a course
bool isValidAssignment(const string &courseName, const string &assignmentTitle) {
    ifstream file("assignments.csv");
    string line;
    while (getline(file, line)) {
        vector<string> data = split(line, ',');
        if (data[0] == courseName && data[1] == assignmentTitle) {
            return true;
        }
    }
    file.close();
    return false;
}

#endif