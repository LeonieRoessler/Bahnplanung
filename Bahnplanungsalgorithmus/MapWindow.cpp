#include "MapWindow.h"
#include "MapToCSV.h"
#include <iostream>

// Konstruktor
MapWindow::MapWindow(Map& map) : map(map), button(sf::Vector2f(100, 30)), buttonText("Action", font, 20) {

    // Fenster initialisieren
    window.create(sf::VideoMode(600, 700), "Map Editor");

    // Schriftart laden
    if (!font.loadFromFile("assets/arial.ttf")) {
        std::cerr << "Fehler beim Laden der Schriftart!" << std::endl;
    }

    // Button initialisieren
    button.setFillColor(sf::Color::Blue);
    button.setPosition(250, 630);
    button.setOutlineThickness(2);
    button.setOutlineColor(sf::Color::White);

    // Text für den Button
    buttonText.setFillColor(sf::Color::White);
    buttonText.setPosition(265, 635); // Zentriert auf dem Button
}

// Läuft die Hauptschleife für das Fenster
void MapWindow::run() {
    while (window.isOpen()) {
        handleEvents();
        draw();
    }
}

// Zeichnet die Map und die UI
void MapWindow::draw() {
    window.clear();

    // Zeichne die Karte
    for (int y = 0; y < map.getHeight(); ++y) {
        for (int x = 0; x < map.getWidth(); ++x) {
            sf::RectangleShape tile(sf::Vector2f(50, 50));
            tile.setPosition(x * 50, y * 50);
            tile.setOutlineThickness(1);
            tile.setOutlineColor(sf::Color::Black);

            if (map.getTile(x, y) == 1) {
                tile.setFillColor(sf::Color::Black); // Hindernisse
            }
            else if (map.getTile(x, y) == 2) {
                tile.setFillColor(sf::Color::Blue); // Start
            }
            else if (map.getTile(x, y) == 3) {
                tile.setFillColor(sf::Color::Green); // Ziel
            }
            else {
                tile.setFillColor(sf::Color::White); // Freie Felder
            }
            window.draw(tile);
        }
    }

    // Zeichne den Button
    window.draw(button);
    window.draw(buttonText);

    window.display();
}

// Behandelt Ereignisse wie Maus- oder Tastatureingaben
void MapWindow::handleEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }

        // Mausereignis: Linksklick
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);

            // Prüfe, ob ein Kartenfeld angeklickt wurde
            if (mousePos.y < map.getHeight() * 50) {
                int x = mousePos.x / 50;
                int y = mousePos.y / 50;

                // Kachel umschalten (0 zu 1 oder 1 zu 0)
                int currentValue = map.getTile(x, y);
                map.setTile(x, y, currentValue == 0 ? 1 : 0);
            }

            // Prüfe, ob der Button angeklickt wurde
            if (button.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                onButtonClick();
            }
        }
        // Mausereignis: Rechtsklick
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);

            // Prüfe, ob ein Kartenfeld angeklickt wurde
            if (mousePos.y < map.getHeight() * 50) {
                int x = mousePos.x / 50;
                int y = mousePos.y / 50;

                // Kachel umschalten (0 zu 1 oder 1 zu 0)
                int currentValue = map.getTile(x, y);
                map.setTile(x, y, currentValue == 0 ? 2 : 0);
            }

            // Prüfe, ob der Button angeklickt wurde
            if (button.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                onButtonClick();
            }
        }

        // Mausereignis: Mittlere Maustaste
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Middle) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);

            // Prüfe, ob ein Kartenfeld angeklickt wurde
            if (mousePos.y < map.getHeight() * 50) {
                int x = mousePos.x / 50;
                int y = mousePos.y / 50;

                // Kachel umschalten (0 zu 1 oder 1 zu 0)
                int currentValue = map.getTile(x, y);
                map.setTile(x, y, currentValue == 0 ? 3 : 0);
            }

            // Prüfe, ob der Button angeklickt wurde
            if (button.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                onButtonClick();
            }
        }



    }
}

// Aktion für den Button
void MapWindow::onButtonClick() {
    std::cout << "Button wurde gedrückt! Aktion ausführen..." << std::endl;
    //Karte exportieren
    try {
        MapToCSV exporter(map);
        exporter.exportToFile("map.csv");
        std::cout << "Map erfolgreich als map.csv exportiert!" << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Fehler: " << e.what() << std::endl;
    }
}