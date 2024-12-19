#include "ResultsWindow.h"
#include <math.h>

ResultsWindow::ResultsWindow(Map& originalMap, const std::vector<Algorithm>& algorithms, vector<ParseJson>& parsedResults)
    : map(originalMap), algorithms(algorithms), parsedResults(parsedResults), button(sf::Vector2f(100, 30)), buttonText("Close", font, 20) {
    // Fenster initialisieren
    window.create(sf::VideoMode(2000, 900), "Results");

    // Schriftart laden
    if (!font.loadFromFile("assets/arial.ttf")) {
        std::cerr << "Fehler beim Laden der Schriftart!" << std::endl;
    }

    for (const auto& result : parsedResults) {
        groupedResults[result.getAlgorithm()].push_back(&result);
    }

    // Button initialisieren
    button.setFillColor(sf::Color::Blue);
    button.setPosition(map.getWidth()*5, map.getHeight() * 10 + 20);
    button.setOutlineThickness(2);
    button.setOutlineColor(sf::Color::White);

    // Text für den Button
    buttonText.setFillColor(sf::Color::White);
    buttonText.setPosition(map.getWidth() * 5+3, map.getHeight() * 10 + 25); // Zentriert auf dem Button
}

// Hauptschleife für das Fenster
void ResultsWindow::run() {
    while (window.isOpen()) {
        handleEvents();
        draw();
    }
}

void ResultsWindow::close() {
    while (window.isOpen()) {
        window.close();
    }
}


void ResultsWindow::draw() {
    window.clear(sf::Color(169, 169, 169));
    int xOffset = map.getWidth() * 10 + 5; // Startposition für die Darstellung der Karten und Ergebnisse
    if (map.getWidth() < 13) {
        tileSize = 10;
        drawMap(map, 0, 50);
        tileSize = 30;
    }
    else if (map.getWidth() > 12 && map.getWidth() < 30) {
        tileSize = 10;
        drawMap(map, 0, 50);
        tileSize = 20;
    }
    else if (map.getWidth() > 30 && map.getWidth() < 70) {
        tileSize = 2;
        drawMap(map, 0, 50);
        xOffset = map.getWidth() * 2 + 5;
        tileSize = 5;
    }
    else {
        tileSize = 2;
        drawMap(map, 0, 50);
        xOffset = map.getWidth() * 2 + 5;
        tileSize = 3;
    }




    for (const auto& entry : groupedResults) {
        string algorithmName = entry.first;
        int mapWidth = 0;
        drawText("Algorithm: " + algorithmName, xOffset, 20);
        int yOffset = 10;
        for (const auto& result : entry.second) {
            
            string language = result->getLanguage();
            float computingTime = result->getComputingTime();
            float memoryUsage = result->getMemoryUsage();
            int pathLength = result->pathLength;
            mapWidth = result->getAlgorithmMap().getWidth();
            drawPath(map, result->getPath(), xOffset, 50);
            if (mapWidth < 25)
                drawNumbers(result->getAlgorithmMap(), xOffset, 50);
            string info = language + ": ";
            drawText(info, xOffset, 100 + result->getAlgorithmMap().getHeight() * tileSize + yOffset);
            info = "Computing Time: " + to_string(computingTime) + "ms";
            drawText(info, xOffset, 100 + result->getAlgorithmMap().getHeight() * tileSize + yOffset + 20);
            info = "Memory Usage: " + to_string(memoryUsage) + "MB";
            drawText(info, xOffset, 100 + result->getAlgorithmMap().getHeight() * tileSize + yOffset + 40);
            info = "Path Length: " + to_string(pathLength);
            drawText(info, xOffset, 100 + result->getAlgorithmMap().getHeight() * tileSize + yOffset + 60);

            yOffset+=100;
        }
        
        if (mapWidth * tileSize - tileSize < 400) {
            xOffset += 400;
        }
        else {
            xOffset += mapWidth * tileSize - tileSize;
        }
    }


    // Button zeichnen
    window.draw(button);
    window.draw(buttonText);

    window.display();
}

void ResultsWindow::drawPath(const Map& map, const vector<pair<int, int>>& path, int x, int y) {
    //Original Map 
    for (int row = 0; row < map.getHeight(); ++row) {
        for (int col = 0; col < map.getWidth(); ++col) {
            sf::RectangleShape tile(sf::Vector2f(tileSize, tileSize));
            tile.setPosition(x + col * tileSize, y + row * tileSize);
            tile.setOutlineThickness(1);
            tile.setOutlineColor(sf::Color::Black);
            // Überprüfe, ob das aktuelle Tile Teil des Pfades ist
            auto it = std::find(path.begin(), path.end(), std::make_pair(row, col));
            if (map.getTile(col, row) == 2) {
                tile.setFillColor(sf::Color::Blue); // Start
            }
            else if (map.getTile(col, row) == 3) {
                tile.setFillColor(sf::Color::Green); // Ziel
            }
            else if (it != path.end()) {
                // Wenn das Feld im Pfad ist, setze es auf Rot
                tile.setFillColor(sf::Color::Red);
            }
            else {
                // Sonst, je nach Tile-Typ, setze die entsprechende Farbe
                if (map.getTile(col, row) == 1) {
                    tile.setFillColor(sf::Color::Black); // Hindernisse
                }
                else {
                    tile.setFillColor(sf::Color::White); // Freie Felder
                }
            }
            window.draw(tile);
        }
    }
}

void ResultsWindow::drawNumbers(const Map& map, int startX, int startY) {


    for (int row = 0; row < map.getHeight(); ++row) {
        for (int col = 0; col < map.getWidth(); ++col) {
            sf::RectangleShape tile(sf::Vector2f(tileSize, tileSize));
            tile.setPosition(startX + col * tileSize, startY + row * tileSize);
            // Erstelle den Text für die Zahl
            sf::Text text;
            text.setFont(font);
            text.setString(std::to_string(map.getTile(col, row))); // Zahl aus der Map
            text.setCharacterSize(15); // Textgröße
            text.setFillColor(sf::Color::Black); // Farbe des Texts
            text.setPosition(startX + col * tileSize + tileSize / 3, startY + row * tileSize + tileSize / 4); // Position im Kästchen

            // Zeichne den Text
            window.draw(text);
        }
    }
}

void ResultsWindow::drawMap(const Map& map, int startX, int startY) {
    // Zeichne die Karte (start bei (startX, startY))
    for (int row = 0; row < map.getHeight(); ++row) {
        for (int col = 0; col < map.getWidth(); ++col) {
            sf::RectangleShape tile(sf::Vector2f(tileSize, tileSize));
            tile.setPosition(startX + col * tileSize, startY + row * tileSize);
            tile.setOutlineThickness(1);
            tile.setOutlineColor(sf::Color::Black);

            if (map.getTile(col, row) == 1) {
                tile.setFillColor(sf::Color::Black); // Hindernisse
            }
            else if (map.getTile(col, row) == 2) {
                tile.setFillColor(sf::Color::Blue); // Start
            }
            else if (map.getTile(col, row) == 3) {
                tile.setFillColor(sf::Color::Green); // Ziel
            }
            else {
                tile.setFillColor(sf::Color::White); // Freie Felder
            }
            window.draw(tile);
        }
    }
}

void ResultsWindow::drawText(const std::string& text, int x, int y) {
    sf::Text displayText(text, font, 20);
    displayText.setPosition(x, y);
    displayText.setFillColor(sf::Color::Black);
    window.draw(displayText);
}


void ResultsWindow::onButtonClick() {
    close();
}

void ResultsWindow::handleEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }

        // Mausereignis: Linksklick
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);

            // Prüfe, ob der Button angeklickt wurde
            if (button.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                onButtonClick();
            }
        }

    }
}