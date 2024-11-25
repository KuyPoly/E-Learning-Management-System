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

// Admin features menu
void admin_menu(Admin& admin) {
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
                        admin_menu(admin); // Go to Admin menu
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
