#ifndef TEACHER
#define TEACHER
#include "../utils/file.h"
#include "../utils/validate.h"


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


#endif