#include "track.h"
#include <filesystem>
#include <iomanip>
#include <sstream>
#include <fstream>

namespace fs = std::filesystem;

Track::Track(int trackId, int numSectors, size_t sectorSize, const std::string& basePath)
    : id(trackId) {
    std::ostringstream oss;
    oss << std::setw(3) << std::setfill('0') << trackId;
    std::string trackFolder = basePath + "/track" + oss.str();

    fs::create_directories(trackFolder);

    for (int i = 0; i < numSectors; ++i) {
        std::ostringstream sectorPath;
        sectorPath << trackFolder << "/sector" << std::setw(3) << std::setfill('0') << i << ".txt";
        sectors.emplace_back(sectorPath.str(), sectorSize);  // Pass sectorSize here
    }
}


Sector& Track::getSector(int sectorId) {
    return sectors.at(sectorId);
}

const Sector& Track::getSector(int sectorId) const {
    return sectors.at(sectorId);
}

const std::vector<Sector>& Track::getSectors() const {
    return sectors;
}

int Track::getId() const {
    return id;
}

int Track::countBlocksInTrack() const {
    int count = 0;
    for (const Sector& sector : sectors) {
        std::ifstream file(sector.getPath());
        std::string line;
        while (std::getline(file, line)) {
            if (!line.empty()) {
                ++count;
            }
        }
    }
    return count;
}
