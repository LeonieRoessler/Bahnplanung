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

// Bewegungsrichtungen: Oben, Unten, Links, Rechts
const int directions[4][2] = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} };

// Funktion, um den Arbeitsspeicherverbrauch des aktuellen Prozesses zu messen
float getMemoryUsage() {
    PROCESS_MEMORY_COUNTERS_EX pmc;
    HANDLE hProcess = GetCurrentProcess();

    // Holen der Speicherdaten des Prozesses
    if (GetProcessMemoryInfo(hProcess, (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc))) {
        // ru_maxrss entspricht hier der Arbeitsspeicherverbrauch in Bytes
        return pmc.WorkingSetSize / (1024.0f * 1024.0f);  // Umrechnung in MB
    }
    return 0.0f;  // Fehler bei der Abfrage
}

// Funktion zum Speichern der Ergebnisse in eine JSON-Datei
void saveResultsToJson(const vector<vector<int>>& algorithm_map,
    int status_code, int path_length,
    const vector<pair<int, int>>& path,
    double computing_time, float memory_usage,
    const string& result_name) {

    // Erstelle ein JSON-Objekt
    json result_json;

    // Algorithmus Map
    result_json["algorithm_map"] = algorithm_map;

    // Status Code
    result_json["status_code"] = status_code;

    // Pfadlänge (in Tiles)
    result_json["path_length"] = path_length;

    // Pfad
    json path_json = json::array();
    for (const auto& p : path) {
        path_json.push_back({ p.first, p.second });
    }
    result_json["path"] = path_json;

    // Berechnungszeit in Sekunden
    result_json["computing_time"] = computing_time;

    // Speicherverbrauch in MB
    result_json["memory_usage"] = memory_usage;

    // Schreibe das JSON-Objekt in eine Datei
    std::ofstream output_file(result_name);
    if (output_file.is_open()) {
        output_file << std::setw(4) << result_json << std::endl;
        output_file.close();
        std::cout << "Ergebnisse wurden in " << result_name << " gespeichert." << std::endl;
    }
    else {
        std::cerr << "Fehler beim Öffnen der Datei zum Schreiben!" << std::endl;
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

// Wavefront-Algorithmus: Berechnet die minimalen Entfernungen vom Start (2) zum Ziel (3)
pair<int, vector<pair<int, int>>> wavefront(vector<vector<int>>& matrix, int startX, int startY, int goalX, int goalY, vector<vector<int>>& visited) {
    int rows = matrix.size();
    int cols = matrix[0].size();

    // Matrix für Entfernungen (initial mit -1, was "nicht erreichbar" bedeutet)
    vector<vector<int>> distance(rows, vector<int>(cols, -1));

    // BFS-Queue
    queue<pair<int, int>> q;

    // Startpunkt setzen (Startpunkt hat Distanz 0)
    distance[startX][startY] = 0;
    visited[startX][startY] = 0;  // Start ist das erste abgelaufene Feld
    q.push({ startX, startY });

    vector<pair<int, int>> path;  // Um den Pfad zu speichern

    // Breitensuche durchführen
    while (!q.empty()) {
        auto front = q.front();  // Holen des Paars
        int x = front.first;     // x-Koordinate
        int y = front.second;    // y-Koordinate
        q.pop();  // Entfernen des Elements aus der Queue

        // Überprüfen der Nachbarn
        for (const auto& dir : directions) {
            int nx = x + dir[0];
            int ny = y + dir[1];

            // Überprüfen, ob der Nachbar im Gültigkeitsbereich liegt und kein Hindernis ist
            if (nx >= 0 && nx < rows && ny >= 0 && ny < cols && matrix[nx][ny] != 1 && distance[nx][ny] == -1) {
                distance[nx][ny] = distance[x][y] + 1;
                visited[nx][ny] = distance[nx][ny];  // Speichern der abgelaufenen Felder mit Entfernungen
                q.push({ nx, ny });

                // Wenn das Ziel erreicht wurde, speichern wir den Pfad
                if (nx == goalX && ny == goalY) {
                    // Rekonstruktion des Pfades
                    int cx = nx, cy = ny;
                    while (!(cx == startX && cy == startY)) {
                        path.push_back({ cx, cy });
                        for (const auto& dir : directions) {
                            int px = cx + dir[0];
                            int py = cy + dir[1];
                            if (px >= 0 && px < rows && py >= 0 && py < cols && distance[px][py] == distance[cx][cy] - 1) {
                                cx = px;
                                cy = py;
                                break;
                            }
                        }
                    }
                    path.push_back({ startX, startY });
                    reverse(path.begin(), path.end());  // Pfad umkehren, da wir rückwärts rekonstruieren
                    return { distance[goalX][goalY], path };
                }
            }
        }
    }

    return { -1, path };  // Ziel ist nicht erreichbar
}

int main(int argc, char* argv[]) {

    if (argc < 3) {
        cerr << "Fehler: Zu wenige Argumente!" << endl;
        return 1;
    }

    // Die Argumente abrufen
    string input_map = argv[1];
    string result_name = argv[2];

    vector<vector<int>> matrix;
    // CSV in Array lesen
    if (readCsv(matrix, input_map)) {
        cerr << "Fehler beim Einlesen der CSV-Datei!" << endl;
        return 1;
    }

    float memoryBefore = getMemoryUsage();

    // Start- und Zielposition finden
    int startX = -1, startY = -1;
    int goalX = -1, goalY = -1;
    int rows = matrix.size();
    int cols = matrix[0].size();

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (matrix[i][j] == 2) {  // Startpunkt gefunden
                startX = i;
                startY = j;
            }
            if (matrix[i][j] == 3) {  // Zielpunkt gefunden
                goalX = i;
                goalY = j;
            }
        }
    }

    if (startX == -1 || goalX == -1) {
        cout << "Start oder Ziel nicht gefunden!" << endl;
        return 1;
    }

    // Zeitmessung beginnen
    auto startTime = chrono::high_resolution_clock::now();

    // Matrix für besuchte Felder erstellen
    vector<vector<int>> visited(rows, vector<int>(cols, -1));  // -1 bedeutet "nicht besucht"

    // Wavefront-Algorithmus aufrufen
    auto [distanceToGoal, path] = wavefront(matrix, startX, startY, goalX, goalY, visited);

    // Zeitmessung beenden
    auto endTime = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::nanoseconds>(endTime - startTime);

    // Ergebnis ausgeben
    if (distanceToGoal != -1) {
        for (const auto& p : path) {
            cout << "(" << p.first << "," << p.second << ") ";
        }
        cout << endl;
    }
    else {
        cout << "Das Ziel ist vom Start aus nicht erreichbar." << endl;
    }

    // Status Code (0 für erfolgreich, -1 für nicht erreichbar)
    int statusCode = (distanceToGoal != -1) ? 0 : -1;

    float memoryAfter = getMemoryUsage();
    float memoryUsage = memoryAfter - memoryBefore;

    // Speichern der Ergebnisse
    saveResultsToJson(visited, statusCode, distanceToGoal, path, duration.count(), memoryUsage, result_name);

    return 0;
}
