#include "io.h"
#include "select.h"
#include "filter.h"
#include <iostream>
#include "Disk/disk.h"
#include <fstream>
#include "Disk/block.h"


int main() {

    std::string command;
    std::cout << "MEGATRON 3000\n>> ";

    int numPlates, numTracks, numSectors, sectorSize;
    std::string diskPath;

    std::cout << "Ingrese número de platos: ";
    std::cin >> numPlates;
    std::cout << "Ingrese número de pistas por superficie: ";
    std::cin >> numTracks;
    std::cout << "Ingrese número de sectores por pista: ";
    std::cin >> numSectors;
    std::cout << "Ingrese capacidad por sector (bytes): ";
    std::cin >> sectorSize;
    std::cin.ignore(); // limpiar newline

    // BONUS: Validación mínima del input
    if (numPlates <= 0 || numTracks <= 0 || numSectors <= 0) {
        std::cerr << "❌ Todos los valores deben ser mayores que cero.\n";
        return 1;
    }
    std::cout << "Ingrese la ruta base del disco (ej. 'output/disk'): ";
    std::getline(std::cin, diskPath);
    

    Disk disk(numPlates, numTracks, numSectors, sectorSize, diskPath, "titanic.csv"); //platos/pistas/sectores
    



    // ✅ NEW: Write Titanic.csv line by line
    std::ifstream titanicFile("data/titanic.csv");
    if (!titanicFile.is_open()) {
        std::cerr << "❌ No se pudo abrir el archivo titanic.csv\n";
    } else {
        std::cout << "📥 Escribiendo registros de titanic.csv al disco...\n";
        std::string line;
        std::getline(titanicFile, line);
        
        int count = 0;
        while (std::getline(titanicFile, line)) {
            bool success = disk.writeRecordToDisk(line);
            if (success) {
                std::cout << "✅ Registro #" << ++count << " escrito correctamente\n";
            } else {
                std::cerr << "❌ Registro #" << ++count << " no se pudo escribir (sin espacio?)\n";
            }   
        }
        titanicFile.close();
        
        std::vector<Block> blocks;
        std::vector<Sector*> currentSectors;
        int sectorsPerBlock = 4; // example: one block = 4 sectors
        int blockID = 0;



        // Flatten all sectors from the disk into a list
        std::vector<Sector*> allSectors = disk.getAllSectors(); // <-- You'll need to expose this method

        for (size_t i = 0; i < allSectors.size(); ++i) {
            currentSectors.push_back(allSectors[i]);
            if (currentSectors.size() == static_cast<size_t>(sectorsPerBlock) || i == allSectors.size() - 1){
                blocks.emplace_back(blockID++, currentSectors, "titanic.csv");
                currentSectors.clear();
            }
        }
        for (const auto& block : blocks) {
            block.writeHeaderToFile("blocks/");
            block.writeContentToFile("blocks/");
            
        }    
}

        /*
        std::string recordLine;
    
        for (auto& block : blocks) {
            while (std::getline(titanicFile, recordLine)) {
                if (block.canFit(recordLine)) {
                    block.insertRecord(recordLine);
                    std::cout << "✅ Registro #" << ++count << " insertado en bloque #" << block.getID() << "\n";
                } else {
                    break; // Try next block
                }
            }   
            if (titanicFile.eof()) break;
        }
        */

    try { 
        disk.generateStructureReport("output/disk_report.txt");
        std::cout << "Disk report generated.\n";
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }
    
    std::string path = "data/titanic.csv"; // or housing.csv
    loadCSVandSave(path, "output", "schema/schema.txt");
    // filter_relation("titanic", "Age", ">=", "30", "titanic_adults");
    select_relation("titanic_adults");

    while (std::getline(std::cin, command)) {
        if (command == "Quit") break;
        if (command.front() == '&' && command.back() == '#') {
            process_query(command);  // <- handle it in select.cpp
        } else {
            std::cout << "Invalid command format. Use & ... #\n";
        }
        std::cout << ">> ";
    }
    
    
    return 0;
}
