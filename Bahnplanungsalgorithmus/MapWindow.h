#ifndef MAPWINDOW_H
#define MAPWINDOW_H

#include <SFML/Graphics.hpp>
#include "Map.h"
#include <iostream>
#include "Algorithm.h"

class MapWindow {
public:
    // Konstruktor
    MapWindow(Map& map, vector<Algorithm>& algorithms);

    // Startet die Hauptschleife des Fensters
    void run();

    void close();

private:

    // Handhabung von Ereignissen (z. B. Maus- oder Tastaturaktionen)
    void handleEvents();

    // Zeichnet die Karte und die Benutzeroberfläche
    void draw();

    // Methode, die bei einem Button-Klick ausgeführt wird
    void onButtonClick();

    // Labels und Checkboxen erstellen
    void initCheckboxes();

    // Zeichne Checkboxen
    void drawCheckboxes();


    void createAlgorithms(int i);

    // Referenz zur Karte
    Map& map;

    int tileSize;
    
    //Vektor für Algortihmen
    vector<Algorithm>& algorithms;

    // SFML-Fenster
    sf::RenderWindow window;

    // Schriftart für den Button-Text
    sf::Font font;

    // Button (Rechteckform)
    sf::RectangleShape button;
   
    // Button-Text
    sf::Text buttonText;

    // Labels für die Algorithmen
    sf::Text alg_label[4] = {};

    // Labels für Programmiersprachen
    sf::Text lang_label[3] = {};

    // Checkboxen für Auswahl
    sf::RectangleShape checkbox[12];  // Array für 12 Checkboxen
    bool checkboxState[12] = {false};           // Zustand der Checkboxen
};


#endif
