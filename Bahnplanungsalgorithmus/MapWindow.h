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

    // Zeichnet die Karte und die Benutzeroberfl�che
    void draw();

    // Methode, die bei einem Button-Klick ausgef�hrt wird
    void onButtonClick();

    // Referenz zur Karte
    Map& map;

    // SFML-Fenster
    sf::RenderWindow window;

    // Schriftart f�r den Button-Text
    sf::Font font;

    // Button (Rechteckform)
    sf::RectangleShape button;

    // Button-Text
    sf::Text buttonText;
};

#endif
