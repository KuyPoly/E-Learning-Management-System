#ifndef STUDENT_HPP
#define STUDENT_HPP
#include <iostream>
#include <string>
#include "../utils/file.h"
#include "../utils/validate.h"

void viewEnrolledCourses(const string &studentName) {
    ifstream file("../courses.csv");
    if (!file.is_open()) {
        cout << "Error: Could not open file courses.csv" << endl;
        return;
    }

    string line;
    cout << "Your Enrolled Courses:" << endl;
    while (getline(file, line)) {
        vector<string> data = split(line, ',');
        for (size_t i = 2; i < data.size(); i++) {
            if (data[i] == studentName) {
                cout << data[0] << " (Instructor: " << data[1] << ")" << endl;
            }
        }
    }
    file.close();
}

void submitAssignment(const string &studentName) {
    string courseName, assignmentTitle, submission;
    cout << "Enter course name: ";
    cin.ignore();
    getline(cin, courseName);

    if (!isValidCourse(courseName)) {
        cout << "Error: Course does not exist!" << endl;
        return;
    }

    cout << "Enter assignment title: ";
    getline(cin, assignmentTitle);

    if (!isValidAssignment(courseName, assignmentTitle)) {
        cout << "Error: Assignment does not exist!" << endl;
        return;
    }

    cout << "Enter your submission: ";
    getline(cin, submission);

    string row = courseName + "," + assignmentTitle + "," + studentName + "," + submission;
    appendToFile("submissions.csv", row);
    cout << "Submission recorded successfully!" << endl;
}



#endif