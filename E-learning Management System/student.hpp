#ifndef STUDENT_HPP
#define STUDENT_HPP
#include <iostream>
#include <string>
#include "../utils/file.h"
#include "../utils/validate.h"

class Student {
private:
    string username;
    string password;

public:
    Student(const string& username, const string& password) : username(username), password(password) {}

    void viewEnrolledCourses() const {
        ifstream file("../course.csv");
        if (!file.is_open()) {
            cout << "Error: Could not open file course.csv" << endl;
            return;
        }

        string line;
        bool foundCourse = false; // To track if any courses are found

        cout << "Your Enrolled Courses:" << endl;

        cout << "--------------------------------------------" << endl;
        cout << "Course ID | Instructor Name" << endl;
        cout << "--------------------------------------------" << endl;

        while (getline(file, line)) {
            vector<string> data = split(line, ',');
            for (size_t i = 2; i < data.size(); i++) { // Check names starting from data[2]
                if (data[i] == username) {
                    // Print course details only if the username matches
                    cout << data[0] << "  |  " << data[1] << endl;
                    foundCourse = true;
                    break; // No need to check further names in this row
                }
            }
        }

        if (!foundCourse) {
            cout << "You are not enrolled in any courses." << endl;
        }

        cout << "--------------------------------------------" << endl;
        file.close();
    }


    void changePassword() {
        string old_password, new_password, confirm_password;
        bool exit_loop = false;

        do {
            cout << "Enter your old password: ";
            cin >> old_password;

            ifstream file("../student.csv");
            if (!file.is_open()) {
                cout << "Error: Could not open file student.csv" << endl;
                return;
            }

            vector<string> lines;
            string line;
            bool password_updated = false;

            while (getline(file, line)) {
                vector<string> data = split(line, ',');
                if (data[0] == username) {
                    if (data[1] == old_password) {
                        cout << "Enter new password: ";
                        cin >> new_password;
                        cout << "Confirm new password: ";
                        cin >> confirm_password;

                        if (new_password != confirm_password) {
                            cout << "Passwords do not match!" << endl;
                            lines.push_back(line);
                            continue;
                        }

                        lines.push_back(data[0] + "," + new_password);
                        cout << "Password updated successfully!" << endl;
                        password_updated = true;
                    } else {
                        cout << "Old password is incorrect!" << endl;
                        lines.push_back(line);
                    }
                } else {
                    lines.push_back(line);
                }
            }

            file.close();

            if (password_updated) {
                ofstream outFile("../student.csv");
                if (!outFile.is_open()) {
                    cerr << "Error: Unable to write to file!" << endl;
                    return;
                }

                for (const string& updatedLine : lines) {
                    outFile << updatedLine << endl;
                }

                outFile.close();
                exit_loop = true;
            } else {
                cout << "Press 1 to try again or 2 to exit: ";
                int choice;
                cin >> choice;
                if (choice == 2) {
                    exit_loop = true;
                }
            }
        } while (!exit_loop);
    }

    void viewAssignments() const {
        ifstream file("../assignments.csv");

        if (!file.is_open()) {
            cout << "Could not open file assignments.csv" << endl;
            return;
        }

        string line;
        cout << "Assignments List:" << endl;
        cout << "--------------------------------------------" << endl;
        cout << "Course ID | Assignment" << endl;
        cout << "--------------------------------------------" << endl;

        while (getline(file, line)) {
            vector<string> data = split(line, ',');
            if (data.size() < 2) {
                continue;
            }

            string course_id = data[0];
            string assignment_name = data[1];

            cout << course_id << " | " << assignment_name << endl;
            cout << "--------------------------------------------" << endl;
        }

        file.close();
    }

    void submitAssignment() const {
        ifstream assignmentsFile("../assignments.csv");

        if (!assignmentsFile.is_open()) {
            cout << "Error: Could not open file assignments.csv" << endl;
            return;
        }

        string line;
        vector<pair<string, string>> coursesAssignments;

        while (getline(assignmentsFile, line)) {
            vector<string> data = split(line, ',');
            if (data.size() >= 2) {
                string course_id = data[0];
                string assignment_name = data[1];
                coursesAssignments.push_back({course_id, assignment_name});
            }
        }

        assignmentsFile.close();

        cout << "Available Assignments:" << endl;
        cout << "---------------------------------------------" << endl;
        cout << "No | Course ID | Assignment Name" << endl;
        cout << "---------------------------------------------" << endl;

        for (size_t i = 0; i < coursesAssignments.size(); ++i) {
            cout << i + 1 << " | " << coursesAssignments[i].first << " | " << coursesAssignments[i].second << endl;
        }

        int choice;
        cout << "Enter the number of the assignment to submit: ";
        cin >> choice;
        if (choice < 1 || choice > coursesAssignments.size()) {
            cout << "Invalid choice!" << endl;
            return;
        }

        string selected_course = coursesAssignments[choice - 1].first;
        string selected_assignment = coursesAssignments[choice - 1].second;

        string submission_text;
        cout << "Enter your submission for " << selected_assignment << ": " << endl;
        cin.ignore();
        getline(cin, submission_text);

        ofstream submissionsFile("../submissions.csv", ios::app);

        if (!submissionsFile.is_open()) {
            cout << "Error: Could not open file submissions.csv" << endl;
            return;
        }

        submissionsFile << username << "," << selected_course << "," << selected_assignment << "," << submission_text << endl;
        cout << "Assignment submitted successfully!" << endl;

        submissionsFile.close();
    }
        void viewGrade() const {
        ifstream gradesFile("../grades.csv");
        if (!gradesFile.is_open()) {
            cout << "Error: Could not open grades.csv file!" << endl;
            return;
        }

        string line;
        vector<vector<string>> userGrades;

        while (getline(gradesFile, line)) {
            vector<string> data = split(line, ',');
            if (data.size() >= 4 && data[2] == username) {
                userGrades.push_back(data);
            }
        }
        gradesFile.close();

        if (userGrades.empty()) {
            cout << "No grades found for username: " << username << endl;
            return;
        }

        cout << "Grades for " << username << ":" << endl;
        cout << "------------------------------------------------------" << endl;
        cout << "Course       | Assignment      | Name    | Grade" << endl;
        cout << "------------------------------------------------------" << endl;
        // Loop through all the grades for the student
        for(const auto& grade : userGrades){
            cout<<grade[0] <<" | " <<grade[1] <<" | ";

            // Split grades and display them
            for (size_t i = 3; i < grade.size(); ++i) {
            cout << grade[i];
            if (i != grade.size() - 1) {
                cout << ", ";  // Separate grades by commas
            }
            }
        cout << endl;
        }
        cout << "------------------------------------------------------" << endl;
    }
        void viewProgress() const {
        ifstream gradesFile("../grades.csv");
        if (!gradesFile.is_open()) {
            cout << "Error: Could not open grades.csv file!" << endl;
            return;
        }

        string line;
        vector<string> subjects;
        vector<vector<int>> gradesList;  // Stores grades for each subject

        // Read grades from grades.csv
        while (getline(gradesFile, line)) {
            vector<string> data = split(line, ',');
            if (data.size() >= 4 && data[2] == username) {  // Check if the current student
                string subject = data[0];  // Get the course name (subject)
                bool subjectFound = false;

                // Check if the subject already exists in subjects
                for (size_t i = 0; i < subjects.size(); ++i) {
                    if (subjects[i] == subject) {
                        subjectFound = true;
                        gradesList[i].push_back(stoi(data[3]));  // Add grade for the subject
                        break;
                    }
                }

                // If subject is not found, add a new subject and grades
                if (!subjectFound) {
                    subjects.push_back(subject);
                    vector<int> newGrades = { stoi(data[3]) };
                    gradesList.push_back(newGrades);
                }
            }
        }
        gradesFile.close();

        if (subjects.empty()) {
            cout << "No grades found for username: " << username << endl;
            return;
        }

        // Write the progress to progress.csv
        ofstream progressFile("../progress.csv", ios::app);  // Open in append mode to add new records
        if (!progressFile.is_open()) {
            cout << "Error: Could not open progress.csv file!" << endl;
            return;
        }

        cout << "Progress for " << username << ":" << endl;
        cout << "---------------------------------------------------------------" << endl;
        cout << "Subject  | Scores  | Average" << endl;
        cout << "---------------------------------------------------------------" << endl;

        // Loop through each subject and its corresponding grades
        for (size_t i = 0; i < subjects.size(); ++i) {
            string subject = subjects[i];
            vector<int> grades = gradesList[i];

            // Calculate the average grade
            double average = 0;
            if (!grades.empty()) {
                double sum = 0;
                for (int grade : grades) {
                    sum += grade;
                }
                average = sum / grades.size();  // Compute the average
            }

            // Write the progress to progress.csv
            progressFile << subject << "," << username << ",";
            for (size_t j = 0; j < grades.size(); ++j) {
                progressFile << grades[j];
                if (j != grades.size() - 1) {
                    progressFile << ",";  // Separate grades with a comma
                }
            }
            progressFile << "," << average << endl;  // Write the average at the end of the record

            // Display the progress (subject, scores, and average)
            cout << subject << " | ";
            for (size_t j = 0; j < grades.size(); ++j) {
                cout << grades[j];
                if (j != grades.size() - 1) {
                    cout << ", ";  // Separate grades with a comma
                }
            }
            cout << " | " << average << endl;
            cout << "---------------------------------------------------------------" << endl;
        }

        progressFile.close();
    }
        void displayInformation() const {
        ifstream studentsFile("../student.csv");
        if (!studentsFile.is_open()) {
            cout << "Error: Could not open students.csv file!" << endl;
            return;
        }

        string line;
        string password;
        bool found = false;

        while (getline(studentsFile, line)) {
            vector<string> data = split(line, ',');
            if (data.size() >= 2 && data[0] == username) {
                password = data[1];
                found = true;
                break;
            }
        }

        studentsFile.close();

        if (!found) {
            cout << "No information found for username: " << username << endl;
            return;
        }

        ifstream coursesFile("../course.csv");
        if (!coursesFile.is_open()) {
            cout << "Error: Could not open course.csv file!" << endl;
            return;
        }

        vector<string> enrolledCourses;
        while (getline(coursesFile, line)) {
            vector<string> data = split(line, ',');
            if (data.size() > 2 && data[2] == username) {
                enrolledCourses.push_back(data[0]);
            }
        }

        coursesFile.close();

        cout << "Student Information for " << username << ":" << endl;
        cout << "---------------------------------------------------" << endl;
        cout << "Username: " << username << endl;
        cout << "Password: " << password << endl;
        cout << "Enrolled Courses: ";
        if (enrolledCourses.empty()) {
            cout << "None" << endl;
        } else {
            for (const auto& course : enrolledCourses) {
                cout << course << " ";
            }
            cout << endl;
        }
        cout << "---------------------------------------------------" << endl;
    }

    /*MENU*/
    void manageAssignments() {
    int choice;
    do {
        cout << "\n=== Assignment Management ===\n";
        cout << "1. View Assignments\n";
        cout << "2. Submit Assignment\n";
        cout << "3. View Grades\n";
        cout << "4. Back to Main Menu\n";
        cout << "==================\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                viewAssignments();
                break;
            case 2:
                submitAssignment();
                break;
            case 3:
                viewGrade();
                break;
            case 4:
                cout << "Returning to main menu...\n";
                break;
            default:
                cout << "Invalid choice!\n";
        }
    } while (choice != 4);
}

void manageProfile() {
    int choice;
    do {
        cout << "\n=== Profile Management ===\n";
        cout << "1. View Personal Information\n";
        cout << "2. Change Password\n";
        cout << "3. Back to Main Menu\n";
        cout << "==================\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                displayInformation();
                break;
            case 2:
                changePassword();
                break;
            case 3:
                cout << "Returning to main menu...\n";
                break;
            default:
                cout << "Invalid choice!\n";
        }
    } while (choice != 3);
}

void viewAcademicProgress() {
    int choice;
    do {
        cout << "\n=== Progress Tracking ===\n";
        cout << "1. View Enrolled Courses\n";
        cout << "2. View Academic Progress\n";
        cout << "3. Back to Main Menu\n";
        cout << "==================\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                viewEnrolledCourses();
                break;
            case 2:
                viewProgress();
                break;
            case 3:
                cout << "Returning to main menu...\n";
                break;
            default:
                cout << "Invalid choice!\n";
        }
    } while (choice != 3);
}


};

#endif