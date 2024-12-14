#ifndef ADMIN_HPP
#define ADMIN_HPP
#include "../utils/file.h"
#include "../utils/validate.h"
#include <fstream>

void addUser() {
    string username, role, password;
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter role (Teacher or Student): ";
    cin >> role;
    cout << "Enter password: ";
    cin >> password;

    string row = username + "," + password + "\n" ;
    if (role == "Teacher"){
        appendToFile("../teacher.csv", row);
        cout << "User added successfully!" << endl;
    }else if (role == "Student"){
        appendToFile("../student.csv", row);
        cout << "User added successfully!" << endl;
    }else{
        cout << "Error: Invalid role!" << endl;
    }
}

void createCourse() {
    string courseName, instructor;

    // Input course name
    cout << "Enter course name: ";
    cin.ignore();
    getline(cin, courseName);

    if (courseName.empty()) {
        cout << "Error: Course name cannot be empty!" << endl;
        return;
    }

    // Check if course already exists
    if (isValidCourse(courseName)) {
        cout << "Error: Course already exists!" << endl;
        return;
    }

    // Input instructor username
    cout << "Enter teacher's username: ";
    cin >> instructor;

    // Validate instructor username
    if (!isValidUser(instructor, "Teacher")) {
        cout << "Error: Invalid teacher username!" << endl;
        return;
    }

    // Append course data to the file
    ofstream file("../course.csv", ios::app);
    if (!file) {
        cout << "Error: Unable to open courses file!" << endl;
        return;
    }

    file << courseName << "," << instructor << endl;
    file.close();
    cout << "Course created successfully!" << endl;
}

void enrollStudent() {
    string courseName, studentName;
    cout << "Enter course name: ";
    cin.ignore();
    getline(cin, courseName);
    cout << "Enter student's username: ";
    cin >> studentName;

    if (!isValidCourse(courseName)) {
        cout << "Error: Course does not exist!" << endl;
        return;
    }
    if (!isValidUser(studentName, "Student")) {
        cout << "Error: Invalid student username!" << endl;
        return;
    }

    ifstream file("../course.csv");
    ofstream tempFile("temp.csv");
    string line;

    bool found = false;
    while (getline(file, line)) {
        vector<string> data = split(line, ',');
        if (data[0] == courseName) {
            found = true;
            tempFile << line << "," << studentName << endl;
        } else {
            tempFile << line << endl;
        }
    }
    file.close();
    tempFile.close();
    remove("../course.csv");
    rename("temp.csv", "../course.csv");

    if (found) {
        cout << "Student enrolled successfully!" << endl;
    } else {
        cout << "Course not found!" << endl;
    }
}

void viewCourses() {
    ifstream file("../course.csv");
    if (!file.is_open()) {
        cout << "Error: Could not open file courses.csv" << endl;
        return;
    }

    string line;
    cout << "Courses List:" << endl;
    cout << "Course Name | Instructor | Enrolled Students" << endl;
    while (getline(file, line)) {
        vector<string> data = split(line, ',');
        cout << data[0] << " | " << data[1];
        for (size_t i = 2; i < data.size(); i++) {
            cout << " | " << data[i];
        }
        cout << endl;
    }
    file.close();
}


#endif