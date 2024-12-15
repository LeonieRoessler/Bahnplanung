#pragma once
#include <iostream>
#include "Map.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <vector>

using namespace std;

class ParseJson
{
public:
	ParseJson(string filename, string algorithm, string language);
	string language = "none";
	string algorithm = "none";
	string filename = "none";
	int statusCode = 0;

	int pathLength = 0;
	float computingTime = 0;
	float memoryUsage = 0;

	const Map& getAlgorithmMap() const;

	string getAlgorithm() const;

	string getLanguage() const;

	vector<pair<int, int>> getPath() const;

	float getComputingTime() const;

	float getMemoryUsage() const;

	Map algorithmMap;

	//vector<vector<int>> algorithm_map;      // 2D-Matrix
	vector<pair<int, int>> path;   // Liste von Koordinaten

	void displayData() const;
	// Methode: JSON-Datei parsen und Attribute befüllen
	void parse();

private:
	// Hilfsmethode, um die Map aus JSON-Daten zu initialisieren
	void initializeMap(const nlohmann::json& algorithmMapJson);
	//Konstruktor zum Parsen 
	//Dateiname Angeben
	//Attribute: language, algorithm
	// matrix, path length (optional), time taken
	//Datei mit JSON Format parsen
};

