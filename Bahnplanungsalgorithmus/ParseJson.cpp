#include "ParseJson.h"

ParseJson::ParseJson(string filename) : filename(filename), algorithm_map(0, 0) {
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
            computing_time = json_data["computing_time"].get<float>();
        }

        if (json_data.contains("memory_usage")) {
            memory_usage = json_data["memory_usage"].get<float>();
        }

        if (json_data.contains("path_length")) {
            path_length = json_data["path_length"].get<int>();
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
            status_code = json_data["status_code"].get<int>();
        }

        if (json_data.contains("language")) {
            language = json_data["language"].get<string>();
        }

        if (json_data.contains("algorithm")) {
            algorithm = json_data["algorithm"].get<string>();
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
    algorithm_map = Map(map_width + 2, map_height + 2);

    // Algorithm Map-Daten einlesen und in Map setzen
    for (int y = 0; y < map_height; ++y) {
        for (int x = 0; x < map_width; ++x) {
            algorithm_map.setTile(x + 1, y + 1, algorithm_map_json[y][x]); // +1 für Rahmen
        }
    }

    // Rahmen setzen (optional)
    for (int x = 0; x < algorithm_map.getWidth(); ++x) {
        algorithm_map.setTile(x, 0, 1);                           // Oben
        algorithm_map.setTile(x, algorithm_map.getHeight() - 1, 1); // Unten
    }
    for (int y = 0; y < algorithm_map.getHeight(); ++y) {
        algorithm_map.setTile(0, y, 1);                           // Links
        algorithm_map.setTile(algorithm_map.getWidth() - 1, y, 1); // Rechts
    }
}

void ParseJson::displayData() const {
    cout << "File Name: " << filename << endl;
    cout << "Language: " << language << endl;
    cout << "Algorithm: " << algorithm << endl;
    cout << "Status Code: " << status_code << endl;
    cout << "Path Length: " << path_length << endl;
    cout << "Computing Time: " << computing_time << " seconds" << endl;
    cout << "Memory Usage: " << memory_usage << " MB" << endl;

    cout << "Algorithm Map:" << endl;
    for (int y = 0; y < algorithm_map.getHeight(); ++y) {
        for (int x = 0; x < algorithm_map.getWidth(); ++x) {
            cout << algorithm_map.getTile(x, y) << " ";
        }
        cout << endl;
    }

    cout << "Path:" << endl;
    for (const auto& coord : path) {
        cout << "(" << coord.first << ", " << coord.second << ")" << endl;
    }
}

