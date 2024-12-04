#ifndef FILE
#define FILE
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

// Helper function to split a CSV line
vector<string> split(const string &line, char delimiter) {
    vector<string> tokens;
    stringstream ss(line);
    string token;
    while (getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

// Function to append a row to a CSV file
void appendToFile(const string &filename, const string &row) {
    ofstream file(filename, ios::app);
    if (file.is_open()) {
        file << row << endl;
        file.close();
    } else {
        cout << "Error: Could not open file " << filename << endl;
    }
}

#endif