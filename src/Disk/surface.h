#ifndef SURFACE_H
#define SURFACE_H

#include "track.h"
#include <vector>
#include <string>

class Surface {
private:
    int id;
    std::vector<Track> tracks;

public:
    Surface(int surfaceId, int numTracks, int numSectors, size_t sectorSize, const std::string& basePath);

    const Track& getTrack(int trackId) const;
    Track& getTrack(int trackId);
    const std::vector<Track>& getTracks() const;
    int getId() const;
};

#endif

