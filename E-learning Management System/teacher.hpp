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

    void viewAssignmentLessons() {
    // Get the courses this teacher teaches
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

        // Read assignments and lessons for the teacher's courses
        ifstream assignmentFile("../assignments.csv");
        ifstream lessonFile("../lessons.csv");
        if (!assignmentFile.is_open() || !lessonFile.is_open()) {
            cout << "Error: Unable to open assignments or lessons file!" << endl;
            return;
        }

        vector<string> assignments;
        while (getline(assignmentFile, line)) {
            vector<string> details = split(line, ',');
            if (find(teacherCourses.begin(), teacherCourses.end(), details[0]) != teacherCourses.end()) {
                assignments.push_back(line);
            }
        }
        assignmentFile.close();

        if (assignments.empty()) {
            cout << "No assignments available for your courses." << endl;
            return;
        }

    }
};

// Lesson class
class Lesson {
public:
    string title;
    string content;
    vector<string> filePaths;
    vector<string> links;
    string details;

    Lesson(const string& title, const string& content)
        : title(title), content(content) {}

    void addFile(const string& filePath) {
        filePaths.push_back(filePath);
        cout << "File added successfully: " << filePath << "\n";
    }

    void addLink(const string& link) {
        links.push_back(link);
        cout << "Link added successfully: " << link << "\n";
    }

    void addDetails(const string& lessonDetails) {
        details = lessonDetails;
        cout << "Details added successfully!\n";
    }

    void display() const {
        cout << "\nTitle: " << title << "\nContent: " << content << "\n";

        if (!filePaths.empty()) {
            cout << "Attached Files:\n";
            for (const auto& file : filePaths) {
                cout << "- " << file << "\n";
            }
        } else {
            cout << "No files attached.\n";
        }

        if (!links.empty()) {
            cout << "Links:\n";
            for (const auto& link : links) {
                cout << "- " << link << "\n";
            }
        } else {
            cout << "No links provided.\n";
        }

        if (!details.empty()) {
            cout << "Details: " << details << "\n";
        } else {
            cout << "No additional details provided.\n";
        }
    }
};

// LessonManager class
class LessonManager {
private:
    vector<Lesson> lessons;

public:
    void addLesson(const Lesson& lesson) {
        lessons.push_back(lesson);
        cout << "Lesson added successfully: " << lesson.title << "\n";
    }

    void deleteLesson(const string& lessonTitle) {
        auto it = remove_if(lessons.begin(), lessons.end(),
                            [&lessonTitle](const Lesson& lesson) {
                                return lesson.title == lessonTitle;
                            });
        if (it != lessons.end()) {
            lessons.erase(it, lessons.end());
            cout << "Lesson deleted successfully: " << lessonTitle << "\n";
        } else {
            cout << "Lesson not found: " << lessonTitle << "\n";
        }
    }

    void displayAllLessons() const {
        if (lessons.empty()) {
            cout << "No lessons available.\n";
        } else {
            cout << "Available Lessons:\n";
            for (const auto& lesson : lessons) {
                lesson.display();
            }
        }
    }

    const vector<Lesson>& getLessons() const {
        return lessons;
    }
};

#endif