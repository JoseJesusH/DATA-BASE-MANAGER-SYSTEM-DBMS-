#ifndef SELECT_H
#define SELECT_H

#include <string>

// Placeholder for SELECT functionality
void select_relation(const std::string& relation_name);
void process_query(const std::string& command);
bool evaluate_condition(
    const std::vector<std::string>& schema,
    const std::vector<std::string>& row,
    const std::string& column,
    const std::string& op,
    const std::string& value
);
#endif // SELECT_H

