#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
using namespace std;

#include "student.hpp"
#include "teacher.hpp"
#include "admin.hpp"

// Main Menu
void displayMenu(const string &role) {
    if (role == "Admin") {
        cout << "1. Add User\n2. Create Course\n3. Enroll Student\n4. View Courses\n5.Delete Course\n6.Delete User\n7.Drop Student from course\n8. Exit\n";
    } else if (role == "Teacher") {
        cout << "\n=== Teacher Menu ===\n";
        cout << "1. Manage Lessons\n";
        cout << "2. Add Assignment\n";
        cout << "3. Grade Assignment\n";
        cout << "4. View Student Progress\n";
        cout << "5. Display Student & Course Info\n";
        cout << "6. Exit\n";
        cout << "==================\n";
        cout << "Enter your choice: ";
    } else if (role == "Student") {
        cout << "1. View Enrolled Courses\n2. Change Password\n3. View assignment\n4. Submit Assignment\n5. View grade\n6. View progress\n7. display Information\n8. Exit\n";
    }
}

int main() {
    string username, password, role;
    cout << "Welcome to the E-learning Management System" << endl;

    // Role Selection Menu
    cout << "Select your role:\n1. Admin\n2. Teacher\n3. Student\nEnter choice: ";
    int roleChoice;
    cin >> roleChoice;

    // Map role choice to file and role name
    string filename;
    if (roleChoice == 1) {
        role = "Admin";
        filename = "../admin.csv";
    } else if (roleChoice == 2) {
        role = "Teacher";
        filename = "../teacher.csv";
    } else if (roleChoice == 3) {
        role = "Student";
        filename = "../student.csv";
    } else {
        cout << "Invalid role selection. Exiting!" << endl;
        return 0;
    }

    // Authentication
    bool authenticated = false;
    do {
        cout << "Enter username: ";
        cin >> username;
        cout << "Enter password: ";
        cin >> password;

        ifstream file(filename);
        string line;
        while (getline(file, line)) {
            vector<string> data = split(line, ',');
            if (data[0] == username && data[1] == password) {
                authenticated = true;
                break;
            }
        }
        file.close();

        if (!authenticated) {
            cout << "Invalid credentials!" << endl;
            cout << "Press 1 to try again or 2 to exit: ";
            int choice;
            cin >> choice;
            if (choice == 2) {
                cout << "Exiting..." << endl;
                return 0;
            }
        }
    } while (!authenticated);

    cout << "Login successful! Welcome, " << username << " (" << role << ")" << endl;

    // Role-Based Menu and Functionalities
    int exitOption = (role == "Admin") ? 8 : (role == "Teacher" ? 6 : 8);
    int choice;

    do {
        displayMenu(role);
        cin >> choice;

        if (role == "Admin") {
            Admin admin;
            if (choice == 1) admin.addUser();
            else if (choice == 2) admin.createCourse();
            else if (choice == 3) admin.enrollStudent();
            else if (choice == 4) admin.viewCourses();
            else if (choice == 5) admin.deleteCourse();
            else if (choice == 6) admin.deleteUser();
            else if (choice == 7) admin.dropStudentFromCourse();
            else if (choice == 8) break; // Exit
        } 
        else if (role == "Teacher") {
            Teacher teacher(username, password);
            if (choice == 1) teacher.manageLessons();
            else if (choice == 2) teacher.addAssignment();
            else if (choice == 3) teacher.gradeAssignment();
            else if (choice == 4) teacher.viewProgress();
            else if (choice == 5) teacher.displayInfo();
            else if (choice == 6) break;
        } 
        else if (role == "Student") {
            Student student(username, password);
            if (choice == 1) student.viewEnrolledCourses();
            else if (choice == 2) student.changePassword();
            else if (choice == 3) student.viewAssignments();
            else if (choice == 4) student.submitAssignment();
            else if (choice == 5) student.viewGrade();
            else if (choice == 6) student.viewProgress();
            else if (choice == 7) student.displayInformation();
            else if (choice == 8) break;
        }
    } while (choice != exitOption);

    cout << "Goodbye!" << endl;
    return 0;
}