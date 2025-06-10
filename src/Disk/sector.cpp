#include "sector.h"
#include <fstream>
#include <sstream>
#include<iostream>

Sector::Sector(const std::string& path, size_t cap)
    : filePath(path), capacity(cap), usedSpace(0), occupied(false) {}

bool Sector::hasSpaceFor(const std::string& record) const {
    return usedSpace + record.size() <= capacity;
}

size_t Sector::getFreeSpace() const {
    return capacity - usedSpace;
}

size_t Sector::getUsedSpace() const {
    return usedSpace;
}

bool Sector::writeRecord(const std::string& record, const std::string& locationInfo) {
    std::string fullRecord = record;
    if (!locationInfo.empty()) {
        fullRecord += "|" + locationInfo;
    }
    fullRecord += "\n"; // newline terminator

    if (!hasSpaceFor(fullRecord)) return false;

    std::ofstream outFile(filePath, std::ios::app); // append mode
    if (!outFile.is_open()) return false;

    outFile << fullRecord;
    outFile.close();

    usedSpace += fullRecord.size();
    occupied = true;

    std::cout << "[INFO] Sector capacity: " << capacity 
          << " bytes | Used: " << usedSpace 
          << " | Free: " << getFreeSpace() << std::endl;

    return true;
}

std::string Sector::readData() const {
    std::ifstream inFile(filePath);
    if (!inFile.is_open()) return "";

    std::string content((std::istreambuf_iterator<char>(inFile)),
                         std::istreambuf_iterator<char>());
    inFile.close();
    return content;
}

void Sector::clear() {
    std::ofstream outFile(filePath, std::ofstream::trunc);
    if (outFile.is_open()) {
        outFile.close();
        usedSpace = 0;
        occupied = false;
    }
}

bool Sector::isOccupied() const {
    return occupied;
}

std::string Sector::getPath() const {
    return filePath;
}
