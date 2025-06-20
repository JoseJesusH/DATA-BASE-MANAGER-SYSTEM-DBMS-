#ifndef SECTOR_H
#define SECTOR_H

#include <string>
#include <vector>

class Sector {
private:
    std::string filePath;
    size_t capacity;
    size_t usedSpace; // NEW
    bool occupied;
    
    std::string header = "";          // store initial header
    bool headerWritten = false;       // control one-time header write


public:
    Sector(const std::string& path, size_t cap = 4096);

    bool writeRecord(const std::string& record, const std::string& locationInfo = "", const std::string& sourceFile="");

    std::string readData() const;
    void clear();
    
    bool isOccupied() const;
    bool hasSpaceFor(const std::string& record) const; // NEW
    size_t getFreeSpace() const; // NEW
    size_t getUsedSpace() const; // NEW

    std::string getPath() const;
};

#endif
