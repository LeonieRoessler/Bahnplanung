#include <SFML/Graphics.hpp>
#include "Map.h"

Map::Map(int width, int height)
    : width(width), height(height), tiles(height, std::vector<int>(width, 0)) {

}

void Map::addFrame() {
    // Füge einen Rahmen aus 1en hinzu
    for (int i = 0; i < height; ++i) {
        tiles[i][0] = 1;                 // Linker Rand
        tiles[i][width - 1] = 1;         // Rechter Rand
    }
    for (int j = 0; j < width; ++j) {
        tiles[0][j] = 1;                 // Oberer Rand
        tiles[height - 1][j] = 1;        // Unterer Rand
    }
}

void Map::setTile(int x, int y, int value) {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        tiles[y][x] = value;
    }
}

int Map::getTile(int x, int y) const {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        return tiles[y][x];
    }
    return -1; // Ungültige Position
}

int Map::getWidth() const {
    return width;
}

int Map::getHeight() const {
    return height;
}


Map Map::loadFromCSV(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        throw runtime_error("Datei konnte nicht geöffnet werden: " + filename);
    }

    vector<std::vector<int>> tiles;
    string line;

    // Lies Zeile für Zeile
    while (std::getline(file, line)) {
        vector<int> row;
        stringstream ss(line);
        string cell;

        // Teile die Zeile in Zellen (getrennt durch Kommata)
        while (getline(ss, cell, ',')) {
            row.push_back(stoi(cell)); // Konvertiere jede Zelle in einen Integer
        }

        tiles.push_back(row);
    }

    file.close();

    if (tiles.empty()) {
        throw runtime_error("CSV-Datei ist leer: " + filename);
    }

    // Erstelle ein Map-Objekt mit den Dimensionen aus der CSV
    int height = tiles.size();
    int width = tiles[0].size();
    Map map(width, height);

    // Übertrage die Werte in das Map-Objekt
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            map.setTile(x, y, tiles[y][x]);
        }
    }

    return map;
}