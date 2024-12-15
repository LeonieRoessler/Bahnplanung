#include "ParseJson.h"


ParseJson::ParseJson(string filename) {
	//Open File
	// JSON-Datei öffnen
	ifstream file(filename);
	if (!file.is_open()) {
		std::cerr << "Fehler beim Öffnen der Datei!" << std::endl;
		return; 
	}

	//JSON parsen
	nlohmann::json jsonData;
	file >> jsonData;


	//Parse Attributes
	timeTaken = jsonData["computing_time"];
	language = jsonData[""];
	algorithm = jsonData[""];
	pathLength = jsonData["path_length"];
	//Path = jsonData["algorithm_map"];

	
	//Parse Map
		// algorithm_map auslesen
	auto algorithm_map = jsonData["algorithm_map"];
	int map_width = algorithm_map[0].size(); // Breite der Map
	int map_height = algorithm_map.size();  // Höhe der Map

	// Map erstellen
	Map pathMap(map_width + 2, map_height + 2); // +2 für Rahmen

	// Daten aus algorithm_map in Map übertragen
	for (int y = 0; y < map_height; ++y) {
		for (int x = 0; x < map_width; ++x) {
			pathMap.setTile(x + 1, y + 1, algorithm_map[y][x]); // +1 wegen Rahmen
		}
	}
	
}

