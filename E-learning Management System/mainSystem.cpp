#include "admin.hpp"
#include "student.hpp"
#include "teacher.hpp"

int main(){

    //Welcome
    cout << "           ------------------------------------------------" << endl;
    cout << "           -----------------Welcome to School--------------" << endl;

    //Login
    int choice_login;
    do{
        cout << "Press 1 for Admin" << endl;
        cout << "Press 2 for Student" << endl;
        cout << "Press 3 for Teacher" << endl;
        cout << "Press 4 to Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice_login;
    }while(choice_login < 1 || choice_login > 4);

    switch(choice_login){
        case 1:
            Admin admin;
            admin.login();
            break;
        case 2:
            Student student;
            student.login();
            break;
        case 3:
            Teacher teacher;
            teacher.login();
            break;
        case 4:
            exit(0);
        default:
            cout << "Invalid choice. Please try again." << endl;
    }
}