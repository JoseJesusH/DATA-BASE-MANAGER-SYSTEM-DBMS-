#ifndef TRACK_H
#define TRACK_H

#include "sector.h"
#include <vector>
#include <string>

class Track {
private:
    int id;
    std::vector<Sector> sectors;

public:
    Track(int trackId, int numSectors, size_t sectorSize, const std::string& basePath);
    
    const Sector& getSector(int sectorId) const;
    Sector& getSector(int sectorId);
    const std::vector<Sector>& getSectors() const;
    int getId() const;
};

#endif

