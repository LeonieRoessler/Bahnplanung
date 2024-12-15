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
