#ifndef FILTER_H
#define FILTER_H

#include <string>

// Placeholder for filter functionality (WHERE criteria)
bool applyFilter(const std::string& inputFile, const std::string& outputFile, const std::string& condition);
void filter_relation(const std::string& relation_name, const std::string& attribute, const std::string& op, const std::string& value, const std::string& new_relation_name);

#endif // FILTER_H

