#include "Map.h"
#include "MapToCSV.h"
#include "MapWindow.h"
#include "ParseJson.h"
#include "ResultsWindow.h"
#include <fstream>

int main() {
    // Karte erstellen
    Map map(11, 11);
    map.addFrame();
    vector<Algorithm> algorithms;
    vector<ParseJson> parsedResults;
    //ToDo: Rand kann nicht ge�ndert werden (und wird ggf nicht geprinted)
    //DONE: Start/Ziel wird bei erneutem Setzen gel�scht 


    // Fenster mit Karte starten
    MapWindow mapWindow(map, algorithms);
    mapWindow.run();

    //ToDo: andere Prozesse iniziieren
    //ToDo: JSON anderer Prozesse einlesen 

    string filename = "result";
    for (const auto& alg : algorithms) {
        filename += "_" + alg.getName() + "_" + alg.getLanguage() + ".json";  // �bergabe im Format "Name:Sprache"
        ifstream file(filename);
        if (file.is_open()) {
            parsedResults.push_back(ParseJson(filename, alg.getName(), alg.getLanguage()));
        }
        else {
            //Error
        }
    }
    
    ResultsWindow resultsWindow(map, algorithms, parsedResults);
    resultsWindow.run();

    //Window: Zeilen: Map/Pfade, Algorithmus, Pfadl�nge, Python Zeit, C++ Zeit, Java Zeit

    //Pfade von Algorithmen: Algorithmus mit Map und Pfad verkn�pfen? 



    return 0;
}
