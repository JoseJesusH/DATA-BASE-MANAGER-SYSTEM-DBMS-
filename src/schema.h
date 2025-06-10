    #ifndef SCHEMA_H
    #define SCHEMA_H

    #include <string>
    #include <vector>

    struct Attribute {
        std::string name;
        std::string type; // string, int, float
    };

    struct RelationSchema {
        std::string relationName;
        std::vector<Attribute> attributes;
    };

    RelationSchema parseSchemaFile(const std::string& schemaPath);
    void saveSchema(const RelationSchema& schema, const std::string& path);
    std::vector<std::string> load_schema(const std::string& schema_path, const std::string& relation_name);
    void save_schema_entry(const std::string& schema_path, const std::string& table, const std::vector<std::string>& columns);

    #endif // SCHEMA_H

