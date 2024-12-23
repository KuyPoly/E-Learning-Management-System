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
#include <map>

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
    string course;

    Lesson(const string& title, const string& content, const string& course = "")
        : title(title), content(content), course(course) {}

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
        cout << "\nCourse: " << course << "\nTitle: " << title << "\nContent: " << content << "\n";
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
    map<string, vector<Lesson>> courseLessons;
    vector<Lesson> lessons;

public:
    string selectCourse() {
        ifstream courseFile("../course.csv");
        if (!courseFile.is_open()) {
            cout << "Error: Unable to open course file.\n";
            return "";
        }

        vector<string> courses;
        string line;
        while (getline(courseFile, line)) {
            vector<string> courseData = splitLine(line, ',');
            if (!courseData.empty()) {
                courses.push_back(courseData[0]);
            }
        }
        courseFile.close();

        if (courses.empty()) {
            cout << "No courses available.\n";
            return "";
        }

        cout << "Available Courses:\n";
        for (size_t i = 0; i < courses.size(); ++i) {
            cout << i + 1 << ". " << courses[i] << "\n";
        }

        int choice;
        cout << "Select a course by number: ";
        cin >> choice;

        if (choice < 1 || choice > courses.size()) {
            cout << "Invalid choice.\n";
            return "";
        }

        return courses[choice - 1];
    }

    void addLesson(const Lesson& newLesson, const string& course) {

    Lesson lessonWithCourse = newLesson;
    lessonWithCourse.course = course;

    lessons.push_back(lessonWithCourse);
    cout << "Lesson added to course: " << course << "\n";
    saveLessonToFile(lessonWithCourse, course);

    }

            void updateLesson(const string& title) {
        // Find the lesson by title
        auto it = find_if(lessons.begin(), lessons.end(), [&title](const Lesson& lesson) {
            return lesson.title == title;
        });

        if (it == lessons.end()) {
            cout << "Lesson not found: " << title << "\n";
            return;
        }

        Lesson& lessonToUpdate = *it;

        // Update lesson details
        string newContent, newDetails;
        cout << "Enter new lesson content (leave empty to keep current): ";
        getline(cin, newContent);
        if (!newContent.empty()) {
            lessonToUpdate.content = newContent;
        }

        cout << "Enter new lesson details (leave empty to keep current): ";
        getline(cin, newDetails);
        if (!newDetails.empty()) {
            lessonToUpdate.details = newDetails;
        }

        char addMore;
        do {
            cout << "Add new file path? (y/n): ";
            cin >> addMore;
            cin.ignore();
            if (addMore == 'y') {
                string filePath;
                cout << "Enter new file path: ";
                getline(cin, filePath);
                lessonToUpdate.addFile(filePath);
            }
        } while (addMore == 'y');

        do {
            cout << "Add new link? (y/n): ";
            cin >> addMore;
            cin.ignore();
            if (addMore == 'y') {
                string link;
                cout << "Enter new link: ";
                getline(cin, link);
                lessonToUpdate.addLink(link);
            }
        } while (addMore == 'y');

        // Save updated lesson to file
        saveLessonToFile(lessonToUpdate, lessonToUpdate.course);
        cout << "Lesson updated successfully!\n";
    }

    void saveLessonToFile(const Lesson& lesson, const string& course) {
        ifstream lessonFileIn("../lesson.csv");
        if (!lessonFileIn.is_open()) {
            cout << "Error: Unable to open lesson file for reading.\n";
            return;
        }

        vector<string> lines;
        string line;
        bool found = false;

        // Read all lines and check if the lesson already exists
        while (getline(lessonFileIn, line)) {
            vector<string> lessonDetails = splitLine(line, ',');
            if (lessonDetails.size() >= 2 && lessonDetails[0] == lesson.title && lessonDetails[1] == course) {
                // Update the existing lesson
                found = true;
                stringstream updatedLine;
                updatedLine << lesson.title << "," << course << "," << lesson.content << "," << lesson.details;
                for (const auto& file : lesson.filePaths) {
                    updatedLine << "," << file;
                }
                for (const auto& link : lesson.links) {
                    updatedLine << "," << link;
                }
                lines.push_back(updatedLine.str());
            } else {
                lines.push_back(line);
            }
        }
        lessonFileIn.close();

        if (!found) {
            // Add the new lesson if it doesn't exist
            stringstream newLine;
            newLine << lesson.title << "," << course << "," << lesson.content << "," << lesson.details;
            for (const auto& file : lesson.filePaths) {
                newLine << "," << file;
            }
            for (const auto& link : lesson.links) {
                newLine << "," << link;
            }
            lines.push_back(newLine.str());
        }

        // Write all lines back to the file
        ofstream lessonFileOut("../lesson.csv");
        if (!lessonFileOut.is_open()) {
            cout << "Error: Unable to open lesson file for writing.\n";
            return;
        }

        for (const auto& l : lines) {
            lessonFileOut << l << "\n";
        }
        lessonFileOut.close();
    }


    void deleteLesson(const string& lessonTitle) {
    auto it = remove_if(lessons.begin(), lessons.end(),
                        [&lessonTitle](const Lesson& lesson) {
                            return lesson.title == lessonTitle;
                        });
    if (it != lessons.end()) {
        lessons.erase(it, lessons.end());
        cout << "Lesson deleted successfully: " << lessonTitle << "\n";
        deleteLessonFromFile(lessonTitle); 
    } else {
        cout << "Lesson not found: " << lessonTitle << "\n";
    }
}

    void deleteLessonFromFile(const string& lessonTitle) {
    ifstream lessonFile("../lesson.csv");
    if (!lessonFile.is_open()) {
        cout << "Error: Unable to open lesson file.\n";
        return;
    }

    vector<string> lines;
    string line;
    bool found = false;
    while (getline(lessonFile, line)) {
        vector<string> lessonDetails = splitLine(line, ',');
        if (!lessonDetails.empty() && lessonDetails[0] == lessonTitle) {
                found = true;
        } else {
            lines.push_back(line);
        }
    }

    lessonFile.close();

    if (!found) {
        cout << "Lesson not found: " << lessonTitle << "\n";
        return;
    }

    ofstream outFile("../lesson.csv");
    if (!outFile.is_open()) {
        cout << "Error: Unable to open lesson file for writing.\n";
        return;
    }

    for (const auto& l : lines) {
        outFile << l << "\n";
    }
    outFile.close();

}

    void displayAllLessons() const {
    ifstream courseFile("../course.csv");
    if (!courseFile.is_open()) {
        cout << "Error: Unable to open course file.\n";
        return;
    }

    map<string, string> courseDetails;
    string courseLine;
    while (getline(courseFile, courseLine)) {
        vector<string> courseData = splitLine(courseLine, ',');
        if (courseData.size() >= 2) {
            courseDetails[courseData[0]] = courseData[1];
        }
    }
    courseFile.close();

    ifstream lessonFile("../lesson.csv");
    if (!lessonFile.is_open()) {
        cout << "Error: Unable to open lesson file.\n";
        return;
    }

    string line;
    while (getline(lessonFile, line)) {
        vector<string> lessonDetails = splitLine(line, ',');
        if (lessonDetails.size() >= 4) {
            cout << "Title: " << lessonDetails[0] << "\n";
            cout << "Course: " << lessonDetails[1] << "\n";
            cout << "Content: " << lessonDetails[2] << "\n";
            cout << "Details: " << lessonDetails[3] << "\n";
            if (courseDetails.find(lessonDetails[1]) != courseDetails.end()) {
                cout << "Course Teacher: " << courseDetails[lessonDetails[1]] << "\n";
            }
            if (lessonDetails.size() > 4) {
                cout << "Files and Links:\n";
                for (size_t i = 4; i < lessonDetails.size(); ++i) {
                    cout << "- " << lessonDetails[i] << "\n";
                }
            }
            cout << "\n";
        }
    }
    lessonFile.close();
}


};

// Teacher class definition
class Teacher {
private:
    string username;
    string password;
    LessonManager lessonManager;
    vector<Lesson> lessons;
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

        string selectCourse() {
        return lessonManager.selectCourse();
    }

    void addLesson(const Lesson& newLesson, const string& course) {
        lessonManager.addLesson(newLesson, course);
    }
    

    void deleteLesson(const string& title) {
        lessonManager.deleteLesson(title);
    }

    void displayAllLessons() {
        lessonManager.displayAllLessons();
    }

        void updateLesson(const string& title) {
        lessonManager.updateLesson(title);
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

   void deleteAssignment() {
    // Get teacher's courses
    vector<string> teacherCourses;
    ifstream courseFile("../course.csv");
    if (!courseFile.is_open()) {
        cout << "Error: Unable to open course file!" << endl;
        return;
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

    // Read all assignments and filter by teacher's courses
    vector<pair<string, string>> teacherAssignments; // pairs of (course, assignment)
    ifstream assignFile("../assignments.csv");
    if (!assignFile.is_open()) {
        cout << "Error: Unable to open assignments file!" << endl;
        return;
    }

    while (getline(assignFile, line)) {
        vector<string> assignDetails = split(line, ',');
        if (assignDetails.size() >= 2) {
            // Check if assignment belongs to teacher's course
            if (find(teacherCourses.begin(), teacherCourses.end(), assignDetails[0]) != teacherCourses.end()) {
                teacherAssignments.push_back({assignDetails[0], assignDetails[1]});
            }
        }
    }
    assignFile.close();

    if (teacherAssignments.empty()) {
        cout << "No assignments found in your courses!" << endl;
        return;
    }

    // Display assignments
    cout << "\nYour Assignments:" << endl;
    cout << "----------------" << endl;
    for (size_t i = 0; i < teacherAssignments.size(); ++i) {
        cout << i + 1 << ". Course: " << teacherAssignments[i].first 
             << ", Assignment: " << teacherAssignments[i].second << endl;
    }

    // Get user choice
    int choice;
    cout << "\nSelect assignment to delete (Enter number): ";
    cin >> choice;

    if (choice < 1 || choice > static_cast<int>(teacherAssignments.size())) {
        cout << "Invalid selection!" << endl;
        return;
    }

    string selectedCourse = teacherAssignments[choice-1].first;
    string selectedAssignment = teacherAssignments[choice-1].second;

    // 1. Delete from assignments.csv
    vector<string> updatedAssignments;
    ifstream readAssignFile("../assignments.csv");
    while (getline(readAssignFile, line)) {
        vector<string> assignDetails = split(line, ',');
        if (assignDetails.size() >= 2) {
            if (!(assignDetails[0] == selectedCourse && 
                  assignDetails[1] == selectedAssignment)) {
                updatedAssignments.push_back(line);
            }
        }
    }
    readAssignFile.close();

    ofstream writeAssignFile("../assignments.csv");
    for (const string& assignment : updatedAssignments) {
        writeAssignFile << assignment << endl;
    }
    writeAssignFile.close();

    // 2. Delete from submissions.csv
    vector<string> updatedSubmissions;
    ifstream readSubmissionFile("../submissions.csv");
    while (getline(readSubmissionFile, line)) {
        vector<string> submissionDetails = split(line, ',');
        if (submissionDetails.size() >= 3) {
            if (!(submissionDetails[1] == selectedCourse && 
                  submissionDetails[2] == selectedAssignment)) {
                updatedSubmissions.push_back(line);
            }
        }
    }
    readSubmissionFile.close();

    ofstream writeSubmissionFile("../submissions.csv");
    for (const string& submission : updatedSubmissions) {
        writeSubmissionFile << submission << endl;
    }
    writeSubmissionFile.close();

    // 3. Delete from grades.csv
    vector<string> updatedGrades;
    ifstream readGradeFile("../grades.csv");
    while (getline(readGradeFile, line)) {
        vector<string> gradeDetails = split(line, ',');
        if (gradeDetails.size() >= 3) {
            if (!(gradeDetails[0] == selectedCourse && 
                  gradeDetails[1] == selectedAssignment)) {
                updatedGrades.push_back(line);
            }
        }
    }
    readGradeFile.close();

    ofstream writeGradeFile("../grades.csv");
    for (const string& grade : updatedGrades) {
        writeGradeFile << grade << endl;
    }
    writeGradeFile.close();

    cout << "Assignment and related submissions/grades deleted successfully!" << endl;
}

    void manageAssignments() {
    int choice;
    do {
        cout << "\n=== Assignment Management ===\n";
        cout << "1. Add Assignment\n";
        cout << "2. Grade Assignment\n";
        cout << "3. Delete Assignment\n";  // New option
        cout << "4. Back to Main Menu\n";
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
                    deleteAssignment();  // New case
                    break;
                case 4:
                    cout << "Returning to main menu...\n";
                    break;
                default:
                    cout << "Invalid choice!\n";
            }
        } while (choice != 4);
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
            cout << "4. Update Lesson\n";
            cout << "5. Back to Main Menu\n";
            cout << "Enter your choice: ";
            cin >> choice;
            cin.ignore();

            switch (choice) {
                case 1: {
                    string course = selectCourse();
                    if (course.empty()) {
                        cout << "No course selected. Returning to menu.\n";
                        break;
                    }

                    string title, content, details;
                    cout << "Enter lesson title: ";
                    cin.ignore();
                    getline(cin, title);

                    cout << "Enter lesson content: ";
                    getline(cin, content);

                    Lesson newLesson(title, content, course);

                    cout << "Enter lesson details: ";
                    getline(cin, details);
                    newLesson.addDetails(details);

                    
                    char addMore;
                    do {
                        cout << "Add file path? (y/n): ";
                        cin >> addMore;
                        cin.ignore();
                        if (addMore == 'y') {
                            string filePath;
                            cout << "Enter file path: ";
                            getline(cin, filePath);
                            newLesson.addFile(filePath);
                        }
                    } while (addMore == 'y');

                do {
                    cout << "Add link? (y/n): ";
                    cin >> addMore;
                    cin.ignore();
                    if (addMore == 'y') {
                        string link;
                        cout << "Enter link: ";
                        getline(cin, link);
                        newLesson.addLink(link);
                    }
                } while (addMore == 'y');

                    addLesson(newLesson, course);
                    break;

                }
                case 2: {
                    string title;
                    cout << "Enter lesson title to delete: ";
                    getline(cin, title);
                    deleteLesson(title);
                    break;
                }
                case 3:
                    displayAllLessons();
                    break;
                case 4: {
                    string title;
                    cout << "Enter lesson title to update: ";
                    getline(cin, title);
                    updateLesson(title);
                    break;
                }
                case 5:
                    cout << "Returning to main menu...\n";
                    break;
                default:
                    cout << "Invalid choice!\n";
            }
        } while (choice != 5);
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