#include "MapWindow.h"

MapWindow::MapWindow(const Map& map, const std::string& algorithmName)
    : window(sf::VideoMode(map.getWidth() * 50, map.getHeight() * 50 + 40), "Bahnplanung"),
    map(map) {
    if (!font.loadFromFile("assets/arial.ttf")) {
        std::cerr << "Fehler beim Laden der Schriftart!" << std::endl;
    }
    text.setFont(font);
    text.setString("Algorithmus: " + algorithmName);
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::Black);
    text.setPosition(10, map.getHeight() * 50 + 10);
}

void MapWindow::run() {
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::White);

        drawMap();
        drawText();

        window.display();
    }
}

void MapWindow::drawMap() {
    const int TILE_SIZE = 50;
    for (int y = 0; y < map.getHeight(); ++y) {
        for (int x = 0; x < map.getWidth(); ++x) {
            sf::RectangleShape tile(sf::Vector2f(TILE_SIZE, TILE_SIZE));
            tile.setPosition(x * TILE_SIZE, y * TILE_SIZE);

            if (map.getTile(x, y) == 1) {
                tile.setFillColor(sf::Color::Black); // Hindernis
            }
            else {
                tile.setFillColor(sf::Color::White); // Freies Feld
            }
            tile.setOutlineThickness(1);
            tile.setOutlineColor(sf::Color::Black);

            window.draw(tile);
        }
    }
}

void MapWindow::drawText() {
    window.draw(text);
}
