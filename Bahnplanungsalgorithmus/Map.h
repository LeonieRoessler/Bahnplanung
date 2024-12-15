#ifndef MAP_H
#define MAP_H

#include <vector>

class Map {
public:
    Map(int width, int height);
    void setTile(int x, int y, int value);
    int getTile(int x, int y) const;
    int getWidth() const;
    int getHeight() const;
    void addFrame();

private:
    int width, height;
    std::vector<std::vector<int>> tiles;
};

#endif
