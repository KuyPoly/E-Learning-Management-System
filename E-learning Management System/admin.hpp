#ifndef ADMIN_HPP
#define ADMIN_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <limits>
#include <cctype>
using namespace std;

class Admin {
public:
    // Add a new user to the system
    void addUser() {
        string fullName, role, password;
        while (true) {
            cout << "Enter full name (First and Last): ";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            getline(cin, fullName);

            cout << "Enter role (Admin, Teacher, or Student): ";
            getline(cin, role);

            cout << "Enter password: ";
            getline(cin, password);

            string filePath;
            if (role == "Admin") filePath = "../admin.csv";
            else if (role == "Teacher") filePath = "../teacher.csv";
            else if (role == "Student") filePath = "../student.csv";
            else {
                cout << "Error: Invalid role! Please enter 'Admin', 'Teacher', or 'Student'." << endl;
                if (exitPrompt()) return;
                continue;
            }

            string row = fullName + "," + password;
            appendToFile(filePath, row);
            cout << role << " user added successfully!" << endl;
            break;
        }
    }

    // Delete a user from the system
    void deleteUser() {
        string fullName, role;
        while (true) {
            cout << "Enter full name (First and Last) to delete: ";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            getline(cin, fullName);

            cout << "Enter role (Admin, Teacher, or Student): ";
            getline(cin, role);

            string filePath;
            if (role == "Admin") filePath = "../admin.csv";
            else if (role == "Teacher") filePath = "../teacher.csv";
            else if (role == "Student") filePath = "../student.csv";
            else {
                cout << "Error: Invalid role! Please try again." << endl;
                if (exitPrompt()) return;
                continue;
            }

            ifstream file(filePath);
            ofstream tempFile("temp.csv");
            string line;
            bool userFound = false;

            while (getline(file, line)) {
                vector<string> data = split(line, ',');
                if (data[0] == fullName) {
                    userFound = true; // Skip the user to be deleted
                } else {
                    tempFile << line << endl;
                }
            }

            file.close();
            tempFile.close();

            if (userFound) {
                remove(filePath.c_str());
                rename("temp.csv", filePath.c_str());
                cout << role << " user deleted successfully!" << endl;
            } else {
                remove("temp.csv");
                cout << "Error: User not found!" << endl;
            }
            break;
        }
    }

    // Create a new course
    void createCourse() {
        string courseName, instructor;
        while (true) {
            cout << "Enter course name: ";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            getline(cin, courseName);

            if (courseName.empty()) {
                cout << "Error: Course name cannot be empty!" << endl;
                if (exitPrompt()) return;
                continue;
            }

            if (isValidCourse(courseName)) {
                cout << "Error: Course already exists!" << endl;
                if (exitPrompt()) return;
                continue;
            }

            cout << "Enter teacher's full name: ";
            getline(cin, instructor);

            if (!isValidUser(instructor, "Teacher")) {
                cout << "Error: Invalid teacher username!" << endl;
                if (exitPrompt()) return;
                continue;
            }

            ofstream file("../course.csv", ios::app);
            if (!file) {
                cout << "Error: Unable to open courses file!" << endl;
                if (exitPrompt()) return;
                continue;
            }

            file << courseName << "," << instructor << endl;
            file.close();
            cout << "Course created successfully!" << endl;
            break;
        }
    }

    // Delete a course
    void deleteCourse() {
        string courseName;
        while (true) {
            cout << "Enter course name to delete: ";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            getline(cin, courseName);

            if (courseName.empty()) {
                cout << "Error: Course name cannot be empty!" << endl;
                continue;
            }

            ifstream courseFile("../course.csv");
            ofstream tempFile("temp.csv");
            string line;
            bool courseFound = false;

            while (getline(courseFile, line)) {
                vector<string> data = split(line, ',');
                if (data[0] == courseName) {
                    courseFound = true; // Skip the course to be deleted
                } else {
                    tempFile << line << endl;
                }
            }

            courseFile.close();
            tempFile.close();

            if (courseFound) {
                remove("../course.csv");
                rename("temp.csv", "../course.csv");
                cout << "Course deleted successfully!" << endl;
            } else {
                remove("temp.csv");
                cout << "Error: Course not found!" << endl;
            }
            return;
        }
    }

    // Enroll a student in a course
    void enrollStudent() {
        string courseName, studentName;
        while (true) {
            cout << "Enter course name: ";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            getline(cin, courseName);

            cout << "Enter student's full name: ";
            getline(cin, studentName);

            if (!isValidCourse(courseName)) {
                cout << "Error: Course does not exist!" << endl;
                if (exitPrompt()) return;
                continue;
            }

            if (!isValidUser(studentName, "Student")) {
                cout << "Error: Invalid student username!" << endl;
                if (exitPrompt()) return;
                continue;
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

            if (found) {
                remove("../course.csv");
                rename("temp.csv", "../course.csv");
                cout << "Student enrolled successfully!" << endl;
            } else {
                remove("temp.csv");
                cout << "Error: Course not found!" << endl;
            }
            break;
        }
    }

    // Drop a student from a course
    void dropStudentFromCourse() {
        string courseName, studentName;
        while (true) {
            cout << "Enter course name: ";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            getline(cin, courseName);

            cout << "Enter student's full name to drop: ";
            getline(cin, studentName);

            if (!isValidCourse(courseName)) {
                cout << "Error: Course does not exist!" << endl;
                if (exitPrompt()) return;
                continue;
            }

            ifstream file("../course.csv");
            ofstream tempFile("temp.csv");
            string line;
            bool studentDropped = false;

            while (getline(file, line)) {
                vector<string> data = split(line, ',');
                if (data[0] == courseName) {
                    tempFile << data[0] << "," << data[1];
                    for (size_t i = 2; i < data.size(); i++) {
                        if (data[i] != studentName) {
                            tempFile << "," << data[i];
                        } else {
                            studentDropped = true;
                        }
                    }
                    tempFile << endl;
                } else {
                    tempFile << line << endl;
                }
            }

            file.close();
            tempFile.close();

            if (studentDropped) {
                remove("../course.csv");
                rename("temp.csv", "../course.csv");
                cout << "Student dropped from course successfully!" << endl;
            } else {
                remove("temp.csv");
                cout << "Error: Student not found in the course!" << endl;
            }
        }
    }

    // View all courses
    void viewCourses() {
        ifstream file("../course.csv");
        if (!file.is_open()) {
            cout << "Error: Could not open file courses.csv" << endl;
            return;
        }

        string line;
        cout << "Courses List:\n";
        cout << "Course Name | Instructor | Enrolled Students\n";
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

    // Manage users menu
    void manageUsers() {
        int choice;
        do {
            cout << "\n=== User Management ===\n";
            cout << "1. Add User\n";
            cout << "2. Delete User\n";
            cout << "3. Back to Main Menu\n";
            cout << "==================\n";
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice) {
                case 1:
                    addUser();
                    break;
                case 2:
                    deleteUser();
                    break;
                case 3:
                    cout << "Returning to main menu...\n";
                    break;
                default:
                    cout << "Invalid choice!\n";
            }
        } while (choice != 3);
    }

    // Manage courses menu
    void manageCourses() {
        int choice;
        do {
            cout << "\n=== Course Management ===\n";
            cout << "1. Create Course\n";
            cout << "2. Delete Course\n";
            cout << "3. View All Courses\n";
            cout << "4. Back to Main Menu\n";
            cout << "==================\n";
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice) {
                case 1:
                    createCourse();
                    break;
                case 2:
                    deleteCourse();
                    break;
                case 3:
                    viewCourses();
                    break;
                case 4:
                    cout << "Returning to main menu...\n";
                    break;
                default:
                    cout << "Invalid choice!\n";
            }
        } while (choice != 4);
    }

    // Manage enrollments menu
    void manageEnrollments() {
        int choice;
        do {
            cout << "\n=== Enrollment Management ===\n";
            cout << "1. Enroll Student\n";
            cout << "2. Drop Student from Course\n";
            cout << "3. Back to Main Menu\n";
            cout << "==================\n";
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice) {
                case 1:
                    enrollStudent();
                    break;
                case 2:
                    dropStudentFromCourse();
                    break;
                case 3:
                    cout << "Returning to main menu...\n";
                    break;
                default:
                    cout << "Invalid choice!\n";
            }
        } while (choice != 3);
    }

private:
    // Prompt the admin to exit or retry
    bool exitPrompt() {
        char choice;
        cout << "Do you want to exit? (y/n): ";
        cin >> choice;
        return tolower(choice) == 'y';
    }

    // Utility function to split a string by a delimiter
    vector<string> split(const string &str, char delimiter) {
        vector<string> tokens;
        string token;
        istringstream tokenStream(str);
        while (getline(tokenStream, token, delimiter)) {
            tokens.push_back(token);
        }
        return tokens;
    }

    // Append a row to a file
    void appendToFile(const string &filePath, const string &row) {
        ofstream file(filePath, ios::app);
        if (file.is_open()) {
            file << row << endl;
            file.close();
        } else {
            cout << "Error: Could not open file for writing!" << endl;
        }
    }

    // Check if a course exists
    bool isValidCourse(const string &courseName) {
        ifstream file("../course.csv");
        string line;
        while (getline(file, line)) {
            vector<string> data = split(line, ',');
            if (data[0] == courseName) {
                file.close();
                return true;
            }
        }
        file.close();
        return false;
    }

    // Check if a user exists in a specific role
    bool isValidUser(const string &name, const string &role) {
        string filePath;
        if (role == "Admin") filePath = "../admin.csv";
        else if (role == "Teacher") filePath = "../teacher.csv";
        else if (role == "Student") filePath = "../student.csv";
        else return false;

        ifstream file(filePath);
        string line;
        while (getline(file, line)) {
            vector<string> data = split(line, ',');
            if (data[0] == name) {
                file.close();
                return true;
            }
        }
        file.close();
        return false;
    }
};

#endif
