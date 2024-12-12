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
        cout << "1. Add User\n2. Create Course\n3. Enroll Student\n4. View Courses\n5. Exit\n";
    } else if (role == "Teacher") {
        cout << "1. Add Assignment\n2. Exit\n";
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
    do{
        cout << "Enter username: ";
        cin >> username;
        cout << "Enter password: ";
        cin >> password;

        bool authenticated = false;
        ifstream file(filename);
        string line;
        while (getline(file, line)) {
            vector<string> data = split(line, ','); // Assuming file format: username,password
            if (data[0] == username && data[1] == password) {
                authenticated = true;
                break;
            }
        }
        file.close();

        if (!authenticated) {
            cout << "Invalid credentials!" << endl;
            cout<< "Press 1 to try again or 2 to exit!";
            int choice;
            cin>>choice;
            if(choice==2){
                cout<<"Exiting..."<<endl;
                break;//exit the loop directly
            }
        }else{
             cout << "Login successful! Welcome, " << username << " (" << role << ")" << endl;
                 // Role-Based Menu and Functionalities

            int exitOption = 0; // Define an exit option based on role
            if (role == "Admin") exitOption = 5;
            else if (role == "Teacher") exitOption = 2;
            else if (role == "Student") exitOption = 8;

            int choice;
            do {
                displayMenu(role); // Display role-specific menu
                cout << "Enter your choice: ";
                cin >> choice;

                if (role == "Admin") {
                    if (choice == 1) addUser();
                    else if (choice == 2) createCourse();
                    else if (choice == 3) enrollStudent();
                    else if (choice == 4) viewCourses();
                    else if (choice == 5) break; // Exit
                } else if (role == "Teacher") {
                    if (choice == 1) teacher.addAssignment();
                    else if (choice == 2) break; // Exit
                } else if (role == "Student") {
                    if (choice == 1) viewEnrolledCourses(username);
                    else if (choice == 2) changePassword(username);
                    else if(choice == 3) viewAssigment();
                    else if(choice == 4) submitAssignment(username);
                    else if(choice == 5) viewGrade(username);
                    else if(choice == 6) viewProgress(username);
                    else if(choice == 7) displayInformation(username);
                    else if (choice == 8) break; // Exit
                } else {
                    cout << "Invalid choice. Try again!" << endl;
                }
            } while (choice != exitOption); // Exit condition

        cout << "Goodbye!" << endl;
            return 0;
        }
    }while(!authenticated);

}