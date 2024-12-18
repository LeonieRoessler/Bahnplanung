#ifndef MAPTOCSV_H
#define MAPTOCSV_H

#include "Map.h"
#include <string>

class MapToCSV {
public:
    // Konstruktor
    MapToCSV(const Map& map);

    // Exportiert die Map als CSV in die angegebene Datei
    void exportToFile(const std::string& filename) const;

private:
    const Map& map; // Referenz auf die Map
};

#endif
