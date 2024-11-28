#ifndef ADMIN_HPP
#define ADMIN_HPP

#include <iostream>
#include <sstream>
#include <fstream>
#include "../utils/chechfile.h"
#include <string>
#include <map>
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
void admin_menu() {
    int admin_choice;
    do {
        cout << "\nAdmin Menu" << endl;
        cout << "Press 1 to View Students" << endl;
        cout << "Press 2 to View Teachers" << endl;
        cout << "Press 3 to View Courses" << endl;
        cout << "Press 4 to Assign Course to Student" << endl;
        cout << "Press 5 to Assign Course to Teacher" << endl;
        cout << "Press 6 to View Assigned Courses" << endl;
        cout << "Press 7 to Logout" << endl;
        cout << "Enter your choice: ";
        cin >> admin_choice;

        switch (admin_choice) {
            case 1:
                display_file("../student.csv", "Students");
                break;
            case 2:
                display_file("../teacher.csv", "Teachers");
                break;
            case 3:
                display_file("../course.csv", "Courses");
                break;
            case 4:
                assign_course("Student");
                break;
            case 5:
                assign_course("Teacher");
                break;
            case 6:
                view_assigned_courses();
                break;
            case 7:
                cout << "Logging out..." << endl;
                return;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    } while (true);
}


    // Display students, teachers, or courses
    void display_file(const string& filename, const string& entity) {
        ifstream file(filename);
        checkfile(file);

        string line;
        cout << "\n" << entity << " List:\n";
        while (getline(file, line)) {
            cout << line << endl;
        }
        file.close();
    }

// Assign courses to students or teachers
    void assign_course(const string& assigneeType) {
        string assigneeID, courseID;

        // Display relevant lists
        if (assigneeType == "Student") {
            display_file("../student.csv", "Students");
        } else if (assigneeType == "Teacher") {
            display_file("../teacher.csv", "Teachers");
        }
        display_file("../course.csv", "Courses");

        // Input assignee ID and course ID
        cout << "\nEnter the " << assigneeType << " ID: ";
        cin >> assigneeID;
        cout << "Enter the Course ID: ";
        cin >> courseID;

        // Save the assignment to assignments.csv
        ofstream file("../Book1.csv", ios::app);
        if (file.is_open()) {
            file << courseID << "," << assigneeID << "," << assigneeType << "\n";
            cout << assigneeType << " with ID " << assigneeID << " has been assigned to course ID " << courseID << "." << endl;
        } else {
            cerr << "Failed to open assignments file." << endl;
        }
        file.close();
    }

// Load data into a map for quick lookup
    map<string, string> load_map(const string& filename) {
        map<string, string> data_map;
        ifstream file(filename);
        checkfile(file);

        string line, id, name;
        while (getline(file, line)) {
            stringstream ss(line);
            getline(ss, id, ','); // Get ID
            getline(ss, name, ','); // Get Name
            data_map[id] = name;   // Store in map
        }
        file.close();
        return data_map;
    }


    void view_assigned_courses() {
    // Load all data into maps
    map<string, string> courses = load_map("../course.csv");
    map<string, string> students = load_map("../student.csv");
    map<string, string> teachers = load_map("../teacher.csv");

    ifstream file("../Book1.csv");
    checkfile(file);

    string line, course_id, assignee_id, role;
    cout << "\nAssigned Courses:\n";
    cout << "---------------------------------------\n";
    cout << "Course Name    | Assignee Name   | Role\n";
    cout << "---------------------------------------\n";

    while (getline(file, line)) {
        stringstream ss(line);
        getline(ss, course_id, ','); // Get Course ID
        getline(ss, assignee_id, ','); // Get Assignee ID
        getline(ss, role, ','); // Get Role

        string course_name = courses[course_id]; // Get Course Name
        string assignee_name = (role == "Student") ? students[assignee_id] : teachers[assignee_id]; // Get Name based on Role

        cout << course_name << "    | " << assignee_name << "   | " << role << endl;
    }
    file.close();
    }


};



#endif // ADMIN_HPP
