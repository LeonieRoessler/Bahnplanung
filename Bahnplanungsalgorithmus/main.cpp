#include "Map.h"
#include "MapToCSV.h"
#include "MapWindow.h"

int main() {
    // Karte erstellen
    Map map(11, 11);

    //ToDo: Rand kann nicht ge�ndert werden (und wird ggf nicht geprinted)
    //DONE: Start/Ziel wird bei erneutem Setzen gel�scht 


    // Fenster mit Karte starten
    MapWindow mapWindow(map);
    mapWindow.run();

    //ToDo: andere Prozesse iniziieren
    //ToDo: JSON anderer Prozesse einlesen 


    //Window: Zeilen: Map/Pfade, Algorithmus, Pfadl�nge, Python Zeit, C++ Zeit, Java Zeit

    //Pfade von Algorithmen: Algorithmus mit Map und Pfad verkn�pfen? 



    return 0;
}
