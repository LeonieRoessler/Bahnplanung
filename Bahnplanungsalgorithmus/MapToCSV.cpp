#include "MapToCSV.h"
#include <fstream>
#include <stdexcept>

// Konstruktor: Nimmt eine Referenz auf eine Map entgegen
MapToCSV::MapToCSV(const Map& map) : map(map) {}

// Exportiert die Map als CSV in die angegebene Datei
void MapToCSV::exportToFile(const std::string& filename) const {
    std::ofstream outFile(filename);

    if (!outFile.is_open()) {
        throw std::runtime_error("Fehler beim Öffnen der Datei: " + filename);
    }

    // Gehe durch die Kacheln der Map und schreibe sie in die Datei
    for (int y = 0; y < map.getHeight(); ++y) {
        for (int x = 0; x < map.getWidth(); ++x) {
            outFile << map.getTile(x, y);
            if (x < map.getWidth() - 1) {
                outFile << ","; // Komma nach jedem Wert außer am Ende der Zeile
            }
        }
        outFile << "\n"; // Neue Zeile nach jeder Zeile der Map
    }

    outFile.close();
}
