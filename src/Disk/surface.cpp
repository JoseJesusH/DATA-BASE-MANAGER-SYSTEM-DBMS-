#include "surface.h"
#include <sstream>
#include <iomanip>
#include <filesystem>

namespace fs = std::filesystem;

Surface::Surface(int surfaceId, int numTracks, int numSectors, size_t sectorSize, const std::string& basePath)
    : id(surfaceId) {
    std::ostringstream oss;
    oss << std::setw(3) << std::setfill('0') << surfaceId;
    std::string surfacePath = basePath + "/surface" + oss.str();

    fs::create_directories(surfacePath);

    for (int i = 0; i < numTracks; ++i) {
        tracks.emplace_back(i, numSectors, sectorSize, surfacePath); 

    }
}

Track& Surface::getTrack(int trackId) {
    return tracks.at(trackId);
}
const Track& Surface::getTrack(int trackId) const {
    return tracks.at(trackId);
}
const std::vector<Track>& Surface::getTracks() const {
    return tracks;
}

int Surface::getId() const {
    return id;
}

