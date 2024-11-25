#ifndef CHECKFILE_H
#define CHECKFILE_H

#include <fstream>
#include <iostream>
#include <cstdlib>
using namespace std;

// Function to check if a file is open
void checkfile(ifstream& file) {
    if (!file.is_open()) {
        cerr << "Error: Could not open file." << endl;
        exit(EXIT_FAILURE);
    }
}

#endif // CHECKFILE_H
