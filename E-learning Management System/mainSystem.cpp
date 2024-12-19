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
        cout << "\n=== Admin Menu ===\n";
        cout << "1. User Management\n";
        cout << "2. Course Management\n";
        cout << "3. Enrollment Management\n";
        cout << "4. Exit\n";
        cout << "==================\n";
        cout << "Enter your choice: ";
    } 
    else if (role == "Teacher") {
        cout << "\n=== Teacher Menu ===\n";
        cout << "1. Manage Lessons\n";
        cout << "2. Manage Assignments\n";
        cout << "3. View Student Progress\n";
        cout << "4. Display Information\n";
        cout << "5. Exit\n";
        cout << "==================\n";
        cout << "Enter your choice: ";
    } 
    else if (role == "Student") {
        cout << "\n=== Student Menu ===\n";
        cout << "1. Assignment Management\n";
        cout << "2. Profile Management\n";
        cout << "3. Progress Tracking\n";
        cout << "4. Exit\n";
        cout << "==================\n";
        cout << "Enter your choice: ";
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
    int exitOption = (role == "Admin") ? 8 : (role == "Teacher" ? 5 : 8);
    int choice;

do {
    displayMenu(role);
    cin >> choice;

    if (role == "Admin") {
        Admin admin;
        switch (choice) {
            case 1:
                admin.manageUsers();  
                break;
            case 2:
                admin.manageCourses();  
                break;
            case 3:
                admin.manageEnrollments();  
                break;
            case 4:
                cout << "Goodbye!" << endl;
                return 0;
            default:
                cout << "Invalid choice!" << endl;
        }
    } 
    else if (role == "Teacher") {
        Teacher teacher(username, password);
        switch (choice) {
            case 1:
                teacher.manageLessons();
                break;
            case 2:
                teacher.manageAssignments();
                break;
            case 3:
                teacher.viewProgress();
                break;
            case 4:
                teacher.displayInformation();
                break;
            case 5:
                cout << "Goodbye!" << endl;
                return 0;
            default:
                cout << "Invalid choice!" << endl;
        }
    }
    else if (role == "Student") {
        Student student(username, password);
        switch (choice) {
            case 1:
                student.manageAssignments();  // New grouped function
                break;
            case 2:
                student.manageProfile();  // New grouped function
                break;
            case 3:
                student.viewAcademicProgress();  // New grouped function
                break;
            case 4:
                cout << "Goodbye!" << endl;
                return 0;
            default:
                cout << "Invalid choice!" << endl;
        }
    }
} while (true);

    cout << "Goodbye!" << endl;
    return 0;
}