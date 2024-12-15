#include "Map.h"
#include "MapToCSV.h"
#include "MapWindow.h"

int main() {
    // Karte erstellen
    Map map(11, 11);

    //ToDo: Rand kann nicht geändert werden (und wird ggf nicht geprinted)
    //DONE: Start/Ziel wird bei erneutem Setzen gelöscht 


    // Fenster mit Karte starten
    MapWindow mapWindow(map);
    mapWindow.run();

    //ToDo: andere Prozesse iniziieren
    //ToDo: JSON anderer Prozesse einlesen 


    //Window: Zeilen: Map/Pfade, Algorithmus, Pfadlänge, Python Zeit, C++ Zeit, Java Zeit

    //Pfade von Algorithmen: Algorithmus mit Map und Pfad verknüpfen? 



    return 0;
}
