// Stage 1: Load CSV, Create Schema, Save with # Delimiter
// File: io.cpp (new code for Stage 1)

#include "io.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <filesystem>
#include<vector>
#include<string>
using namespace std;

namespace fs = filesystem;

// Helper to detect data type (very basic)

string detectType(const string& value) {
    if (value.empty()) return "string";
    bool hasDot = false;
    for (char c : value) {
        if (isdigit(c)) continue;
        if (c == '.' && !hasDot) {
            hasDot = true;
            continue;
        }
        return "string";  // Contains letters or symbols
    }
    return hasDot ? "float" : "int";
}
void loadCSVandSave(const string& filepath, const string& outDir, const string& schemaFilePath) {
    ifstream fin(filepath);
    if (!fin) {
        cerr << "Could not open file: " << filepath << endl;
        return;
    }

    fs::create_directories(outDir);
    auto schemaDir = fs::path(schemaFilePath).parent_path();
    if (!schemaDir.empty()) {
        fs::create_directories(schemaDir);
    }


    string filename = fs::path(filepath).stem().string();  // e.g., titanic.csv → titanic
    string outPath = outDir + "/" + filename + ".txt";     // titanic.txt
    ofstream fout(outPath, ios::trunc);                    // Clear old data
    ofstream fschema(schemaFilePath, ios::app);            // Append mode

    string line;
    getline(fin, line); // Read header
    vector<string> headers;
    stringstream ss(line), st;
    string item;

    while (getline(ss, item, ',')) {
        headers.push_back(item);
    }

    // Read second line to guess types
    string secondLine;
    getline(fin, secondLine);
    st.str(secondLine);
    vector<string> values;

    while (getline(st, item, ',')) {
        values.push_back(item);
    }

    // Generate schema line: titanic#id#int#name#string...
    fschema << filename;
    for (size_t i = 0; i < headers.size(); ++i) {
        string value = i < values.size() ? values[i] : "";
        string type = detectType(value);
        fschema << "#" << headers[i] << "#" << type;
    }
    fschema << "\n";

    // Write second and rest of CSV lines to titanic.txt
    auto writeLine = [&](string& l) {
        for (char& c : l) if (c == ',') c = '#';
        fout << l << "\n";
    };

    writeLine(secondLine);
    while (getline(fin, line)) {
        writeLine(line);
    }

    cout << "Processed: " << filepath << "\n→ Data: " << outPath << "\n→ Schema: " << schemaFilePath << "\n";
}

std::vector<std::string> split_csv_line(const std::string& line) {
    std::vector<std::string> result;
    std::stringstream ss(line);
    std::string item;

    while (std::getline(ss, item, ',')) {
        result.push_back(item);
    }

    return result;
}
