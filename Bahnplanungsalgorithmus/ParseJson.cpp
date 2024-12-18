#include "ParseJson.h"


ParseJson::ParseJson(string filename, string algorithm, string language) : filename(filename), algorithm(algorithm), language(language), algorithmMap(0, 0) {
	parse();
}

void ParseJson::parse() {
    // JSON-Datei öffnen
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Fehler beim Öffnen der Datei: " << filename << endl;
        return;
    }

    // JSON-Daten einlesen
    nlohmann::json json_data;
    file >> json_data;

    // Attribute parsen
    try {
        if (json_data.contains("computing_time")) {
            this->computingTime = json_data["computing_time"].get<float>();
        }

        if (json_data.contains("memory_usage")) {
            this->memoryUsage = json_data["memory_usage"].get<float>();
        }

        if (json_data.contains("path_length")) {
            this->pathLength = json_data["path_length"].get<int>();
        }

        if (json_data.contains("algorithm_map")) {
            initializeMap(json_data["algorithm_map"]);
        }

        if (json_data.contains("path")) {
            for (const auto& coord : json_data["path"]) {
                path.emplace_back(coord[0].get<int>(), coord[1].get<int>());
            }
        }

        if (json_data.contains("status_code")) {
            this->statusCode = json_data["status_code"].get<int>();
        }

        if (json_data.contains("language")) {
            this->language = json_data["language"].get<string>();
        }

        if (json_data.contains("algorithm")) {
            this->algorithm = json_data["algorithm"].get<string>();
        }
    }
    catch (const nlohmann::json::exception& e) {
        cerr << "JSON Parsing Error: " << e.what() << endl;
    }
}

void ParseJson::initializeMap(const nlohmann::json& algorithm_map_json) {
    int map_width = algorithm_map_json[0].size();
    int map_height = algorithm_map_json.size();

    // Map erstellen (mit Rahmen: +2 für die Grenzen)
    algorithmMap = Map(map_width, map_height );

    // Algorithm Map-Daten einlesen und in Map setzen
    for (int y = 0; y < map_height; ++y) {
        for (int x = 0; x < map_width; ++x) {
            algorithmMap.setTile(x , y , algorithm_map_json[y][x]); // +1 für Rahmen
        }
    }
    /*
    // Rahmen setzen (optional)
    for (int x = 0; x < algorithmMap.getWidth(); ++x) {
        algorithmMap.setTile(x, 0, 1);                           // Oben
        algorithmMap.setTile(x, algorithmMap.getHeight() - 1, 1); // Unten
    }
    for (int y = 0; y < algorithmMap.getHeight(); ++y) {
        algorithmMap.setTile(0, y, 1);                           // Links
        algorithmMap.setTile(algorithmMap.getWidth() - 1, y, 1); // Rechts
    }*/
}

void ParseJson::displayData() const {
    cout << "File Name: " << filename << endl;
    cout << "Language: " << language << endl;
    cout << "Algorithm: " << algorithm << endl;
    cout << "Status Code: " << statusCode << endl;
    cout << "Path Length: " << pathLength << endl;
    cout << "Computing Time: " << computingTime << " seconds" << endl;
    cout << "Memory Usage: " << memoryUsage << " MB" << endl;

    cout << "Algorithm Map:" << endl;
    for (int y = 0; y < algorithmMap.getHeight(); ++y) {
        for (int x = 0; x < algorithmMap.getWidth(); ++x) {
            cout << algorithmMap.getTile(x, y) << " ";
        }
        cout << endl;
    }

    cout << "Path:" << endl;
    for (const auto& coord : path) {
        cout << "(" << coord.first << ", " << coord.second << ")" << endl;
    }
}

const Map& ParseJson::getAlgorithmMap() const {
    return algorithmMap;
}

std::string ParseJson::getAlgorithm() const {
    return algorithm;
}

std::string ParseJson::getLanguage() const {
    return language;
}

float ParseJson::getComputingTime() const {
    return computingTime;
}

float ParseJson::getMemoryUsage() const {
    return memoryUsage;
}

vector<pair<int, int>> ParseJson::getPath() const {
    return path;
}
