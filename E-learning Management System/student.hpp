#ifndef STUDENT_HPP
#define STUDENT_HPP
#include <iostream>
#include <vector>
#include <map>
#include <string>

// Class Declaration and Implementation
class Student {
private:
    std::string username; // Student's username
    std::string password; // Student's password (6-digit number)
    std::map<std::string, std::vector<std::string>> courseMaterials; // Maps course names to their materials
    std::map<std::string, std::string> grades; // Maps course names to grades
    std::map<std::string, std::vector<std::string>> submittedAssignments; // Maps course names to submitted assignments
    std::map<std::string, int> progress; // Track course progress in percentage

public:
    static std::map<std::string, Student> students;

    // Constructor
    Student() = default;
    Student(const std::string& username, const std::string& password)
        : username(username), password(password) {}

    // Static function to load students from a CSV file
    static void loadStudentsFromCSV(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Error: Unable to open file " << filename << std::endl;
            return;
        }
        std::string line, username, name;
        while (getline(file, line)) {
            std::stringstream ss(line);
            getline(ss, username, ','); // Read username
            getline(ss, name, ',');     // Read name
            students[username] = Student(username, username); // Default password = username
        }
        file.close();
        std::cout << "Students loaded from " << filename << std::endl;
    }

//Funtions
    // Login function
    bool login(const string& username, const string& password) {
        ifstream file("../student.csv"); // Open the file
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

    // Change password
    void changePassword(const std::string& newPassword){
        password = newPassword;
        std::cout << "Password successfully changed!\n";
    }

    // View grades
    void viewGrades() {
        if (grades.empty()) {
            std::cout << "No grades available.\n";
        } else {
            for (const auto& course : grades) {
                std::cout << "Course: " << course.first << " - Grade: " << course.second << "\n";
            }
        }
    }

    // View Assignments
    void viewAssignments(const std::map<std::string, std::vector<std::string>>& assignments) {
        for (const auto& course : assignments) {
            std::cout << "Course: " << course.first << "\nAssignments:\n";
            for (const auto& assignment : course.second) {
                std::cout << "  - " << assignment << "\n";
            }
        }
    }

    // View Course Materials
    void viewCourseMaterials() {
        for (const auto& course : courseMaterials) {
            std::cout << "Course: " << course.first << "\nMaterials:\n";
            for (const auto& material : course.second) {
                std::cout << "  - " << material << "\n";
            }
        }
    }

    // View a Specific Course
    void viewCourse(const std::string& courseName) {
        if (courseMaterials.find(courseName) != courseMaterials.end()) {
            std::cout << "Course: " << courseName << "\nMaterials:\n";
            for (const auto& material : courseMaterials[courseName]) {
                std::cout << "  - " << material << "\n";
            }
        } else {
            std::cout << "Course not found.\n";
        }
    }

    // Submit Assignment
    void submitAssignment(const std::string& courseName, const std::string& assignmentName) {
        submittedAssignments[courseName].push_back(assignmentName);
        std::cout << "Assignment \"" << assignmentName << "\" submitted for course \"" << courseName << "\".\n";
    }

    // View Progress
    void viewProgress() {
        if (progress.empty()) {
            std::cout << "No progress data available.\n";
        } else {
            for (const auto& course : progress) {
                std::cout << "Course: " << course.first << " - Progress: " << course.second << "%\n";
            }
        }
    }

    // Display User Information
    void displayInfo() {
        std::cout << "Username: " << username << "\n";
        std::cout << "Password: " << password << "\n";
        std::cout << "Enrolled Courses: ";
        for (const auto& course : courseMaterials) {
            std::cout << course.first << " ";
        }
        std::cout << "\n";
    }
};

// Initialize static member
std::map<std::string, Student> Student::students;

#endif // STUDENT_HPP
