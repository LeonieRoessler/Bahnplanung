#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <queue>
#include <chrono>
#include <iomanip>
#include <algorithm>
#include <nlohmann/json.hpp>
#include <windows.h>
#include <psapi.h>

using namespace std;
using json = nlohmann::json;

// Funktion, um den Arbeitsspeicherverbrauch des aktuellen Prozesses zu messen
float getMemoryUsage() {
    PROCESS_MEMORY_COUNTERS_EX pmc;
    HANDLE hProcess = GetCurrentProcess();
    if (GetProcessMemoryInfo(hProcess, (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc))) {
        return pmc.WorkingSetSize / (1024.0f * 1024.0f);  // Umrechnung in MB
    }
    return 0.0f;
}

// Funktion zum Speichern der Ergebnisse in eine JSON-Datei
void saveResultsToJson(const vector<vector<int>>& algorithm_map,
    int status_code, double computing_time, float memory_usage, const string& result_name) {

    json result_json;
    result_json["algorithm_map"] = algorithm_map;
    result_json["status_code"] = status_code;
    result_json["computing_time"] = computing_time;
    result_json["memory_usage"] = memory_usage;

    ofstream output_file(result_name);
    if (output_file.is_open()) {
        output_file << setw(4) << result_json << endl;
        output_file.close();
        cout << "Ergebnisse wurden in " << result_name << " gespeichert." << endl;
    }
    else {
        cerr << "Fehler beim Öffnen der Datei zum Schreiben!" << endl;
    }
}

// Funktion zum Einlesen der CSV-Datei in eine Matrix
int readCsv(vector<vector<int>>& matrix, const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Fehler: Datei konnte nicht geöffnet werden!" << endl;
        return 1;
    }
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        vector<int> row;
        string cell;
        while (getline(ss, cell, ',')) {
            row.push_back(stoi(cell));
        }
        matrix.push_back(row);
    }
    return 0;
}


void brushfire(vector<vector<int>>& matrix, vector<vector<int>>& visited) {
    int rows = matrix.size();
    int cols = matrix[0].size();
    queue<pair<int, int>> q;

    // Alle Hindernisse (matrix[i][j] == 1) als Startpunkte hinzufügen
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (matrix[i][j] == 1) {
                visited[i][j] = 0; // Hindernisse haben Distanz 0
                q.push({ i, j });
            }
            else {
                visited[i][j] = -1; // Nicht besuchte Felder setzen auf -1
            }
        }
    }
    // Richtung für Nachbarn (oben, unten, links, rechts)
    vector<pair<int, int>> directions = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} };

    while (!q.empty()) {
        auto [x, y] = q.front();
        q.pop();

        for (const auto& dir : directions) {
            int nx = x + dir.first;
            int ny = y + dir.second;

            // Bedingungen prüfen:
            // 1. Innerhalb der Grenzen
            // 2. Kein Hindernis
            // 3. Noch nicht besucht (visited[nx][ny] == -1)
            if (nx >= 0 && nx < rows && ny >= 0 && ny < cols && matrix[nx][ny] != 1 && visited[nx][ny] == -1) {
                visited[nx][ny] = visited[x][y] + 1; // Entfernungswert setzen
                q.push({ nx, ny }); // Nachbarn in die Queue einfügen
            }
        }
    }
}

int main(int argc, char* argv[]) {
    int statusCode;
    if (argc < 3) {
        cerr << "Fehler: Zu wenige Argumente!" << endl;
        return 1;
    }

    string input_map = argv[1];
    string result_name = argv[2];

    vector<vector<int>> matrix;
    if (readCsv(matrix, input_map)) {
        cerr << "Fehler beim Einlesen der CSV-Datei!" << endl;
        statusCode = 500;
        return statusCode;
    }

    float memoryBefore = getMemoryUsage();

    int rows = matrix.size();
    int cols = matrix[0].size();


    auto startTime = chrono::high_resolution_clock::now();

    vector<vector<int>> visited(rows, vector<int>(cols, -1));  // -1 bedeutet "nicht besucht"
    brushfire(matrix, visited);

    auto endTime = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::nanoseconds>(endTime - startTime);

    cout << "Brushfire-Algorithmus abgeschlossen." << endl;
    cout << "Berechnungszeit: " << duration.count() << " ms" << endl;

    float memoryAfter = getMemoryUsage();
    float memoryUsage = memoryAfter - memoryBefore;
    double timeInMs = duration.count() / 1000000.0;

    saveResultsToJson(visited, 0, timeInMs, memoryUsage, result_name);

    return 0;
}
