// block.cpp
#include "block.h"
#include <fstream>
#include <sstream>

Block::Block(int id, const std::vector<Sector*>& sectorList, const std::string& source)
    : blockID(id), sectors(sectorList), usedSize(0), recordCount(0), sourceFile(source) {
    totalSize = 0;
    for (const auto& sector : sectors) {
        totalSize += sector->getFreeSpace() + sector->getUsedSpace();
        usedSize += sector->getUsedSpace();
    }
}

bool Block::canFit(const std::string& record) const {
    return record.size() <= getFreeSpace();
}

bool Block::insertRecord(const std::string& record) {
    for (Sector* sector : sectors) {
        if (sector->hasSpaceFor(record)) {
            if (sector->writeRecord(record, "BlockID=" + std::to_string(blockID))) {
                usedSize += record.size();
                ++recordCount;
                return true;
            }
        }
    }
    return false;
}

std::string Block::getReport() const {
    std::ostringstream oss;
    oss << "[Block ID: " << blockID << "]\n";
    oss << "Total Size: " << totalSize << " bytes\n";
    oss << "Used Size: " << usedSize << " bytes\n";
    oss << "Free Space: " << getFreeSpace() << " bytes\n";
    oss << "Record Count: " << recordCount << "\n";
    oss << "Source File: " << (sourceFile.empty() ? "<unknown>" : sourceFile) << "\n";
    oss << "Sectors:\n";
    for (const auto& s : sectors) {
        oss << " - " << s->getPath()
            << " (Used: " << s->getUsedSpace()
            << "/" << s->getFreeSpace() + s->getUsedSpace() << ")\n";
    }
    return oss.str();
}

int Block::getID() const { return blockID; }
size_t Block::getFreeSpace() const { return totalSize - usedSize; }
size_t Block::getUsedSize() const { return usedSize; }
size_t Block::getTotalSize() const { return totalSize; }
int Block::getRecordCount() const { return recordCount; }
const std::vector<Sector*>& Block::getSectors() const { return sectors; }

void Block::writeHeaderToFile(const std::string& basePath) const {
    std::ofstream out(basePath + "/block_" + std::to_string(blockID) + "_header.txt");
    if (out.is_open()) {
        out << getReport();
        out.close();
    }
}

void Block::writeContentToFile(const std::string& basePath) const {
    std::ofstream out(basePath + "/block_" + std::to_string(blockID) + "_data.txt");
    if (out.is_open()) {
        for (const auto& s : sectors) {
            out << s->readData() << "\n";
        }
        out.close();
    }
}
