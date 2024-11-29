#ifndef MAPWINDOW_H
#define MAPWINDOW_H

#include <SFML/Graphics.hpp>
#include "Map.h"
#include <iostream>

class MapWindow {
public:
    MapWindow(const Map& map, const std::string& algorithmName);
    void run();

private:
    void drawMap();
    void drawText();

    sf::RenderWindow window;
    sf::Font font;
    sf::Text text;
    const Map& map; // Referenz auf die anzuzeigende Map
};

#endif
