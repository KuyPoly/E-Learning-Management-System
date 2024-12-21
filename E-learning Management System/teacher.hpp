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

// Lesson class definition
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

// LessonManager class definition
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
};

// Teacher class definition
class Teacher {
private:
    string username;
    string password;
    LessonManager lessonManager;
       // Helper method to clean and process file paths
   string processFilePath(const string& filePath) const {
       string processedPath = filePath;
       
       // Remove leading/trailing whitespace
       processedPath.erase(0, processedPath.find_first_not_of(" \t\n\r"));
       processedPath.erase(processedPath.find_last_not_of(" \t\n\r") + 1);
       
       // Remove quotes and special characters
       processedPath.erase(remove(processedPath.begin(), processedPath.end(), '\''), processedPath.end());
       processedPath.erase(remove(processedPath.begin(), processedPath.end(), '\"'), processedPath.end());
       processedPath.erase(remove(processedPath.begin(), processedPath.end(), '&'), processedPath.end());
       
       // Replace backslashes with forward slashes
       replace(processedPath.begin(), processedPath.end(), '\\', '/');
       
       return processedPath;
   }
    bool openFile(const string& filePath) const {
       #ifdef _WIN32
           string command = "start \"\" \"" + filePath + "\"";
       #elif __APPLE__
           string command = "open \"" + filePath + "\"";
       #else
           string command = "xdg-open \"" + filePath + "\"";
       #endif
       return system(command.c_str()) == 0;
   }

public:
    Teacher(string username, string password) {
        this->username = username;
        this->password = password;
    }

    void addAssignment() {
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
       try {
           // Get teacher's courses
           vector<string> teacherCourses;
           ifstream courseFile("../course.csv");
           if (!courseFile.is_open()) {
               throw runtime_error("Unable to open course file!");
           }
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
            // Get pending submissions
           vector<vector<string>> pendingSubmissions;
           ifstream submissionFile("../submissions.csv");
           if (!submissionFile.is_open()) {
               throw runtime_error("Unable to open submissions file!");
           }
            while (getline(submissionFile, line)) {
               vector<string> submission = split(line, ',');
               if (submission.size() < 4) continue;
                // Check if submission is for teacher's course
               if (find(teacherCourses.begin(), teacherCourses.end(), submission[1]) != teacherCourses.end()) {
                   // Check if already graded
                   bool alreadyGraded = false;
                   ifstream gradeCheck("../grades.csv");
                   string gradeLine;
                   
                   while (getline(gradeCheck, gradeLine)) {
                       vector<string> gradeDetails = split(gradeLine, ',');
                       if (gradeDetails.size() >= 4 && 
                           gradeDetails[0] == submission[1] && 
                           gradeDetails[1] == submission[2] && 
                           gradeDetails[2] == submission[0]) {
                           alreadyGraded = true;
                           break;
                       }
                   }
                   gradeCheck.close();
                    if (!alreadyGraded) {
                       pendingSubmissions.push_back(submission);
                   }
               }
           }
           submissionFile.close();
            if (pendingSubmissions.empty()) {
               cout << "No pending submissions to grade!" << endl;
               return;
           }
            // Display pending submissions
           cout << "\nPending Submissions:" << endl;
           cout << "-------------------" << endl;
           for (size_t i = 0; i < pendingSubmissions.size(); ++i) {
               cout << i + 1 << ". Student: " << pendingSubmissions[i][0]
                    << ", Course: " << pendingSubmissions[i][1]
                    << ", Assignment: " << pendingSubmissions[i][2]
                    << ", Submission: " << pendingSubmissions[i][3] << endl;
           }
            // Get submission selection
           int choice;
           cout << "\nSelect submission to grade (Enter number): ";
           cin >> choice;
            if (choice < 1 || choice > static_cast<int>(pendingSubmissions.size())) {
               cout << "Invalid selection!" << endl;
               return;
           }
            vector<string>& selectedSubmission = pendingSubmissions[choice - 1];
           
           // Display submission details
           cout << "\nViewing submission for student: " << selectedSubmission[0] 
                << "\nCourse: " << selectedSubmission[1]
                << "\nAssignment: " << selectedSubmission[2]
                << "\nSubmitted file/link: " << selectedSubmission[3] << endl;
            // Process and handle submission
           string submission = processFilePath(selectedSubmission[3]);
           bool isLink = submission.find("http://") == 0 || submission.find("https://") == 0;
            cout << "\nThis is a " << (isLink ? "link" : "file") << " submission. Would you like to:" << endl;
           cout << "1. Open " << (isLink ? "in browser" : "file") << endl;
           cout << "2. Continue to grading" << endl;
           cout << "Enter choice: ";
           
           int viewChoice;
           cin >> viewChoice;
           
           if (viewChoice == 1) {
               if (!openFile(submission)) {
                   cout << "Error: Could not open the " << (isLink ? "link" : "file") << "." << endl;
                   cout << "Please check if the " << (isLink ? "link is valid" : "file exists and you have permission to access it") << "." << endl;
               }
               
               if (!isLink) {
                   cout << "\nPress Enter when you're ready to continue with grading...";
                   cin.ignore();
                   cin.get();
               }
           }
            // Grade submission
           int grade;
           cout << "\nEnter grade (0-100): ";
           cin >> grade;
            while (grade < 0 || grade > 100) {
               cout << "Invalid grade. Grade must be between 0 and 100." << endl;
               cout << "Enter grade (0-100): ";
               cin >> grade;
           }
            // Save grade
           ofstream gradeFile("../grades.csv", ios::app);
           if (!gradeFile.is_open()) {
               throw runtime_error("Unable to open grades file!");
           }
            gradeFile << selectedSubmission[1] << "," // course
                    << selectedSubmission[2] << "," // assignment
                    << selectedSubmission[0] << "," // student
                    << grade << endl;
           gradeFile.close();
            cout << "Grade recorded successfully!" << endl;
        } catch (const exception& e) {
           cout << "An error occurred: " << e.what() << endl;
       }
   }

    void manageAssignments() {
        int choice;
        do {
            cout << "\n=== Assignment Management ===\n";
            cout << "1. Add Assignment\n";
            cout << "2. Grade Assignment\n";
            cout << "3. Back to Main Menu\n";
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice) {
                case 1:
                    addAssignment();
                    break;
                case 2:
                    gradeAssignment();
                    break;
                case 3:
                    cout << "Returning to main menu...\n";
                    break;
                default:
                    cout << "Invalid choice!\n";
            }
        } while (choice != 3);
    }

    void displayStudentInfo() {
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

        void displayPersonalInfo() {
        ifstream teacherFile("../teacher.csv");
        if (!teacherFile.is_open()) {
            cout << "Error: Unable to open teacher file!" << endl;
            return;
        }

        string line;
        bool found = false;
        while (getline(teacherFile, line)) {
            vector<string> teacherData = split(line, ',');
            if (teacherData.size() >= 2 && teacherData[0] == username) {
                cout << "\nTeacher Personal Information:" << endl;
                cout << "-------------------------" << endl;
                cout << "Username: " << teacherData[0] << endl;
                cout << "Password: " << teacherData[1] << endl;
                found = true;
                break;
            }
        }
        teacherFile.close();

        if (!found) {
            cout << "Teacher information not found!" << endl;
        }
    }

        void changePassword() {
    string newPassword;
   cout << "\nEnter new password (must be at least 6 characters): ";
   cin >> newPassword;
    while (newPassword.length() < 6) {
       cout << "Password must be at least 6 characters long!" << endl;
       cout << "Enter new password: ";
       cin >> newPassword;
   }
    // Read all teachers from file
   vector<string> allTeachers;
   ifstream readFile("../teacher.csv");
   string line;
   
   while (getline(readFile, line)) {
       vector<string> teacherData = split(line, ',');
       if (teacherData[0] == username) {
           // Update this teacher's password
           allTeachers.push_back(username + "," + newPassword);
       } else {
           allTeachers.push_back(line);
       }
   }
   readFile.close();
    // Write all teachers back to file
   ofstream writeFile("../teacher.csv");
   for (const string& teacherLine : allTeachers) {
       writeFile << teacherLine << endl;
   }
   writeFile.close();
    // Update the password in current object
   this->password = newPassword;
   cout << "Password changed successfully!" << endl;
}

    void displayInformation() {
        int choice;
        do {
            cout << "\n=== Information Display ===\n";
            cout << "1. Display Personal Information\n";
            cout << "2. Display Student & Course Information\n";
            cout << "3. Change Password\n";
            cout << "4. Back to Main Menu\n";
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice) {
                case 1:
                    displayPersonalInfo();
                    break;
                case 2:
                    displayStudentInfo();  
                    break;
                case 3:
                    changePassword();
                    break;
                case 4:
                    cout << "Returning to main menu...\n";
                    break;
                default:
                    cout << "Invalid choice!\n";
            }
        } while (choice != 4);
    }

    void manageLessons() {
        int choice;
        do {
            cout << "\n=== Lesson Management ===\n";
            cout << "1. Add New Lesson\n";
            cout << "2. Delete Lesson\n";
            cout << "3. Display All Lessons\n";
            cout << "4. Back to Main Menu\n";
            cout << "Enter your choice: ";
            cin >> choice;
            cin.ignore();

            switch (choice) {
                case 1: {
                    string title, content;
                    cout << "Enter lesson title: ";
                    getline(cin, title);
                    cout << "Enter lesson content: ";
                    getline(cin, content);
                    
                    Lesson newLesson(title, content);
                    
                    char addMore;
                    cout << "Add file path? (y/n): ";
                    cin >> addMore;
                    cin.ignore();
                    if (addMore == 'y') {
                        string filePath;
                        cout << "Enter file path: ";
                        getline(cin, filePath);
                        newLesson.addFile(filePath);
                    }
                    
                    lessonManager.addLesson(newLesson);
                    break;
                }
                case 2: {
                    string title;
                    cout << "Enter lesson title to delete: ";
                    getline(cin, title);
                    lessonManager.deleteLesson(title);
                    break;
                }
                case 3:
                    lessonManager.displayAllLessons();
                    break;
                case 4:
                    cout << "Returning to main menu...\n";
                    break;
                default:
                    cout << "Invalid choice!\n";
            }
        } while (choice != 4);
    }

void viewProgress() {
    try {
        // First, get the courses this teacher teaches
        vector<string> teacherCourses;
        vector<pair<string, string>> studentsInCourses; // pairs of (course, student)
        
        ifstream courseFile("../course.csv");
        if (!courseFile.is_open()) {
            cout << "Error: Unable to open course file!" << endl;
            return;
        }

        string line;
        while (getline(courseFile, line)) {
            vector<string> courseDetails = split(line, ',');
            if (courseDetails.size() >= 2 && courseDetails[1] == username) {
                string courseName = courseDetails[0];
                teacherCourses.push_back(courseName);
                
                // Store all students in this course
                for (size_t i = 2; i < courseDetails.size(); i++) {
                    studentsInCourses.push_back({courseName, courseDetails[i]});
                }
            }
        }
        courseFile.close();

        if (teacherCourses.empty()) {
            cout << "You are not teaching any courses!" << endl;
            return;
        }

        if (studentsInCourses.empty()) {
            cout << "No students enrolled in your courses!" << endl;
            return;
        }

        // Display courses and their students
        cout << "\nYour courses and students:" << endl;
        cout << "-------------------------" << endl;
        int counter = 1;
        for (const auto& pair : studentsInCourses) {
            cout << counter << ". Course: " << pair.first << ", Student: " << pair.second << endl;
            counter++;
        }

        // Get user choice
        int choice;
        cout << "\nSelect a student (Enter number): ";
        cin >> choice;

        if (choice < 1 || choice > static_cast<int>(studentsInCourses.size())) {
            cout << "Invalid selection!" << endl;
            return;
        }

        // Get the selected course and student
        string selectedCourse = studentsInCourses[choice-1].first;
        string selectedStudent = studentsInCourses[choice-1].second;

        // Get all grades for this student in this course
        vector<pair<string, int>> assignments; // pairs of (assignment name, grade)
        ifstream gradesFile("../grades.csv");
        if (!gradesFile.is_open()) {
            cout << "Error: Unable to open grades file!" << endl;
            return;
        }
        
        while (getline(gradesFile, line)) {
            vector<string> gradeInfo = split(line, ',');
            if (gradeInfo.size() >= 4 && 
                gradeInfo[0] == selectedCourse && 
                gradeInfo[2] == selectedStudent) {
                assignments.push_back({gradeInfo[1], stoi(gradeInfo[3])});
            }
        }
        gradesFile.close();

        cout << "\nProgress Report for " << selectedStudent << " in " << selectedCourse << ":" << endl;
        cout << "------------------------------------------------" << endl;

        if (assignments.empty()) {
            cout << "No grades recorded yet for this student." << endl;
        } else {
            // Display all assignments and grades
            double total = 0;
            cout << "Graded Assignments:" << endl;
            for (size_t i = 0; i < assignments.size(); ++i) {
                cout << i + 1 << ". " << assignments[i].first << ": " << assignments[i].second << endl;
                total += assignments[i].second;
            }

            // Calculate and display average
            double average = total / assignments.size();
            cout << "\nTotal Assignments Graded: " << assignments.size() << endl;
            cout << "Average Score: " << average << endl;
        }

        // Show ungraded assignments
        ifstream assignFile("../assignments.csv");
        if (!assignFile.is_open()) {
            cout << "Error: Unable to open assignments file!" << endl;
            return;
        }

        cout << "\nPending Assignments:" << endl;
        bool hasPending = false;
        
        while (getline(assignFile, line)) {
            vector<string> assignInfo = split(line, ',');
            if (assignInfo.size() >= 2 && assignInfo[0] == selectedCourse) {
                bool isGraded = false;
                for (const auto& graded : assignments) {
                    if (graded.first == assignInfo[1]) {
                        isGraded = true;
                        break;
                    }
                }
                if (!isGraded) {
                    cout << "- " << assignInfo[1] << " (Not submitted/graded yet)" << endl;
                    hasPending = true;
                }
            }
        }
        assignFile.close();

        if (!hasPending && !assignments.empty()) {
            cout << "No pending assignments!" << endl;
        }

        // Check submission status
        if (hasPending) {
            cout << "\nSubmission Status:" << endl;
            ifstream submissionFile("../submissions.csv");
            if (!submissionFile.is_open()) {
                cout << "Error: Unable to open submissions file!" << endl;
                return;
            }

            while (getline(submissionFile, line)) {
                vector<string> subInfo = split(line, ',');
                if (subInfo.size() >= 4 && 
                    subInfo[0] == selectedStudent && 
                    subInfo[1] == selectedCourse) {
                    bool isGraded = false;
                    for (const auto& graded : assignments) {
                        if (graded.first == subInfo[2]) {
                            isGraded = true;
                            break;
                        }
                    }
                    if (!isGraded) {
                        cout << "- " << subInfo[2] << ": Submitted (File: " << subInfo[3] << ")" << endl;
                    }
                }
            }
            submissionFile.close();
        }
    } catch (const exception& e) {
        cout << "An error occurred: " << e.what() << endl;
    }
}
};

#endif