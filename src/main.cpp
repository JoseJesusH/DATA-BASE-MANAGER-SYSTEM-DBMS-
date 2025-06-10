#include "io.h"
#include "select.h"
#include "filter.h"
#include <iostream>
#include "Disk/disk.h"
#include <fstream>


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
    

    Disk disk(numPlates, numTracks, numSectors, sectorSize, diskPath); //platos/pistas/sectores
    
    
    

    // ✅ NEW: Write Titanic.csv line by line
    std::ifstream titanicFile("data/titanic.csv");
    if (!titanicFile.is_open()) {
        std::cerr << "❌ No se pudo abrir el archivo Titanic.csv\n";
    } else {
        std::cout << "📥 Escribiendo registros de Titanic.csv al disco...\n";
        std::string line;
        std::getline(titanicFile, line);
 
        int count = 0;
        while (std::getline(titanicFile, line)) {
            bool success = disk.writeRecordToDisk(line);
            if (success) {
                std::cout << "✅ Registro #" << ++count << " escrito correctamente\n";
            } else {
                std::cerr << "❌ No hay espacio para el registro #" << ++count << "\n";
                break; // Stop if full
            }
        }
        titanicFile.close();
    }

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