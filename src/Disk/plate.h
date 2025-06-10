#ifndef PLATE_H
#define PLATE_H

#include "surface.h"
#include <vector>
#include <string>

class Plate {
private:
    int id;
    Surface topSurface;
    Surface bottomSurface;

public:
    Plate(int plateId, int numTracks, int numSectors, int sectorSize, const std::string& basePath);

    Surface& getTopSurface();      // mutable
    Surface& getBottomSurface();   // mutable
    const Surface& getTopSurface() const;
    const Surface& getBottomSurface() const;
    int getId() const;
};

#endif

