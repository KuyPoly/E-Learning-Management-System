#include "admin.hpp"
#include "student.hpp"
#include "teacher.hpp"
#include <iostream>
#include <string>
using namespace std;

// Function to ask for login choice
int ask_login() {
    int choice_login;
    do {
        cout << "Press 1 for Admin" << endl;
        cout << "Press 2 for Student" << endl;
        cout << "Press 3 for Teacher" << endl;
        cout << "Press 4 to Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice_login;
    } while (choice_login < 1 || choice_login > 4);
    return choice_login;
}

// Function for Student Menu
void studentMenu(Student& student) {
    int choice;
    do {
        cout << "Student Menu: \n";
        cout << "1. Change Password\n";
        cout << "2. View Courses\n";
        cout << "3. Submit Assignment\n";
        cout << "4. View Grades\n";
        cout << "5. View Progress\n";
        cout << "6. Logout\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                string newPassword;
                cout << "Enter new password: ";
                cin >> newPassword;
                student.changePassword(newPassword);
                break;
            }
            case 2:
                student.viewCourseMaterials();
                break;
            case 3: {
                string courseName, assignmentName;
                cout << "Enter course name: ";
                cin >> courseName;
                cout << "Enter assignment name: ";
                cin >> assignmentName;
                student.submitAssignment(courseName, assignmentName);
                break;
            }
            case 4:
                student.viewGrades();
                break;
            case 5:
                // You might have a map of progress to display here
                break;
            case 6:
                cout << "Logging out...\n";
                break;
            default:
                cout << "Invalid choice.\n";
                break;
        }
    } while (choice != 6);
}

int main() {
    // Welcome message
    cout << "           ------------------------------------------------" << endl;
    cout << "           -----------------Welcome to School--------------" << endl;

    while (true) { // Main loop
        int choice_login = ask_login(); // Get the user's choice
        string username, password;

        switch (choice_login) {
            case 1: {   
                Admin admin; // Default constructor
                bool logged_in = false;

                // Login loop
                do {
                    cout << "Enter your username: ";
                    cin >> username;
                    cout << "Enter your password: ";
                    cin >> password;

                    if (admin.login(username, password)) {
                        cout << "Login successful!" << endl;
                        logged_in = true;
                        admin.admin_menu(); // Go to Admin menu
                        break;
                    } else {
                        cout << "Invalid username or password." << endl;
                        cout << "Press 1 to try again or 2 to go back to the main menu: ";
                        int login_choice;
                        cin >> login_choice;

                        if (login_choice == 2) {
                            cout << "Returning to the main menu..." << endl;
                            break; // Exit login loop
                        }
                    }
                } while (!logged_in); // Keep looping until login is successful or user chooses to exit
                break;
            }
            case 2:
                cout << "Student functionality not implemented yet." << endl;
                break;
            case 3:
                cout << "Teacher functionality not implemented yet." << endl;
                break;
            case 4:
                cout << "Exiting the program. Goodbye!" << endl;
                return 0; // Exit the program
            default:
                cout << "Invalid choice. Please try again." << endl;
                break;
        }
    }

    return 0;
}
