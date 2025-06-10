#include "plate.h"
#include <sstream>
#include <iomanip>
#include <filesystem>

namespace fs = std::filesystem;


Plate::Plate(int plateId, int numTracks, int numSectors, int sectorSize, const std::string& basePath)
    : id(plateId),
      topSurface(0, numTracks, numSectors, sectorSize, basePath + "/plate" + std::to_string(plateId) + "_top"),
      bottomSurface(1, numTracks, numSectors, sectorSize, basePath + "/plate" + std::to_string(plateId) + "_bottom") {}
      
const Surface& Plate::getTopSurface() const{
    return topSurface;
}

const Surface& Plate::getBottomSurface() const{
    return bottomSurface;
}
Surface& Plate::getTopSurface() {
    return topSurface;
}

Surface& Plate::getBottomSurface() {
    return bottomSurface;
}

int Plate::getId() const {
    return id;
}

