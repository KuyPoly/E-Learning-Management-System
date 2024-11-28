#ifndef STUDENT_HPP
#define STUDENT_HPP
#include <iostream>
#include <vector>
#include <map>
#include <string>

//class Declaration
    class Student{
    private:
        std::string username;
        std::string password;
        std::map<std::string, std::vector<std::string>> courseMaterials;//Course -> materials
        std::map<std::string, std::string> grades;// course -> grade
        std::map<std::string, std::vector<std::string>> submittedAssignments;//Course -> assignments

    public:
        //Constructor
        Student(std::string username,std::string Password);

        //Funtion
        bool login(const std::string&enteredUsername, const std::string& enteredPassword);
        void changePassword(const std::string& newPassword);
        void viewGrades();

        //viewAssignment
        void viewAssignments(const std::map<std::string, std::vector<std::string>>& assignments){
            for(const auto& course : assignments){
                std::cout<<"Course:" <<course.first<<"\nAssignments:\n";
                for(const auto& assignment : course.second){
                    std::cout<<" - " << assignment << "\n";
                }
            }
        };

        //view Course Materials
        void viewCourseMaterials(const std::map<std::string, std::vector<std::string>>& courseMaterials){
            for (const auto& course : courseMaterials) {
                std::cout << "Course: " << course.first << "\nMaterials:\n";
                for (const auto& material : course.second) {
                    std::cout << "  - " << material << "\n";
                }
            }   
        };

        //View a Specific Course
        void viewCourse(const std::string& courseName){
            if (courseMaterials.find(courseName) != courseMaterials.end()) {
                std::cout << "Course: " << courseName << "\nMaterials:\n";
                for (const auto& material : courseMaterials[courseName]) {
                    std::cout << "  - " << material << "\n";
                }
            } else {
                std::cout << "Course not found.\n";
            }    
        };

        //Submit Assignment
        void submitAssignment(const std::string& courseName, const std::string& assignmentName){
            submittedAssignments[courseName].push_back(assignmentName);
            std::cout << "Assignment \"" << assignmentName << "\" submitted for course \"" << courseName << "\".\n";
        };

        //viewProgress
        void viewProgress(const std::map<std::string, int>& progress){
            for (const auto& course : progress) {
                std::cout << "Course: " << course.first << " - Progress: " << course.second << "%\n";
            }  
        };
        
        // Display User Information
        void displayInfo(){
            std::cout << "Username: " << username << "\n";
            std::cout << "Password: " << password << "\n";
            std::cout << "Enrolled Courses: ";
            for (const auto& course : courseMaterials) {
                std::cout << course.first << " ";
            }
            std::cout << "\n";
        }
    };

#endif