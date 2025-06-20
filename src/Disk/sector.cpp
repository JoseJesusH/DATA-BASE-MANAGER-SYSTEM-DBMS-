#include "sector.h"
#include <fstream>
#include <sstream>
#include<iostream>

Sector::Sector(const std::string& path, size_t cap)
    : filePath(path), capacity(cap), usedSpace(0), occupied(false) {}

bool Sector::hasSpaceFor(const std::string& record) const {
    size_t recordSize = record.size() + 1; // +1 for '\n'
    return usedSpace + record.size() <= capacity;
}

size_t Sector::getFreeSpace() const {
    return capacity - usedSpace;
}

size_t Sector::getUsedSpace() const {
    return usedSpace;
}
bool Sector::writeRecord(const std::string& record, const std::string& locationInfo, const std::string& sourceFile) {
    std::string fullRecord = record;
    if (!locationInfo.empty()) {
        fullRecord += "|" + locationInfo;
    }
    fullRecord += "\n"; // always end with newline

    // Prepare to write
    std::ofstream outFile;
    outFile.open(filePath, std::ios::app);
    if (!outFile.is_open()) return false;

    // First write: header
    if (!occupied) {
        std::ostringstream headerStream;
        headerStream << "Source File: " << sourceFile << "\n";
        headerStream << "[Sector Metadata]\n";
        headerStream << "Capacity: " << capacity << " bytes\n";
        headerStream << "Free Space: (placeholder)\n";
        headerStream << "Used Space: (placeholder)\n";
        headerStream << "Source File: (to be set dynamically or from block if available)\n";
        headerStream << "----------------------------------------\n";

        header = headerStream.str(); // store for size tracking
        usedSpace += header.size();  // add to used space
        occupied = true;
    }

    size_t potentialUsed = usedSpace + fullRecord.size();
    if (potentialUsed > capacity) {
        return false; // ❌ no space
    }

    outFile << (!headerWritten ? header : ""); // write header only once
    outFile << fullRecord;
    outFile.close();

    usedSpace += fullRecord.size();
    headerWritten = true;

    // Now update the header with correct values
    // Since the file already exists, we’ll reopen and rewrite it completely with accurate header
    std::ifstream in(filePath);
    std::string allContent((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    in.close();

    // Rebuild header with correct values
    std::ostringstream correctedHeader;
    correctedHeader << "Source File: " << sourceFile << "\n";
    correctedHeader << "[Sector Metadata]\n";
    correctedHeader << "Capacity: " << capacity << " bytes\n";
    correctedHeader << "Free Space: " << (capacity - usedSpace) << " bytes\n";
    correctedHeader << "Used Space: " << usedSpace << " bytes\n";
    correctedHeader << "Source File: (to be set dynamically or from block if available)\n";
    correctedHeader << "----------------------------------------\n";

    std::string newHeader = correctedHeader.str();

    // Remove old header (from start to first separator)
    size_t oldHeaderEnd = allContent.find("----------------------------------------\n");
    if (oldHeaderEnd != std::string::npos) {
        oldHeaderEnd += std::string("----------------------------------------\n").size();
        std::string restOfContent = allContent.substr(oldHeaderEnd);

        std::ofstream out(filePath);
        out << newHeader << restOfContent;
        out.close();
    }

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
