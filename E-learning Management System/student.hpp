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

public:
    // Constructor
    Student(const std::string& username, const std::string& password);

    bool login(const std::string& enteredUsername, const std::string& enteredPassword);
    void changePassword(const std::string& newPassword);
    // View Grades
    void viewGrades() {
        std::cout << "Grades:\n";
        for (const auto& course : grades) {
            std::cout << "Course: " << course.first << " - Grade: " << course.second << "\n";
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
    void viewCourseMaterials(const std::map<std::string, std::vector<std::string>>& courseMaterials) {
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
    void viewProgress(const std::map<std::string, int>& progress) {
        for (const auto& course : progress) {
            std::cout << "Course: " << course.first << " - Progress: " << course.second << "%\n";
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

#endif // STUDENT_HPP
