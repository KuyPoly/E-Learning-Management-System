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
        void enrollIncourse(const std::string& courseName);
        void dropCourse(const std::string& courseName);
        void viewAssignments(const std::map<std::string, std::vector<std::string>>& assignments);
        void viewCourseMaterials(const std::map<std::string, std::vector<std::string>>& courseMaterials);
        void viewGrades(const std::map<std::string, std::string>&grades);
        void submitAssignment(const std::string& courseName, const std::string& assignmentName);
        void viewProgress(const std::map<std::string, int>& progress);
        void displayInfo();
        void updateInfor(const std::string& newUsername, const std::string& newPassword);
    };
#endif