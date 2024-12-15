#ifndef MAPWINDOW_H
#define MAPWINDOW_H

#include <SFML/Graphics.hpp>
#include "Map.h"
#include <iostream>

class MapWindow {
public:
    // Konstruktor
    MapWindow(Map& map);


    // Startet die Hauptschleife des Fensters
    void run();

private:
    // Handhabung von Ereignissen (z. B. Maus- oder Tastaturaktionen)
    void handleEvents();

    // Zeichnet die Karte und die Benutzeroberfläche
    void draw();

    // Methode, die bei einem Button-Klick ausgeführt wird
    void onButtonClick();

    // Referenz zur Karte
    Map& map;

    // SFML-Fenster
    sf::RenderWindow window;

    // Schriftart für den Button-Text
    sf::Font font;

    // Button (Rechteckform)
    sf::RectangleShape button;

    // Button-Text
    sf::Text buttonText;
};

#endif
