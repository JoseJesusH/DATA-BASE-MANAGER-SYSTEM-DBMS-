#ifndef DISK_H
#define DISK_H

#include "plate.h"
#include <vector>
#include <string>

class Disk {
private:
    int numPlates;
    int numTracks;
    int numSectors;
    int sectorSize;

    

    size_t totalCapacity; // NEW

    std::string basePath;
    std::string currentCsvName;
    std::vector<Plate> plates;

public:
    Disk(int plates, int tracks, int sectors, int sectorSize, const std::string& basePath, const std::string& csvName);

    Plate& getPlate(int plateId);
    std::vector<Plate>& getPlates();
    
    void setCurrentCsvName(const std::string& name);
    std::string getCurrentCsvName() const;
    
    void generateStructureReport(const std::string& outputPath) const;
    bool writeRecordToDisk(const std::string& record);

    size_t getTotalCapacity() const; // NEW

    int getSectorSize() const;
    int getPlateCount() const;
    int getTrackCount() const;
    int getSectorCount() const;
    std::string getBasePath() const;
    std::vector<Sector*> getAllSectors();



};

#endif

