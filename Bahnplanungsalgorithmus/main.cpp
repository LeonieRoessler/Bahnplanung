#include "Map.h"
#include "MapToCSV.h"
#include "MapWindow.h"
#include "ParseJson.h"
#include "ResultsWindow.h"
#include <fstream>
#include <iostream>
#include <string>

int main() {
    bool uploadMap = false;
    int mapLength = 1;
    string input;
    string filename;
    vector<Algorithm> algorithms;
    vector<ParseJson> parsedResults;
    // Karte erstellen
    cout << "Möchten Sie eine vorhandene Karte verwenden? (ja/nein): ";
    getline(cin, input);
    Map map(1, 1);
    if (input == "ja" || input == "Ja") {
        uploadMap = true;
        cout << "Bitte Dateiname angeben:" << std::endl;
        cin >> filename;
        try {
            map= map.loadFromCSV(filename);
        }
        catch (const std::exception& e) {
            std::cerr << "Fehler beim Laden der Karte: " << e.what() << std::endl;
            return 1;
        }
    }
    else {
        cout << "Bitte geben Sie die Kartenbreite an: ";
        cin >> mapLength;
        map = Map(mapLength, mapLength);
        map.addFrame();
    }


    // Fenster mit Karte starten
    MapWindow mapWindow(map, algorithms);
    mapWindow.run();


    filename = "result";
    for (const auto& alg : algorithms) {
        filename += "_" + alg.getName() + "_" + alg.getLanguage() + ".json";  // Übergabe im Format "Name:Sprache"
        ifstream file(filename);
        if (file.is_open()) {
            parsedResults.push_back(ParseJson(filename, alg.getName(), alg.getLanguage()));
        }
        else {
            //Error
        }
        filename = "result";
    }
    
    ResultsWindow resultsWindow(map, algorithms, parsedResults);
    resultsWindow.run();



    return 0;
}
