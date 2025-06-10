#ifndef IO_H
#define IO_H

#include <string>

bool fileExists(const std::string& filename);
void printLine(const std::string& line);
void loadCSVandSave(const std::string& filepath, const std::string& outDir, const std::string& schemaDir);
std::vector<std::string> split_csv_line(const std::string& line);

#endif // IO_H

