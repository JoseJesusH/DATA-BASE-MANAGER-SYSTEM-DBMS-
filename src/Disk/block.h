// block.h
#ifndef BLOCK_H
#define BLOCK_H

#include <vector>
#include <string>
#include "sector.h"

class Block {
private:
    int blockID;
    std::vector<Sector*> sectors;
    size_t totalSize;
    size_t usedSize;
    int recordCount;
    std::string sourceFile; // optional, for tracking origin (e.g., housing.csv)

public:
    Block(int id, const std::vector<Sector*>& sectorList, const std::string& source = "");

    bool canFit(const std::string& record) const;
    bool insertRecord(const std::string& record);
    std::string getReport() const;

    int getID() const;
    size_t getFreeSpace() const;
    size_t getUsedSize() const;
    size_t getTotalSize() const;
    int getRecordCount() const;
    const std::vector<Sector*>& getSectors() const;

    void writeHeaderToFile(const std::string& basePath) const;
    void writeContentToFile(const std::string& basePath) const;
};

#endif // BLOCK_H