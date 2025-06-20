#include "disk.h"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <filesystem>
#include<iostream>

namespace fs = std::filesystem;

Disk::Disk(int platesCount, int tracksCount, int sectorsCount, int sectorSizeBytes, const std::string& path, const std::string& csvName)
    : numPlates(platesCount), numTracks(tracksCount), numSectors(sectorsCount), sectorSize(sectorSizeBytes), basePath(path), currentCsvName(csvName) {
    
    totalCapacity = static_cast<size_t>(platesCount) * 2 * tracksCount * sectorsCount * sectorSizeBytes; // 2 = surfaces

    fs::create_directories(basePath);

    for (int i = 0; i < numPlates; ++i) {
       
        plates.emplace_back(i, numTracks, numSectors, sectorSize, basePath);

    }
}

bool Disk::writeRecordToDisk(const std::string& record) {
    for (int p = 0; p < numPlates; ++p) {
        Plate& plate = plates[p];

        for (int s = 0; s < 2; ++s) { // 0 = Top, 1 = Bottom
            Surface& surface = (s == 0) ? plate.getTopSurface() : plate.getBottomSurface();
            
            std::string surfaceLabel = (s == 0) ? "Top" : "Bottom";

            const std::vector<Track>& tracks = surface.getTracks();
           

            for (size_t t = 0; t < tracks.size(); ++t) {
                const Track& track = tracks[t];
                const std::vector<Sector>& sectors = track.getSectors();

                for (size_t sec = 0; sec < sectors.size(); ++sec) {
                    const Sector& sector = sectors[sec];

                    if (sector.hasSpaceFor(record)) {
                        // Reconstruct non-const reference to write
                        //Sector& mutableSector = surface.getTrack(t).getSector(sec);
                        Track& mutableTrack = surface.getTrack(t); // una sola vez
                        Sector& mutableSector = mutableTrack.getSector(sec);

                        std::ostringstream location;
                        location << "Plate " << p
                                 << " - Surface " << surfaceLabel
                                 << " - Track " << t
                                 << " - Sector " << sec;

                        if (mutableSector.writeRecord(record, location.str(), currentCsvName)) {
                           return true;
                        }
                        //bool success = mutableSector.writeRecord(record, location.str());
                        //return success;
                    }
                }
            }
        }
    }

    // If no space found
    std::cerr << "❌ No space found to write record.\n";
    return false;
}


Plate& Disk::getPlate(int plateId) {
    return plates.at(plateId);
}

std::vector<Plate>& Disk::getPlates() {
    return plates;
}

int Disk::getPlateCount() const {
    return numPlates;
}

int Disk::getTrackCount() const {
    return numTracks;
}

int Disk::getSectorCount() const {
    return numSectors;
}
int Disk::getSectorSize() const {
    return sectorSize;
}

std::string Disk::getBasePath() const {
    return basePath;
}

size_t Disk::getTotalCapacity() const {
    return totalCapacity;
}

void Disk::setCurrentCsvName(const std::string& name) {
    currentCsvName = name;
}

std::string Disk::getCurrentCsvName() const {
    return currentCsvName;
}


void Disk::generateStructureReport(const std::string& outputPath) const {
    std::ofstream out(outputPath);
    if (!out.is_open()) {
        throw std::runtime_error("Could not open report file: " + outputPath);
    }

    out << "=============================================\n";
    out << "         MEGATRONJ3000J DISK REPORT\n";
    out << "=============================================\n";
    out << "📁 Base path: " << basePath << "\n";
    out << "🔧 Configuration: " << numPlates << " plate(s), "
        << numTracks << " track(s)/surface, "
        << numSectors << " sector(s)/track\n\n";

    for (const auto& plate : plates) {
        out << "🪞 Plate #" << plate.getId() << "\n";

        const Surface& top = plate.getTopSurface();
        const Surface& bottom = plate.getBottomSurface();

        for (const Surface& surf : {top, bottom}) {
            std::string surfaceLabel = (surf.getId() % 2 == 0) ? "Top" : "Bottom";// Optional logic

            out << " ├─ Surface [" << surfaceLabel << "] ID: " << surf.getId() << "\n";

            for (const Track& track : surf.getTracks()) {
                out << " │   ├─ Track #" << std::setw(2) << track.getId() << ": "
                    << track.getSectors().size() << " sectors\n";
            }
        }
        out << "\n";
    }
    int totalBlocks = 0;

    for (const auto& plate : plates) {
        int plateBlockCount = 0;

        for (const Surface* surf : { &plate.getTopSurface(), &plate.getBottomSurface() }) {
            std::string surfaceLabel = (surf->getId() % 2 == 0) ? "Top" : "Bottom";
            out << " ├─ Surface [" << surfaceLabel << "] ID: " << surf->getId() << "\n";

            for (const Track& track : surf->getTracks()) {
                // OPTIONAL: You might need to count blocks from metadata elsewhere
                int blocksInTrack = track.countBlocksInTrack(); // custom function you implement
                plateBlockCount += blocksInTrack;

                out << " │   ├─ Track #" << track.getId() << ": "
                    << track.getSectors().size() << " sectors, "
                    << blocksInTrack << " blocks\n";
            }
        }

        out << " 🎯 Total Blocks in Plate #" << plate.getId() << ": " << plateBlockCount << "\n\n";
        totalBlocks += plateBlockCount;
    }
    out << "📦 Total Blocks in Disk: " << totalBlocks << "\n";
    out << "💾 Total Disk Capacity: " << totalCapacity << " bytes ("
        << (totalCapacity / 1024.0) << " KB | "
        << (totalCapacity / (1024.0 * 1024)) << " MB)\n";

    out << "========== END OF STRUCTURE REPORT ==========\n";
    out.close();
}


std::vector<Sector*> Disk::getAllSectors() {
    std::vector<Sector*> allSectors;

    for (auto& plate : plates) {
        for (Surface* surface : { &plate.getTopSurface(), &plate.getBottomSurface() }) {
            for (const Track& track : surface->getTracks()) {
                for (const Sector& sector : track.getSectors()) {
                    allSectors.push_back(const_cast<Sector*>(&sector)); // Remove constness
                }
            }
        }
    }

    return allSectors;
}



