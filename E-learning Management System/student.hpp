#ifndef STUDENT_HPP
#define STUDENT_HPP
#include <iostream>
#include <string>
#include <set>
#include <cstdlib> 
#include <utility>
#include "../utils/file.h"
#include "../utils/validate.h"

class Student {
private:
    string username;
    string password;

        // Add these helper methods to the Student class private section
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
    Student(const string& username, const string& password) : username(username), password(password) {}

    void viewEnrolledCourses() const {
        ifstream file("../course.csv");
        if (!file.is_open()) {
            cout << "Error: Could not open file course.csv" << endl;
            return;
        }

        string line;
        bool foundCourse = false; // To track if any courses are found
        cout << "--------------------------------------------" << endl;
        cout << "Your Enrolled Courses:" << endl;
        cout << "--------------------------------------------" << endl;
        cout << "Course ID | Instructor Name" << endl;
        cout << "--------------------------------------------" << endl<<endl;
        system("pause");
        system("cls");
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

            cout<<"---------- change your password ----------\n"<<endl;
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
                        
                        // Allowing passwords 6 or more characters long
                        if (new_password.length() < 6) {
                            cout << "New password must be than 6 characters!" << endl<<endl;  
                            lines.push_back(line);
                            continue;

                        }

                        if (new_password != confirm_password) {
                            cout << "Passwords do not match!" << endl<<endl;
                            lines.push_back(line);
                            continue;
                        }

                        lines.push_back(data[0] + "," + new_password);
                        cout << "Password updated successfully!" << endl;
                        system("pause");
                        system("cls");
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
                system("cls");
                if (choice == 2) {
                    system("cls");
                    exit_loop = true;
                }
            }
        } while (!exit_loop);
    }

   void viewLessons() {
        ifstream lessonFile("../lesson.csv");
        if (!lessonFile.is_open()) {
            cout << "Error: Unable to open lesson file.\n";
            return;
        }

        vector<vector<string>> lessons;
        string line;

        // Read all lessons from the file
        while (getline(lessonFile, line)) {
            cout << "Lesson: " << line << "\n";
            vector<string> data = split(line, ',');
            if (data.size() >= 3) {
                lessons.push_back(data);
            } //else {
            //cout << "Invalid lesson format: " << line << "\n";//
       // } //
    }

        lessonFile.close();

        // Check if lessons are available
        if (lessons.empty()) {
            cout << "No lessons found in the file.\n";
            return;
        }

        // Display available lessons
        cout << "Available Lessons:\n";
        cout << "-------------------------------------------------------------\n";
        cout << "No | Lesson Title         | Course           | Details\n";
        cout << "-------------------------------------------------------------\n";

        for (size_t i = 0; i < lessons.size(); ++i) {
            cout << i + 1 << "  | " << lessons[i][0] << "  | " << lessons[i][1] << "  | " << lessons[i][2] << "\n";
        }

        cout << "-------------------------------------------------------------\n";

        int choice;
        cout << "Enter the number of the lesson to view more details (or 0 to exit): ";
        cin >> choice;
        system("cls");
        
        if (choice == 0) {
            cout << "Exiting lesson view.\n";
            system("cls");
            return;
        } else if (choice < 1 || choice > lessons.size()) {
            cout << "Invalid choice.\n";
            system("pause");
            system("cls");
            return;
        }

        // Display detailed lesson content
        cout << "\nLesson Details:\n";
        cout << "-------------------------------------------------------------\n";
        cout << "Lesson Title: " << lessons[choice - 1][0] << "\n";
        cout << "Course      : " << lessons[choice - 1][1] << "\n";
        cout << "Details     : " << lessons[choice - 1][2] << "\n";
        cout << "-------------------------------------------------------------\n";
        system("pause");
        system("cls");
    }

void viewAssignments(const string& username) const {
        ifstream assignmentsFile("../assignments.csv");
        ifstream submissionsFile("../submissions.csv");

        if (!assignmentsFile.is_open()) {
            cout << "Could not open file assignments.csv" << endl;
            return;
        }

        if (!submissionsFile.is_open()) {
            cout << "Could not open file submissions.csv" << endl;
            assignmentsFile.close();
            return;
        }

        // Read submitted assignments for the given user into a set for quick lookup
        set<pair<string, string>> submittedAssignments;
        string line;
        while (getline(submissionsFile, line)) {
            vector<string> submissionData = split(line, ',');
            if (submissionData.size() < 3) {
                continue; // Skip invalid entries
            }

            string studentName = submissionData[0];
            string courseId = submissionData[1];
            string assignmentName = submissionData[2];

            if (studentName == username) {
                submittedAssignments.insert({courseId, assignmentName});
            }
        }

        submissionsFile.close();

        // Display assignments that are not submitted by the given user
        cout << "Assignments List for " << username << ":" << endl;
        cout << "--------------------------------------------" << endl;
        cout << "No. | Course ID | Assignment | Resource" << endl;
        cout << "--------------------------------------------" << endl;

        vector<vector<string>> availableAssignments;
        int counter = 1;

        while (getline(assignmentsFile, line)) {
            vector<string> assignmentData = split(line, ',');
            if (assignmentData.size() < 2) {
                continue; // Skip invalid entries
            }

            string courseId = assignmentData[0];
            string assignmentName = assignmentData[1];
            string resource = assignmentData.size() > 2 ? assignmentData[2] : "None";

            if (submittedAssignments.find({courseId, assignmentName}) == submittedAssignments.end()) {
                cout << counter << ". | " << courseId << " | " << assignmentName;
                if (resource != "None") {
                    cout << " | [Available]";
                } else {
                    cout << " | [None]";
                }
                cout << endl;
                availableAssignments.push_back(assignmentData);
                counter++;
            }
        }

        assignmentsFile.close();

        if (availableAssignments.empty()) {
            cout << "No pending assignments." << endl;
            return;
        }

        cout << "--------------------------------------------" << endl;
        cout << "Would you like to view an assignment resource? (y/n): ";
        char choice;
        cin >> choice;

        if (choice == 'y' || choice == 'Y') {
            cout << "Enter assignment number to view resource: ";
            int assignmentChoice;
            cin >> assignmentChoice;

            if (assignmentChoice < 1 || assignmentChoice > static_cast<int>(availableAssignments.size())) {
                cout << "Invalid assignment number!" << endl;
                system("pause");
                system("cls");
                return;
            }

            vector<string>& selectedAssignment = availableAssignments[assignmentChoice - 1];
            if (selectedAssignment.size() <= 2 || selectedAssignment[2] == "None") {
                cout << "No resource available for this assignment." << endl;
                return;
            }

            string resource = selectedAssignment[2];
            bool isLink = resource.find("http://") == 0 || resource.find("https://") == 0;
            cout << "\nOpening " << (isLink ? "link" : "file") << ": " << resource << endl;
            

            // if (!openFile(resource)) {
            //     cout << "Error: Could not open the " << (isLink ? "link" : "file") << "." << endl;
            //     cout << "Please check if the " << (isLink ? "link is valid" : "file exists and you have permission to access it") << "." << endl;
            // }
            system("pause");
            system("cls");
        }
    }

    void submitAssignment(const string& username) const {
        ifstream assignmentsFile("../assignments.csv");
        ifstream submissionsFile("../submissions.csv");

        if (!assignmentsFile.is_open()) {
            cout << "Error: Could not open file assignments.csv" << endl;
            return;
        }

        if (!submissionsFile.is_open()) {
            cout << "Error: Could not open file submissions.csv" << endl;
            assignmentsFile.close();
            return;
        }

        // Read submitted assignments for the given user into a set for quick lookup
        set<pair<string, string>> submittedAssignments;
        string line;
        while (getline(submissionsFile, line)) {
            vector<string> submissionData = split(line, ',');
            if (submissionData.size() < 3) {
                continue; // Skip invalid entries
            }

            string studentName = submissionData[0];
            string courseId = submissionData[1];
            string assignmentName = submissionData[2];

            if (studentName == username) {
                submittedAssignments.insert({courseId, assignmentName});
            }
        }

        submissionsFile.close();

        // Store assignments that are not yet submitted
        vector<pair<string, string>> availableAssignments;
        while (getline(assignmentsFile, line)) {
            vector<string> assignmentData = split(line, ',');
            if (assignmentData.size() < 2) {
                continue; // Skip invalid entries
            }

            string courseId = assignmentData[0];
            string assignmentName = assignmentData[1];

            if (submittedAssignments.find({courseId, assignmentName}) == submittedAssignments.end()) {
                availableAssignments.push_back({courseId, assignmentName});
            }
        }

        assignmentsFile.close();

        if (availableAssignments.empty()) {
            cout << "No available assignments to submit." << endl;
            return;
        }

        // Display available assignments
        cout << "Available Assignments:" << endl;
        cout << "---------------------------------------------" << endl;
        cout << "No | Course ID | Assignment Name" << endl;
        cout << "---------------------------------------------" << endl;

        for (size_t i = 0; i < availableAssignments.size(); ++i) {
            cout << i + 1 << " | " << availableAssignments[i].first << " | " << availableAssignments[i].second << endl;
        }

        int choice;
        cout << "Enter the number of the assignment to submit: ";
        cin >> choice;
        if (choice < 1 || choice > availableAssignments.size()) {
            cout << "Invalid choice!" << endl;
            system("pause");
            system("cls");
            return;
        }

        string selectedCourse = availableAssignments[choice - 1].first;
        string selectedAssignment = availableAssignments[choice - 1].second;

        string submission;
        while (true) {
            cout << "Submitting for assignment: " << selectedAssignment << endl;
            cout << "Enter a file path or a link to submit (or type 'exit' to quit): ";

            cin.clear();
            cin.sync();
            getline(cin, submission);

            // Trim whitespace
            submission.erase(0, submission.find_first_not_of(" \t\n\r"));
            submission.erase(submission.find_last_not_of(" \t\n\r") + 1);

            if (submission == "exit") {
                cout << "Exiting the process." << endl;
                system("pause");
                system("cls");
                return;
            }

            // Validate submission as a file path or link
            if (submission.empty()) {
                cout << "Submission cannot be empty." << endl;
                cout << "---------------------||------------------------" << endl;
                cout << "Please try again!" << endl;
            } else if (submission.find("http://") == 0 || submission.find("https://") == 0) {
                break; // Valid link
            } else if (submission.find(":\\") != string::npos || submission.find("/") != string::npos) {
                break; // Valid file path
            } else {
                cout << "Invalid! Please try again" << endl;
                cout << "---------------------||------------------------" << endl;
                system("pause");
                system("cls");
            }
        }

        ofstream submissionsFileOut("../submissions.csv", ios::app);

        if (!submissionsFileOut.is_open()) {
            cout << "Error: Could not open file submissions.csv" << endl;
            return;
        }

        // Check if the submission is a link or file
        bool isLink = submission.find("http://") == 0 || submission.find("https://") == 0;

        if (isLink) {
            submissionsFileOut << username << "," << selectedCourse << "," << selectedAssignment << "," << submission << endl;
            cout << "Link submitted successfully!" << endl;
            system("pause");
            system("cls");
        } else {
            submissionsFileOut << username << "," << selectedCourse << "," << selectedAssignment << "," << submission << endl;
            cout << "File submitted successfully!" << endl;
            system("pause");
            system("cls");
        }

        submissionsFileOut.close();
    }
    void deleteSubmission() {
        ifstream submissionsFile("../submissions.csv"); // Open submissions file

        if (!submissionsFile.is_open()) {
            cout << "Error: Could not open file submissions.csv" << endl;
            return;
        }

        vector<vector<string>> submissions;
        string line;

        // Read all submissions into a vector
        while (getline(submissionsFile, line)) {
            vector<string> data = split(line, ',');
            if (data.size() >= 4) { // Ensure the line has all necessary fields
                submissions.push_back(data);
            }
        }

        submissionsFile.close();

        // Check if there are submissions to delete
        if (submissions.empty()) {
            cout << "No submissions available to delete!" << endl;
            return;
        }

        cout << "Your Submissions:" << endl;
        cout << "---------------------------------------------------------------" << endl;
        cout << "No | Course ID | Assignment Name | Submission" << endl;
        cout << "---------------------------------------------------------------" << endl;

        for (size_t i = 0; i < submissions.size(); ++i) {
            if (submissions[i][0] == username) { // Only show user's submissions
                cout << i + 1 << " | " << submissions[i][1] << " | " << submissions[i][2] << " | " << submissions[i][3] << endl;
            }
        }

        int choice;
        cout << "Enter the number of the submission to delete: ";
        cin >> choice;

        if (choice < 1 || choice > submissions.size()) {
            cout << "Invalid choice!" << endl;
            system("pause");
            system("cls");
            return;
        }

        // Remove the selected submission
        submissions.erase(submissions.begin() + (choice - 1));

        // Write the updated list back to the file
        ofstream submissionsOutFile("../submissions.csv");

        if (!submissionsOutFile.is_open()) {
            cout << "Error: Could not open file submissions.csv for writing!" << endl;
            return;
        }

        for (const auto& submission : submissions) {
            submissionsOutFile << submission[0] << "," << submission[1] << "," << submission[2] << "," << submission[3] << endl;
        }

        submissionsOutFile.close();
        cout << "Submission deleted successfully!" << endl;
        system("pause");
        system("cls");
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
        cout << "------------------------------------------------------" << endl<<endl;
        system("pause");
        system("cls");
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
            cout << "---------------------------------------------------------------" << endl<<endl;
            system("pause");
            system("cls");
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
        system("pause");
        system("cls");
    }

    /*MENU*/
    void manageAssignments() {
    int choice;
    do {
        cout << "\n=== Assignment Management ===\n";
        cout << "1. View Lessons\n";
        cout << "2. View Assignments\n";
        cout << "3. Submit Assignment\n";
        cout << "4. Delete Submissions\n";
        cout << "5. View Grades\n";
        cout << "6. Back to Main Menu\n";
        cout << "==================\n";
        cout << "Enter your choice: ";
        cin >> choice;
        system("cls");

        switch (choice) {
            case 1:
                viewLessons();
                break;
            case 2:
                viewAssignments(username);
                break;
            case 3:
                submitAssignment(username);
                break;
            case 4:
                deleteSubmission();
                break;
            case 5:
                viewGrade();
                break;
            case 6:
                cout << "Returning to main menu...\n";
                break;
            default:
                cout << "Invalid choice!\n";
        }
    } while (choice != 6);
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
        system("cls");
        
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
        system("cls");

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