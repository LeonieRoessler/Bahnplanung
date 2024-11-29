#include "Map.h"
#include "MapWindow.h"

int main() {
    // Karte erstellen
    Map map(11, 11);
    map.setTile(5, 5, 1); // Beispiel: Hindernis in der Mitte setzen

    // Fenster mit Karte starten
    MapWindow mapWindow(map, "Dijkstra");
    mapWindow.run();

    return 0;
}
