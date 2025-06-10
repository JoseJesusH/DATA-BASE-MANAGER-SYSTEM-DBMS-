#include "schema.h"
#include <fstream>
#include <sstream>
#include <iostream>

RelationSchema parseSchemaFile(const std::string& schemaPath) {
    RelationSchema schema;
    std::ifstream file(schemaPath);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open schema file " << schemaPath << std::endl;
        return schema;
    }
    std::string line;
    while (getline(file, line)) {
        std::stringstream ss(line);
        getline(ss, schema.relationName, ',');
        std::string attr;
        while (getline(ss, attr, ',')) {
            size_t pos = attr.find(':');
            if (pos != std::string::npos) {
                Attribute a;
                a.name = attr.substr(0, pos);
                a.type = attr.substr(pos + 1);
                schema.attributes.push_back(a);
            }
        }
        break; // Only one line for now
    }
    file.close();
    return schema;
}

void saveSchema(const RelationSchema& schema, const std::string& path) {
    std::ofstream file(path);
    if (!file.is_open()) {
        std::cerr << "Error: Could not save schema to " << path << std::endl;
        return;
    }
    file << schema.relationName;
    for (const auto& attr : schema.attributes) {
        file << "," << attr.name << ":" << attr.type;
    }
    file << "\n";
    file.close();
}

std::vector<std::string> load_schema(const std::string& schema_path, const std::string& relation_name) {
    std::ifstream infile(schema_path);
    std::string line;

    while (std::getline(infile, line)) {
        // Remove leading/trailing whitespace
        line.erase(0, line.find_first_not_of(" \t\r\n"));
        line.erase(line.find_last_not_of(" \t\r\n") + 1);

        if (line.empty() || line[0] == '#') continue;  // skip comments or empty lines

        std::istringstream ss(line);
        std::string table;
        std::getline(ss, table, '#');

        table.erase(0, table.find_first_not_of(" \t\r\n"));
        table.erase(table.find_last_not_of(" \t\r\n") + 1);

        if (table == relation_name) {
            std::vector<std::string> columns;
            std::string part;
            while (std::getline(ss, part, '#')) {
                part.erase(0, part.find_first_not_of(" \t\r\n"));
                part.erase(part.find_last_not_of(" \t\r\n") + 1);
                columns.push_back(part);
            }
            return columns;
        }
    }

    return {};  // Not found
}

void save_schema_entry(const std::string& schema_path, const std::string& table, const std::vector<std::string>& columns) {
    std::ifstream infile(schema_path);
    std::string line;

    // Avoid duplicate schema entries
    while (std::getline(infile, line)) {
        if (line.substr(0, table.size()) == table && line[table.size()] == '#') {
            return; // Already exists
        }
    }

    std::ofstream outfile(schema_path, std::ios::app);  // append mode
    if (!outfile.is_open()) return;

    outfile << table;
    for (const auto& col : columns) {
        outfile << "#" << col;
    }
    outfile << "\n";
}
