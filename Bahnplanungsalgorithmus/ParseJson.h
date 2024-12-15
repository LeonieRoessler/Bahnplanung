#include <iostream>
#include "Map.h"
#include <nlohmann/json.hpp>
#include <fstream>

using namespace std;

class ParseJson
{
public:
	string filename ="none";
	string language = "none";
	string algorithm = "none";
	int status_code = 0;

	int path_length = 0;
	float computing_time = 0;
	float memory_usage = 0;
	ParseJson(string filename);

	Map algorithm_map;
	//vector<vector<int>> algorithm_map;      // 2D-Matrix
	vector<pair<int, int>> path;           // Liste von Koordinaten

	void displayData() const;
	// Methode: JSON-Datei parsen und Attribute befüllen
	void parse();

private:
	// Hilfsmethode, um die Map aus JSON-Daten zu initialisieren
	void initializeMap(const nlohmann::json& algorithm_map_json);
	//Konstruktor zum Parsen 
	//Dateiname Angeben
	//Attribute: language, algorithm
	// matrix, path length (optional), time taken
	//Datei mit JSON Format parsen
};

