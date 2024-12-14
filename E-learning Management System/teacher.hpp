#ifndef TEACHER_HPP
#define TEACHER_HPP
#include "../utils/file.h"
#include "../utils/validate.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;

// Utility function for splitting strings
vector<string> splitLine(const string &line, char delimiter) {
    vector<string> tokens;
    stringstream ss(line);
    string token;
    while (getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

// Teacher class definition
class Teacher {
private:
    string username;
    string password;

public:
    Teacher(string username, string password) {
        this->username = username;
        this->password = password;
    }


void addAssignment() {
    // First, get the courses this teacher teaches
    vector<string> teacherCourses;
    ifstream courseFile("../course.csv");
    string line;
    while (getline(courseFile, line)) {
        vector<string> courseDetails = split(line, ',');
        if (courseDetails.size() >= 2 && courseDetails[1] == username) {
            teacherCourses.push_back(courseDetails[0]);
        }
    }
    courseFile.close();

    if (teacherCourses.empty()) {
        cout << "You are not assigned to any courses!" << endl;
        return;
    }

    // Display teacher's courses
    cout << "\nYour courses:" << endl;
    for (size_t i = 0; i < teacherCourses.size(); ++i) {
        cout << i + 1 << ". " << teacherCourses[i] << endl;
    }

    int courseChoice;
    cout << "Select a course (Enter number): ";
    cin >> courseChoice;

    if (courseChoice < 1 || courseChoice > static_cast<int>(teacherCourses.size())) {
        cout << "Invalid course selection!" << endl;
        return;
    }

    string courseName = teacherCourses[courseChoice - 1];
    string assignmentTitle;
    cout << "Enter assignment title: ";
    cin.ignore();
    getline(cin, assignmentTitle);

    ofstream file("../assignments.csv", ios::app);
    file << courseName << "," << assignmentTitle << endl;
    file.close();

    cout << "Assignment added successfully!" << endl;
}

void gradeAssignment() {
    // First, get the courses this teacher teaches
    vector<string> teacherCourses;
    ifstream courseFile("../course.csv");
    string line;
    while (getline(courseFile, line)) {
        vector<string> courseDetails = split(line, ',');
        if (courseDetails.size() >= 2 && courseDetails[1] == username) {
            teacherCourses.push_back(courseDetails[0]);
        }
    }
    courseFile.close();

    if (teacherCourses.empty()) {
        cout << "You are not assigned to any courses!" << endl;
        return;
    }
    // Read assignments for teacher's courses only
    ifstream assignFile("../assignments.csv");
    if (!assignFile.is_open()) {
        cout << "Error: Unable to open assignments file!" << endl;
        return;
    }

    vector<string> validAssignments;
    while (getline(assignFile, line)) {
        vector<string> details = split(line, ',');
        if (find(teacherCourses.begin(), teacherCourses.end(), details[0]) != teacherCourses.end()) {
            validAssignments.push_back(line);
        }
    }
    assignFile.close();

    if (validAssignments.empty()) {
        cout << "No assignments available to grade in your courses." << endl;
        return;
    }

    cout << "Available assignments in your courses:" << endl;
    for (size_t i = 0; i < validAssignments.size(); ++i) {
        vector<string> details = split(validAssignments[i], ',');
        cout << i + 1 << ". Course: " << details[0] << ", Title: " << details[1] << endl;
    }

    int choice;
    cout << "Select an assignment to grade (Enter number): ";
    cin >> choice;

    if (choice < 1 || choice > static_cast<int>(validAssignments.size())) {
        cout << "Invalid choice!" << endl;
        return;
    }

    vector<string> assignmentDetails = split(validAssignments[choice - 1], ',');
    string courseName = assignmentDetails[0];
    string assignmentTitle = assignmentDetails[1];

    // Get list of students enrolled in this course
    vector<string> enrolledStudents;
    courseFile.open("../course.csv");
    while (getline(courseFile, line)) {
        vector<string> courseDetails = split(line, ',');
        if (courseDetails[0] == courseName) {
            for (size_t i = 2; i < courseDetails.size(); i++) {
                enrolledStudents.push_back(courseDetails[i]);
            }
            break;
        }
    }
    courseFile.close();

    cout << "\nEnrolled students in " << courseName << ":" << endl;
    for (const auto& student : enrolledStudents) {
        cout << "- " << student << endl;
    }

    string studentName;
    cout << "\nEnter student name: ";
    cin.ignore();
    getline(cin, studentName);

    if (find(enrolledStudents.begin(), enrolledStudents.end(), studentName) == enrolledStudents.end()) {
        cout << "Error: Student is not enrolled in this course!" << endl;
        return;
    }

    int grade;
    cout << "Enter grade (0-100): ";
    cin >> grade;

    if (grade < 0 || grade > 100) {
        cout << "Invalid grade. Must be between 0 and 100!" << endl;
        return;
    }

    ofstream gradeFile("../grades.csv", ios::app);
    gradeFile << courseName << "," << assignmentTitle << "," << studentName << "," << grade << endl;
    gradeFile.close();

    cout << "Grade recorded successfully!" << endl;
}
void displayInfo() {
    ifstream courseFile("../course.csv");
    if (!courseFile.is_open()) {
        cout << "Error: Unable to open course file!" << endl;
        return;
    }

    cout << "\nCourse Information:" << endl;
    cout << "--------------------" << endl;
    string line;
    bool foundCourse = false;
    
    while (getline(courseFile, line)) {
        vector<string> courseDetails = split(line, ',');
        if(courseDetails.size() >= 2 && courseDetails[1] == username) {
            foundCourse = true;
            cout << "Course: " << courseDetails[0] << endl;
            cout << "Teacher: " << courseDetails[1] << endl;
            cout << "Enrolled Students: ";
            // Start from index 2 to skip course name and teacher name
            for (size_t i = 2; i < courseDetails.size(); ++i) {
                cout << courseDetails[i];
                if (i != courseDetails.size() - 1) cout << ", ";
            }
            cout << endl << "--------------------" << endl;
        }
    }
    courseFile.close();

    if (!foundCourse) {
        cout << "You are not teaching any courses." << endl;
    }
}
};

#endif