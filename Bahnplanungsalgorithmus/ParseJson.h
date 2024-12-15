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
	int pathLength = 0;
	float timeTaken = 0;
	ParseJson(string filename);

	//Konstruktor zum Parsen 
	//Dateiname Angeben
	//Attribute: language, algorithm
	// matrix, path length (optional), time taken
	//Datei mit JSON Format parsen
};

