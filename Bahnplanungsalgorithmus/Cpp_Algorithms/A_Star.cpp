#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <queue>
#include <chrono>
#include <iomanip>
#include <algorithm>
#include <cmath>
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

    if (GetProcessMemoryInfo(hProcess, (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc))) {
        return pmc.WorkingSetSize / (1024.0f * 1024.0f);
    }
    return 0.0f;
}

// Funktion zum Speichern der Ergebnisse in eine JSON-Datei
void saveResultsToJson(const vector<vector<int>>& algorithm_map, int status_code, int path_length,
    const vector<pair<int, int>>& path, double computing_time, float memory_usage,
    const string& result_name) {

    json result_json;
    result_json["algorithm_map"] = algorithm_map;
    result_json["status_code"] = status_code;
    result_json["path_length"] = path_length;

    json path_json = json::array();
    for (const auto& p : path) {
        path_json.push_back({ p.first, p.second });
    }
    result_json["path"] = path_json;

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

// A*-Algorithmus
pair<int, vector<pair<int, int>>> aStar(vector<vector<int>>& matrix, int startX, int startY, int goalX, int goalY,
    vector<vector<int>>& visited, const string& heuristic) {
    int rows = matrix.size();
    int cols = matrix[0].size();

    auto calculateHeuristic = [&](int x, int y) {
        if (heuristic == "manhattan") {
            return abs(x - goalX) + abs(y - goalY);
        }
        else if (heuristic == "airplane") {
            int ret = sqrt(pow(x - goalX, 2) + pow(y - goalY, 2));
            return ret;
        }
        return 0;
        };

    vector<vector<int>> gScore(rows, vector<int>(cols, INT_MAX));
    vector<vector<int>> fScore(rows, vector<int>(cols, INT_MAX));

    auto compare = [&](const pair<int, int>& a, const pair<int, int>& b) {
        return fScore[a.first][a.second] > fScore[b.first][b.second];
        };

    priority_queue<pair<int, int>, vector<pair<int, int>>, decltype(compare)> openSet(compare);

    gScore[startX][startY] = 0;
    fScore[startX][startY] = calculateHeuristic(startX, startY);
    openSet.push({ startX, startY });

    vector<pair<int, int>> path;

    while (!openSet.empty()) {
        auto current = openSet.top();
        openSet.pop();

        int x = current.first, y = current.second;

        if (x == goalX && y == goalY) {
            while (!(x == startX && y == startY)) {
                path.push_back({ x, y });
                for (const auto& dir : directions) {
                    int nx = x - dir[0], ny = y - dir[1];
                    if (nx >= 0 && nx < rows && ny >= 0 && ny < cols &&
                        gScore[nx][ny] + 1 == gScore[x][y]) {
                        x = nx;
                        y = ny;
                        break;
                    }
                }
            }
            path.push_back({ startX, startY });
            reverse(path.begin(), path.end());
            return { gScore[goalX][goalY], path };
        }

        for (const auto& dir : directions) {
            int nx = x + dir[0], ny = y + dir[1];

            if (nx >= 0 && nx < rows && ny >= 0 && ny < cols && matrix[nx][ny] != 1) {
                int tentative_gScore = gScore[x][y] + 1;

                if (tentative_gScore < gScore[nx][ny]) {
                    gScore[nx][ny] = tentative_gScore;
                    fScore[nx][ny] = gScore[nx][ny] + calculateHeuristic(nx, ny);
                    visited[nx][ny] = tentative_gScore;
                    openSet.push({ nx, ny });
                }
            }
        }
    }

    return { -1, path };
}

int main(int argc, char* argv[]) {
    int statusCode;
    if (argc < 4) {
        cerr << "Fehler: Zu wenige Argumente! Erwartet: <CSV-Datei> <JSON-Datei> <Heuristik>" << endl;
        return 1;
    }

    string input_map = argv[1];
    string result_name = argv[2];
    string heuristic = argv[3];

    if (heuristic != "manhattan" && heuristic != "airplane") {
        cerr << "Fehler: Ungültige Heuristik. Erlaubt sind 'manhattan' und 'airplane'." << endl;
        statusCode = 401;
        return statusCode;
    }

    vector<vector<int>> matrix;
    if (readCsv(matrix, input_map)) {
        cerr << "Fehler beim Einlesen der CSV-Datei!" << endl;
        statusCode = 500;
        return statusCode;
    }

    float memoryBefore = getMemoryUsage();

    int startX = -1, startY = -1;
    int goalX = -1, goalY = -1;

    for (int i = 0; i < matrix.size(); i++) {
        for (int j = 0; j < matrix[0].size(); j++) {
            if (matrix[i][j] == 2) { startX = i; startY = j; }
            if (matrix[i][j] == 3) { goalX = i; goalY = j; }
        }
    }

    if (startX == -1) {
        cout << "Start nicht gefunden!" << endl;
        statusCode = 402;
        return 402;
    }
    if (goalX == -1) {
        cout << "Ziel nicht gefunden!" << endl;
        statusCode = 403;
        return 403;
    }

    auto startTime = chrono::high_resolution_clock::now();

    vector<vector<int>> visited(matrix.size(), vector<int>(matrix[0].size(), -1));

    auto [distanceToGoal, path] = aStar(matrix, startX, startY, goalX, goalY, visited, heuristic);

    auto endTime = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::nanoseconds>(endTime - startTime);
   
    statusCode = (distanceToGoal != -1) ? 200 : 404;

    float memoryAfter = getMemoryUsage();
    float memoryUsage = memoryAfter - memoryBefore;
    double timeInMs = duration.count() / 1000000.0;


    saveResultsToJson(visited, statusCode, distanceToGoal, path, timeInMs, memoryUsage, result_name);

    return 0;
}
