#pragma once

#include <SFML/Graphics.hpp>
#include "Map.h"
#include <iostream>
#include "Algorithm.h"
#include "ParseJson.h"

class ResultsWindow
{
public:
    // Konstruktor
    ResultsWindow(Map& originalMap, const std::vector<Algorithm>& algorithms, vector<ParseJson>& parsedResults);

    // Startet die Hauptschleife des Fensters
    void run();

private:

    // Handhabung von Ereignissen (z. B. Maus- oder Tastaturaktionen)
    void handleEvents();

    // Zeichnet die Karte und die Benutzeroberfläche
    void draw();

    void close();

    void drawMap(const Map& map, int x, int y);

    //Algorithmen Pfade einzeichnen
    void drawPath(const Map& map, const vector<pair<int, int>>& path, int x, int y);

    //Algorithmen Nummern schreiben
    void drawNumbers(const Map& map, int startX, int startY);


    // Methode, die bei einem Button-Klick ausgeführt wird
    void onButtonClick();

    void drawText(const std::string& text, int x, int y);

    // Referenz zur Karte
    Map& map;

    //Vektor für Algortihmen
    vector<Algorithm> algorithms;
    vector<ParseJson> parsedResults;
    // Neue Datenstrukturen
    std::map<std::string, std::vector<const ParseJson*>> groupedResults;// Gruppiert nach Algorithmusname
    std::map<std::string, std::map<std::string, const ParseJson*>> resultsByLanguage; // Nach Sprache gruppiert


    // SFML-Fenster
    sf::RenderWindow window;

    // Schriftart für den Button-Text
    sf::Font font;

    // Button (Rechteckform)
    sf::RectangleShape button;

    // Button-Text
    sf::Text buttonText;

    // Labels für die Algorithmen
    sf::Text alg_label[3] = {};

    // Labels für Programmiersprachen
    sf::Text lang_label[3] = {};



};

