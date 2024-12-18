#ifndef MAP_H
#define MAP_H
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <vector>
#include <string>
using namespace std;

class Map {
public:
    Map(int width, int height);
    void setTile(int x, int y, int value);
    int getTile(int x, int y) const;
    int getWidth() const;
    int getHeight() const;
    void addFrame();
    Map loadFromCSV(const string& filename);

private:
    int width, height;
    std::vector<std::vector<int>> tiles;
};

#endif
