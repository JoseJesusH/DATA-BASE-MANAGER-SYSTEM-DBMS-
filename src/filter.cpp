// filter.cpp
#include "filter.h"
#include "io.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <filesystem>
#include <cctype>


namespace fs = std::filesystem;

bool is_number(const std::string& s) {
    return !s.empty() && std::all_of(s.begin(), s.end(), ::isdigit);
}

bool compare(const std::string& lhs, const std::string& rhs, const std::string& op, const std::string& type) {
    if (type == "int") {
        if (!is_number(lhs) || !is_number(rhs)) return false;
        int a = std::stoi(lhs), b = std::stoi(rhs);
        if (op == "==") return a == b;
        if (op == "!=") return a != b;
        if (op == ">=") return a >= b;
        if (op == "<=") return a <= b;
        if (op == ">")  return a > b;
        if (op == "<")  return a < b;
    } else if (type == "float") {
        float a = std::stof(lhs), b = std::stof(rhs);
        if (op == "==") return a == b;
        if (op == "!=") return a != b;
        if (op == ">=") return a >= b;
        if (op == "<=") return a <= b;
        if (op == ">")  return a > b;
        if (op == "<")  return a < b;
    } else { // treat as string
        if (op == "==") return lhs == rhs;
        if (op == "!=") return lhs != rhs;
        return false;
    }
    return false;
}

void filter_relation(const std::string& relation_name, const std::string& attribute,
                     const std::string& op, const std::string& value, const std::string& new_relation_name) {

    std::ifstream schema_file("schema/schema.txt");
    std::ifstream data_file("output/" + relation_name + ".txt");

    if (!schema_file || !data_file) {
        std::cerr << "[ERROR] Schema or data file not found.\n";
        return;
    }

    std::vector<std::string> attr_names, attr_types;
    std::string line;
    bool schema_found = false;

    // Buscar línea del esquema de la relación
    while (getline(schema_file, line)) {
        std::stringstream ss(line);
        std::string token;
        getline(ss, token, '#'); // nombre de la relación

        if (token == relation_name) {
            schema_found = true;
            while (getline(ss, token, '#')) {
                attr_names.push_back(token);           // nombre atributo
                if (getline(ss, token, '#')) {
                    attr_types.push_back(token);       // tipo atributo
                }
            }
            break;
        }
    }

    if (!schema_found) {
        std::cerr << "[ERROR] Schema for relation '" << relation_name << "' not found.\n";
        return;
    }

    // Buscar índice del atributo
    int attr_index = -1;
    for (size_t i = 0; i < attr_names.size(); ++i) {
        if (attr_names[i] == attribute) {
            attr_index = i;
            break;
        }
    }

    if (attr_index == -1) {
        std::cerr << "[ERROR] Attribute not found in schema.\n";
        return;
    }

    // Filtrar datos
    std::ofstream new_data("output/" + new_relation_name + ".txt");
    while (getline(data_file, line)) {
        std::stringstream ss(line);
        std::string field;
        std::vector<std::string> fields;
        while (getline(ss, field, '#')) fields.push_back(field);

        if (fields.size() != attr_names.size()) continue; // seguridad

        if (compare(fields[attr_index], value, op, attr_types[attr_index])) {
            new_data << line << "\n";
        }
    }

    // Agregar nueva relación al archivo de esquemas
    std::ofstream schema_append("schema/schema.txt", std::ios::app);
    schema_append << new_relation_name;
    for (size_t i = 0; i < attr_names.size(); ++i) {
        schema_append << "#" << attr_names[i] << "#" << attr_types[i];
    }
    schema_append << "\n";

    std::cout << "[INFO] Filtered relation saved as '" << new_relation_name << "'.\n";
}