// select.cpp
#include "select.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <sstream>
#include <vector>
#include "schema.h"  // so we can load the schema
#include "io.h"      // for reading/writing data
#include <stdexcept>
namespace fs = std::filesystem;

void select_relation(const std::string& relation_name) {
    std::string schema_path = "schema/schema.txt";
    std::string data_path = "output/" + relation_name + ".txt";

    // 3.1: Buscar en schema.txt
    std::ifstream schema_file(schema_path);
    if (!schema_file) {
        std::cerr << "[ERROR] Schema file not found.\n";
        return;
    }

    bool found = false;
    std::string line;
    while (std::getline(schema_file, line)) {
        if (line.substr(0, relation_name.size()) == relation_name &&
            line[relation_name.size()] == '#') {
            found = true;
            break;
        }
    }

    if (!found) {
        std::cerr << "[ERROR] Schema for relation '" << relation_name << "' not found.\n";
        return;
    }

    if (!fs::exists(data_path)) {
        std::cerr << "[ERROR] Data file for relation '" << relation_name << "' not found.\n";
        return;
    }

    // 3.2 + 3.3: Mostrar contenido del archivo de datos
    std::ifstream file(data_path);
    std::cout << "[INFO] Contents of '" << relation_name << "':\n";
    while (std::getline(file, line)) {
        std::cout << line << "\n";
    }

    file.close();   

    // 3.4: Tamaño del archivo
    auto size = fs::file_size(data_path);
    std::cout << "[INFO] File size: " << size << " bytes\n";
}
std::vector<std::string> tokenize(const std::string& str) {
    std::istringstream iss(str);
    std::vector<std::string> tokens;
    std::string token;
    while (iss >> token) tokens.push_back(token);
    return tokens;
}

void process_query(const std::string& command) {
    auto tokens = tokenize(command);

    // Example: & SELECT * FROM titanic WHERE age >= 18 | adults #
    if (tokens.size() < 6 || tokens[1] != "SELECT" || tokens[3] != "FROM") {
        std::cout << "Invalid SELECT query.\n";
        return;
    }

    std::string target_table = tokens[4];
    std::string output_table = "";  // Optional

    // Optional WHERE
    bool has_where = false;
    std::string where_column, where_op, where_value;

    for (size_t i = 5; i < tokens.size(); ++i) {
        if (tokens[i] == "WHERE" && i + 3 < tokens.size()) {
            has_where = true;
            where_column = tokens[i + 1];
            where_op     = tokens[i + 2];
            where_value  = tokens[i + 3];
            i += 3;
        } else if (tokens[i] == "|") {
            if (i + 1 < tokens.size()) {
                output_table = tokens[i + 1];
            }
        }
    }

    // Load schema from schema/schema.txt
    std::string schema_path = "schema/schema.txt";
    auto schema = load_schema(schema_path, target_table);  // You define this

    // Read from data/target_table.csv
    std::ifstream infile("data/" + target_table + ".csv");
    if (!infile.is_open()) {
        std::cout << "Could not open data/" << target_table << ".csv\n";
        return;
    }

    std::ofstream outfile;
    if (!output_table.empty()) {
        std::string outpath = "output/" + output_table + ".txt";
        outfile.open(outpath);
    }

    std::string line;
    while (std::getline(infile, line)) {
        auto row = split_csv_line(line);  // Define a function for this in io.cpp

        if (has_where) {
            if (!evaluate_condition(schema, row, where_column, where_op, where_value)) {
                continue;
            }
        }

        if (outfile.is_open()) {
            outfile << line << "\n";
        } else {
            std::cout << line << "\n";
        }
    }

    if (outfile.is_open()) {
        outfile.close();

        // Update schema
        save_schema_entry(schema_path, output_table, schema);  // define this in schema.cpp
        std::cout << "New table '" << output_table << "' created.\n";
    }
}

bool evaluate_condition(
    const std::vector<std::string>& schema,
    const std::vector<std::string>& row,
    const std::string& column,
    const std::string& op,
    const std::string& value
) {
    for (size_t i = 0; i + 1 < schema.size(); i += 2) {
        std::string attr_name = schema[i];
        std::string attr_type = schema[i + 1];

        // Trim spaces
        attr_name.erase(0, attr_name.find_first_not_of(" \t\r\n"));
        attr_name.erase(attr_name.find_last_not_of(" \t\r\n") + 1);

        if (attr_name == column) {
            size_t index = i / 2;  // each attribute has 2 entries (name, type)
            if (index >= row.size()) return false;

            std::string actual = row[index];

            try {
                if (attr_type == "int") {
                    int a = std::stoi(actual);
                    int b = std::stoi(value);
                    if (op == "==") return a == b;
                    if (op == "!=") return a != b;
                    if (op == ">")  return a > b;
                    if (op == "<")  return a < b;
                    if (op == ">=") return a >= b;
                    if (op == "<=") return a <= b;
                } else if (attr_type == "float") {
                    float a = std::stof(actual);
                    float b = std::stof(value);
                    if (op == "==") return a == b;
                    if (op == "!=") return a != b;
                    if (op == ">")  return a > b;
                    if (op == "<")  return a < b;
                    if (op == ">=") return a >= b;
                    if (op == "<=") return a <= b;
                } else {
                    // string comparison
                    if (op == "==") return actual == value;
                    if (op == "!=") return actual != value;
                    if (op == ">")  return actual > value;
                    if (op == "<")  return actual < value;
                    if (op == ">=") return actual >= value;
                    if (op == "<=") return actual <= value;
                }
            } catch (...) {
                return false;
            }
        }
    }

    return false;
}
