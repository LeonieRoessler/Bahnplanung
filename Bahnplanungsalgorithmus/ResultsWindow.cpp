#include "ResultsWindow.h"

ResultsWindow::ResultsWindow(Map& originalMap, const std::vector<Algorithm>& algorithms, vector<ParseJson>& parsedResults)
    : map(originalMap), algorithms(algorithms), parsedResults(parsedResults), button(sf::Vector2f(100, 30)), buttonText("Close", font, 20) {
    // Fenster initialisieren
    window.create(sf::VideoMode(2000, 1000), "Results");

    // Schriftart laden
    if (!font.loadFromFile("assets/arial.ttf")) {
        std::cerr << "Fehler beim Laden der Schriftart!" << std::endl;
    }

    // Maps initialisieren
    /*for (auto& result : parsedResults) {
        groupedResults[result.algorithm] = &result;
        resultsByLanguage[result.algorithm][result.language] = &result;
    }*/

    for (const auto& result : parsedResults) {
        groupedResults[result.getAlgorithm()].push_back(&result);
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

    int xOffset = 500; // Startposition für die Darstellung der Karten und Ergebnisse

    drawMap(map, 0, 50);

   /* for (const auto& parsedResult : parsedResults) {
        // Algorithmus-Name und Sprache
        string algorithmName = parsedResult.getAlgorithm();
        string language = parsedResult.getLanguage();

        // Algorithmus-Map zeichnen
        drawPath(map, parsedResult.getPath(), xOffset, 50);

        //drawNumbers(map, parsedResult.getAlgorithmMap(), xOffset, 50);

        // Algorithmus-Name und Sprache anzeigen
        drawText("Algorithm: " + algorithmName + ", Language: " + language, xOffset, 20);

        // Ergebnisse anzeigen (z. B. Rechenzeit, Speicherverbrauch)
        std::string info = "Computing Time: " + std::to_string(parsedResult.getComputingTime()) + "ms, " +
            "Memory Usage: " + std::to_string(parsedResult.getMemoryUsage()) + "MB";
        drawText(info, xOffset, 100 + parsedResult.getAlgorithmMap().getHeight() * 50 + 10);

        // Versatz für die nächste Darstellung anpassen
        xOffset += parsedResult.getAlgorithmMap().getWidth() * 50 + 70;
    }*/




    for (const auto& entry : groupedResults) {
        string algorithmName = entry.first;
        int mapWidth = 0;
        drawText("Algorithm: " + algorithmName, xOffset, 20);
       
        for (const auto& result : entry.second) {
            int yOffset = 10;
            string language = result->getLanguage();
            float computingTime = result->getComputingTime();
            float memoryUsage = result->getMemoryUsage();
            drawPath(map, result->getPath(), xOffset, 50);
            //drawNumbers(map, parsedResult.getAlgorithmMap(), xOffset, 50);
            mapWidth = result->getAlgorithmMap().getWidth();
            string info = language + ":   Computing Time: " + std::to_string(computingTime) + "ms, " + "Memory Usage: " + std::to_string(memoryUsage) + "MB";
            drawText(info, xOffset, 100 + result->getAlgorithmMap().getHeight() * 50 + yOffset);
            yOffset+=30;
        }
        xOffset += mapWidth * 50 ;
    }


    /*
    for (const auto& parsedResult : parsedResults) {
        // Algorithmus-Name und Sprache
        string algorithmName = parsedResult.getAlgorithm();
        string language = parsedResult.getLanguage();

        for (auto& result : parsedResults) {
            if (result.getAlgorithm() == "Bushfire") {
                language = result.getLanguage();
                algorithmName = result.getAlgorithm();

                drawText("Algorithm: " + algorithmName + ", Language: " + language, xOffset, 20);
                // Ergebnisse anzeigen (z. B. Rechenzeit, Speicherverbrauch)
                std::string info = "Language: " + language + " Computing Time: " + std::to_string(parsedResult.getComputingTime()) + "ms, " +
                    "Memory Usage: " + std::to_string(parsedResult.getMemoryUsage()) + "MB";
                drawText(info, xOffset, 100 + parsedResult.getAlgorithmMap().getHeight() * 50 + 10);
            }
        }


        
        // Algorithmus-Map zeichnen
        //drawPath(map, parsedResult.getPath(), xOffset, 50);

        //drawNumbers(map, parsedResult.getAlgorithmMap(), xOffset, 50);

        // Algorithmus-Name und Sprache anzeigen
        //drawText("Algorithm: " + algorithmName + ", Language: " + language, xOffset, 20);



        // Versatz für die nächste Darstellung anpassen
        //xOffset += parsedResult.getAlgorithmMap().getWidth() * 50 + 70;
    }*/



    // Button zeichnen
    window.draw(button);
    window.draw(buttonText);

    window.display();
}

void ResultsWindow::drawPath(const Map& map, const vector<pair<int, int>>& path, int x, int y) {
    //Original Map 
    for (int row = 0; row < map.getHeight(); ++row) {
        for (int col = 0; col < map.getWidth(); ++col) {
            sf::RectangleShape tile(sf::Vector2f(50, 50));
            tile.setPosition(x + col * 50, y + row * 50);
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

}

void ResultsWindow::drawMap(const Map& map, int startX, int startY) {
    // Zeichne die Karte (start bei (startX, startY))
    for (int row = 0; row < map.getHeight(); ++row) {
        for (int col = 0; col < map.getWidth(); ++col) {
            sf::RectangleShape tile(sf::Vector2f(50, 50));
            tile.setPosition(startX + col * 50, startY + row * 50);
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


}