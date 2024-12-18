#include "MapWindow.h"
#include "MapToCSV.h"
#include <iostream>


// Konstruktor
MapWindow::MapWindow(Map& map, vector<Algorithm>& algorithms) : map(map), button(sf::Vector2f(100, 30)), buttonText("Action", font, 20), algorithms(algorithms) {

    tileSize = 20;
    // Fenster initialisieren
    window.create(sf::VideoMode(map.getWidth() * tileSize + tileSize * 20, map.getHeight() * tileSize +100), "Map Editor");

    // Schriftart laden
    if (!font.loadFromFile("assets/arial.ttf")) {
        std::cerr << "Fehler beim Laden der Schriftart!" << std::endl;
    }

    //Initialisieren Checkboxen + Labels
    initCheckboxes();

    // Button initialisieren
    button.setFillColor(sf::Color::Blue);
    button.setPosition(map.getWidth()* tileSize + 10, map.getHeight() * tileSize);
    button.setOutlineThickness(2);
    button.setOutlineColor(sf::Color::White);

    // Text für den Button
    buttonText.setFillColor(sf::Color::White);
    buttonText.setPosition(map.getWidth() * tileSize + 20, map.getHeight() * tileSize); // Zentriert auf dem Button
}

// Läuft die Hauptschleife für das Fenster
void MapWindow::run() {
    while (window.isOpen()) {
        handleEvents();
        draw();
    }
}

void MapWindow::close() {
    while (window.isOpen()) {
        window.close();
    }
}


// Zeichnet die Map und die UI
void MapWindow::draw() {
    window.clear(sf::Color(169, 169, 169));

    // Zeichne die Karte
    for (int y = 0; y < map.getHeight(); ++y) {
        for (int x = 0; x < map.getWidth(); ++x) {
            sf::RectangleShape tile(sf::Vector2f(tileSize, tileSize));
            tile.setPosition(x * tileSize, y * tileSize);
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

    // Zeichne Checkboxen
    drawCheckboxes();

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
            if (mousePos.y < map.getHeight() * tileSize) {
                int x = mousePos.x / tileSize;
                int y = mousePos.y / tileSize;

                // Kachel umschalten (0 zu 1 oder 1 zu 0)
                int currentValue = map.getTile(x, y);
                map.setTile(x, y, currentValue == 0 ? 1 : 0);
            }

            // Prüfe, ob der Button angeklickt wurde
            if (button.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                onButtonClick();
            }

            // Prüfe, ob eine Checkbox angeklickt wurde
            for (int i = 0; i < 12; ++i) {
                if (checkbox[i].getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    checkboxState[i] = !checkboxState[i];
                    checkbox[i].setFillColor(checkboxState[i] ? sf::Color::Green : sf::Color::White);
                }
            }
        }
        // Mausereignis: Rechtsklick
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);

            // Prüfe, ob ein Kartenfeld angeklickt wurde
            if (mousePos.y < map.getHeight() * tileSize) {
                int x = mousePos.x / tileSize;
                int y = mousePos.y / tileSize;

                // Wenn auf ein existierendes Startfeld geklickt wird, zurücksetzen
                if (map.getTile(x, y) == 2) {
                    map.setTile(x, y, 0);
                }
                else {
                    // Vorherige Startfelder zurücksetzen
                    for (int row = 0; row < map.getHeight(); ++row) {
                        for (int col = 0; col < map.getWidth(); ++col) {
                            if (map.getTile(col, row) == 2) {
                                map.setTile(col, row, 0);
                            }
                        }
                    }
                    // Neues Startfeld setzen
                    map.setTile(x, y, 2);
                }
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
            if (mousePos.y < map.getHeight() * tileSize) {
                int x = mousePos.x / tileSize;
                int y = mousePos.y / tileSize;

                if (map.getTile(x, y) == 3) {
                    map.setTile(x, y, 0);
                }
                else {
                    // Vorherige Zielfelder zurücksetzen
                    for (int row = 0; row < map.getHeight(); ++row) {
                        for (int col = 0; col < map.getWidth(); ++col) {
                            if (map.getTile(col, row) == 3) {
                                map.setTile(col, row, 0);
                            }
                        }
                    }
                    // Neues Zielfeld setzen
                    map.setTile(x, y, 3);
                }
            }

            // Prüfe, ob der Button angeklickt wurde
            if (button.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                onButtonClick();
            }
        }



    }
}

void MapWindow::createAlgorithms(int i) {
    string name;
    string language;
    if (i / 3 == 0) {
        name = "Wavefront";
    }
    else if (i / 3 == 1) {
        name = "Brushfire";
    }
    else if (i / 3 == 2) {
        name = "AStarAP";
    }
    else if (i / 3 == 3) {
        name = "AStarMH";
    }
    if (i % 3 == 0) {
        language = "Java";
    }
    else if (i % 3 == 1) {
        language = "Python";
    }
    else if (i % 3 == 2) {
        language = "Cpp";
    }
    algorithms.push_back(Algorithm(name, language));
}

void MapWindow::onButtonClick() {
    std::cout << "Button wurde gedrückt! Aktion ausführen..." << std::endl;

    // Karte exportieren
    try {
        MapToCSV exporter(map);
        exporter.exportToFile("map.csv");
        std::cout << "Map erfolgreich als map.csv exportiert!" << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Fehler beim Exportieren der Karte: " << e.what() << std::endl;
        return;
    }

    //check checkbox State
    for (int i = 0; i < 12; i++) {
        if (checkboxState[i] == true) {
            createAlgorithms(i);
        }
    }

    // Python-Skript starten
    std::cout << "Starte das Python-Skript für Bahnplanung..." << std::endl;
    string command = "python3 run_algorithms.py ";

    // Füge alle Algorithmen zur Kommandozeile hinzu
    for (const auto& alg : algorithms) {
        command += "\"" + alg.getName() + ":" + alg.getLanguage() + "\" ";  // Übergabe im Format "Name:Sprache"
    }

    // Übergibt den Befehl an das System, um das Python-Skript auszuführen
    std::cout << command.c_str() << std::endl;
    int scriptStatus = system(command.c_str());
    if (scriptStatus == 0) {
        std::cout << "Python-Skript erfolgreich ausgeführt." << std::endl;
    }
    else {
        std::cerr << "Fehler beim Ausfuehren des Python-Skripts." << std::endl;
    }
    close();
}

// Zeichnet die Checkboxen
void MapWindow::drawCheckboxes() {
    for (int i = 0; i < 12; ++i) {
        window.draw(checkbox[i]);
    }
    for (int i = 0; i < 3; ++i) {
        window.draw(lang_label[i]);
    }
    for (int i = 0; i < 4; ++i) {
        window.draw(alg_label[i]);
    }
}

void MapWindow::initCheckboxes() {
    //Labels erstellen
    int xStart = map.getWidth() * tileSize + 50;

    for (int i = 0; i < 4; i++) {
        alg_label[i].setFont(font);
        alg_label[i].setCharacterSize(18);
        alg_label[i].setFillColor(sf::Color::Black);
        alg_label[i].setPosition(xStart, 95 + i * 40);
    }
    for (int i = 0; i < 3; i++) {
        lang_label[i].setFont(font);
        lang_label[i].setCharacterSize(18);
        lang_label[i].setFillColor(sf::Color::Black);
        lang_label[i].setPosition(xStart + 30 + i * 60, 70);
    }


    alg_label[0].setString("wf");
    alg_label[1].setString("bf");
    alg_label[2].setString("a* ap");
    alg_label[3].setString("a* mh");
    lang_label[0].setString("Java");
    lang_label[1].setString("Pyth");
    lang_label[2].setString("Cpp");

    // Checkboxen erstellen
    for (int i = 0; i < 12; ++i) {
        checkbox[i].setSize(sf::Vector2f(20, 20));
        checkbox[i].setFillColor(sf::Color::White);
        checkbox[i].setOutlineThickness(2);
        checkbox[i].setOutlineColor(sf::Color::Black);
        checkboxState[i] = false;
    }
    // Position der Checkboxen
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 3; ++j) {
            checkbox[i * 3 + j].setPosition(xStart + 40 + j * 60, 100 + i * 40);  // 3x3 Raster
        }
    }


}