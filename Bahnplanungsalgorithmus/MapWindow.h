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

    // Zeichnet die Karte und die Benutzeroberfl�che
    void draw();

    // Methode, die bei einem Button-Klick ausgef�hrt wird
    void onButtonClick();

    // Labels und Checkboxen erstellen
    void initCheckboxes();

    // Zeichne Checkboxen
    void drawCheckboxes();


    void createAlgorithms(int i);

    // Referenz zur Karte
    Map& map;

    int tileSize;
    
    //Vektor f�r Algortihmen
    vector<Algorithm>& algorithms;

    // SFML-Fenster
    sf::RenderWindow window;

    // Schriftart f�r den Button-Text
    sf::Font font;

    // Button (Rechteckform)
    sf::RectangleShape button;
   
    // Button-Text
    sf::Text buttonText;

    // Labels f�r die Algorithmen
    sf::Text alg_label[4] = {};

    // Labels f�r Programmiersprachen
    sf::Text lang_label[3] = {};

    // Checkboxen f�r Auswahl
    sf::RectangleShape checkbox[12];  // Array f�r 12 Checkboxen
    bool checkboxState[12] = {false};           // Zustand der Checkboxen
};


#endif
