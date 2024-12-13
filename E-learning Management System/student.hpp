#ifndef STUDENT_HPP
#define STUDENT_HPP
#include <iostream>
#include <string>
#include "../utils/file.h"
#include "../utils/validate.h"

class Student {
private:
    string username;
    string password;

public:
    Student(const string& username, const string& password) : username(username), password(password) {}

    void viewEnrolledCourses() const {
        ifstream file("../course.csv");
        if (!file.is_open()) {
            cout << "Error: Could not open file course.csv" << endl;
            return;
        }

        string line;
        cout << "Your Enrolled Courses:" << endl;
        while (getline(file, line)) {
            vector<string> data = split(line, ',');
            for (size_t i = 2; i < data.size(); i++) {
                if (data[i] == username) {
                    cout << "Courses List:" << endl;
                    cout << "--------------------------------------------" << endl;
                    cout << "Course ID | Instructor Name" << endl;
                    cout << "--------------------------------------------" << endl;
                    cout << data[0] << "  |  " << data[1] << endl;
                    cout << "--------------------------------------------" << endl;
                }
            }
        }
        file.close();
    }

    void changePassword() {
        string old_password, new_password, confirm_password;
        bool exit_loop = false;

        do {
            cout << "Enter your old password: ";
            cin >> old_password;

            ifstream file("../student.csv");
            if (!file.is_open()) {
                cout << "Error: Could not open file student.csv" << endl;
                return;
            }

            vector<string> lines;
            string line;
            bool password_updated = false;

            while (getline(file, line)) {
                vector<string> data = split(line, ',');
                if (data[0] == username) {
                    if (data[1] == old_password) {
                        cout << "Enter new password: ";
                        cin >> new_password;
                        cout << "Confirm new password: ";
                        cin >> confirm_password;

                        if (new_password != confirm_password) {
                            cout << "Passwords do not match!" << endl;
                            lines.push_back(line);
                            continue;
                        }

                        lines.push_back(data[0] + "," + new_password);
                        cout << "Password updated successfully!" << endl;
                        password_updated = true;
                    } else {
                        cout << "Old password is incorrect!" << endl;
                        lines.push_back(line);
                    }
                } else {
                    lines.push_back(line);
                }
            }

            file.close();

            if (password_updated) {
                ofstream outFile("../student.csv");
                if (!outFile.is_open()) {
                    cerr << "Error: Unable to write to file!" << endl;
                    return;
                }

                for (const string& updatedLine : lines) {
                    outFile << updatedLine << endl;
                }

                outFile.close();
                exit_loop = true;
            } else {
                cout << "Press 1 to try again or 2 to exit: ";
                int choice;
                cin >> choice;
                if (choice == 2) {
                    exit_loop = true;
                }
            }
        } while (!exit_loop);
    }

    void viewAssignments() const {
        ifstream file("../assignments.csv");

        if (!file.is_open()) {
            cout << "Could not open file assignments.csv" << endl;
            return;
        }

        string line;
        cout << "Assignments List:" << endl;
        cout << "--------------------------------------------" << endl;
        cout << "Course ID | Assignment" << endl;
        cout << "--------------------------------------------" << endl;

        while (getline(file, line)) {
            vector<string> data = split(line, ',');
            if (data.size() < 2) {
                continue;
            }

            string course_id = data[0];
            string assignment_name = data[1];

            cout << course_id << " | " << assignment_name << endl;
            cout << "--------------------------------------------" << endl;
        }

        file.close();
    }

    void submitAssignment() const {
        ifstream assignmentsFile("../assignments.csv");

        if (!assignmentsFile.is_open()) {
            cout << "Error: Could not open file assignments.csv" << endl;
            return;
        }

        string line;
        vector<pair<string, string>> coursesAssignments;

        while (getline(assignmentsFile, line)) {
            vector<string> data = split(line, ',');
            if (data.size() >= 2) {
                string course_id = data[0];
                string assignment_name = data[1];
                coursesAssignments.push_back({course_id, assignment_name});
            }
        }

        assignmentsFile.close();

        cout << "Available Assignments:" << endl;
        cout << "---------------------------------------------" << endl;
        cout << "No | Course ID | Assignment Name" << endl;
        cout << "---------------------------------------------" << endl;

        for (size_t i = 0; i < coursesAssignments.size(); ++i) {
            cout << i + 1 << " | " << coursesAssignments[i].first << " | " << coursesAssignments[i].second << endl;
        }

        int choice;
        cout << "Enter the number of the assignment to submit: ";
        cin >> choice;
        if (choice < 1 || choice > coursesAssignments.size()) {
            cout << "Invalid choice!" << endl;
            return;
        }

        string selected_course = coursesAssignments[choice - 1].first;
        string selected_assignment = coursesAssignments[choice - 1].second;

        string submission_text;
        cout << "Enter your submission for " << selected_assignment << ": " << endl;
        cin.ignore();
        getline(cin, submission_text);

        ofstream submissionsFile("../submissions.csv", ios::app);

        if (!submissionsFile.is_open()) {
            cout << "Error: Could not open file submissions.csv" << endl;
            return;
        }

        submissionsFile << username << "," << selected_course << "," << selected_assignment << "," << submission_text << endl;
        cout << "Assignment submitted successfully!" << endl;

        submissionsFile.close();
    }

    
};



#endif