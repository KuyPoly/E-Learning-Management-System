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
    string name;

public:
    // Constructor
    Teacher(const string &teacherName) : name(teacherName) {}

    // Function to add an assignment
    void addAssignment() {
        string courseName, assignmentTitle;
        cout << "Enter course name: ";
        cin.ignore();
        getline(cin, courseName);

        if (!isValidCourse(courseName)) {
            cout << "Error: Course does not exist!" << endl;
            return;
        }

        cout << "Enter assignment title: ";
        getline(cin, assignmentTitle);

        string row = courseName + "," + assignmentTitle;
        appendToFile("../assignments.csv", row);
        cout << "Assignment added successfully!" << endl;
    }

    // Function to grade an assignment
    void gradeAssignment() {
        // Read assignments
        ifstream file("../assignments.csv");
        if (!file.is_open()) {
            cout << "Error: Unable to open assignments file!" << endl;
            return;
        }

        vector<string> assignments;
        string line;
        while (getline(file, line)) {
            assignments.push_back(line);
        }
        file.close();

        // Check if there are any assignments
        if (assignments.empty()) {
            cout << "No assignments available to grade." << endl;
            return;
        }

        // Display assignments
        cout << "Available assignments:" << endl;
        for (size_t i = 0; i < assignments.size(); ++i) {
            vector<string> details = splitLine(assignments[i], ',');
            cout << i + 1 << ". Course: " << details[0] << ", Title: " << details[1] << endl;
        }

        // Select assignment
        int choice;
        cout << "Select an assignment to grade (Enter number): ";
        cin >> choice;

        if (choice < 1 || choice > assignments.size()) {
            cout << "Invalid choice!" << endl;
            return;
        }

        string selectedAssignment = assignments[choice - 1];
        vector<string> assignmentDetails = splitLine(selectedAssignment, ',');

        string courseName = assignmentDetails[0];
        string assignmentTitle = assignmentDetails[1];

        // Enter student name and grade
        string studentName;
        int grade;
        cout << "Enter student name: ";
        cin.ignore();
        getline(cin, studentName);

        cout << "Enter grade (0-100): ";
        cin >> grade;

        if (grade < 0 || grade > 100) {
            cout << "Invalid grade. Must be between 0 and 100!" << endl;
            return;
        }

        // Append grade to file
        string gradeRecord = courseName + "," + assignmentTitle + "," + studentName + "," + to_string(grade);
        appendToFile("../grades.csv", gradeRecord);
        cout << "Grade recorded successfully!" << endl;
    }

    // Function to display student and course information
    void displayInfo() {
        // Read student information
        ifstream studentFile("../student.csv");
        if (!studentFile.is_open()) {
            cout << "Error: Unable to open student file!" << endl;
            return;
        }

        cout << "\nStudent Information:" << endl;
        cout << "---------------------" << endl;
        string line;
        while (getline(studentFile, line)) {
            vector<string> studentDetails = splitLine(line, ',');
            cout << "Username: " << studentDetails[0] << ", Password: " << studentDetails[1] << endl;
        }
        studentFile.close();
        
        // Read course information
        ifstream courseFile("../course.csv");
        if (!courseFile.is_open()) {
            cout << "Error: Unable to open course file!" << endl;
            return;
        }

        cout << "\nCourse Information:" << endl;
        cout << "--------------------" << endl;
        while (getline(courseFile, line)) {
            vector<string> courseDetails = splitLine(line, ',');
            cout << "Course: " << courseDetails[0] << ", Enrolled Students: ";
            for (size_t i = 1; i < courseDetails.size(); ++i) {
                cout << courseDetails[i];
                if (i != courseDetails.size() - 1) {
                    cout << ", ";
                }
            }
            cout << endl;
        }
        courseFile.close();
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