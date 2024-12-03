import argparse
import time
import tracemalloc
import heapq
from math import sqrt
from enum import Enum

from read_from_csv import read_from_csv
from write_to_json import write_to_json

class HeuristicType(Enum):
    """An enumeration representing different types of heuristics for the A*-algorithm.
    
    Attributes:
        MANHATTAN_DISTANCE: Represents the Manhattan distance (L1 norm).
        AIRPLANE_DISTANCE: Represents the Euclidean distance (straight-line distance).
    """
    MANHATTAN_DISTANCE = "manhattan"
    AIRPLANE_DISTANCE = "airplane"


def calculate_manhattan_distance(current, goal):
    """Calculate the Manhattan distance as the heuristic function for the A*-algorithm.
    
    Arguments:
        current (tuple[int, int]): The current position on the map.
        goal (tuple[int, int]): The goal position on the map.

    Returns:
        int: The Manhattan distance between the current position and the goal.
    """
    return abs(current[0] - goal[0]) + abs(current[1] - goal[1])


def calculate_airplane_distance(current, goal):
    """Calculate the Euclidean distance (Airplane Distance) as the heuristic function.
    
    Arguments:
        current (tuple[int, int]): The current position on the map.
        goal (tuple[int, int]): The goal position on the map.

    Returns:
        float: The Euclidean distance between the current position and the goal.
    """
    return sqrt((current[0] - goal[0])**2 + (current[1] - goal[1])**2)


def a_star(map, algorithm_map, start_position, goal_position, heuristic_type, status_code):
    """Apply the A* Algorithm to find the shortest path in a grid-based map.
    
    Arguments:
        map (list[list[int]]): The map in form of a list of lists of integers.
        algorithm_map (list[list[int]]): The prepared map in form of a list of lists of integers that will be overwritten by the A*-algorithm.
        start_position (tuple[int, int]): The start position on the map for the A*-algorithm.
        goal_position (tuple[int, int]): The goal position on the map for the A*-algorithm.
        heuristic_type (HeuristicType): The mathematical function chosen to calculate the distance from a tile to the goal (Manhattan or Airplane).
        status_code (int): The status code returned when reading the CSV-file.

    Returns:
        list[list[int]]: The map created by the A*-algorithm filled with the determined values.
        int: The status code symbolizing success or a specific error type prior to or while performing the A*-algorithm.
        int: The shortest determined path length in the form of tiles towards the goal.
        list[tuple[int, int]]: The shortest determined path in form of the positions towards the goal.
        float: The time the A*-algorithm needs to finish in seconds.
        float: The memory used during the execution in MB.
    """
    if status_code != 200:
        return algorithm_map, status_code, -1, [], 0.0, 0.0

    # Start time measurement and memory tracking
    start_time = time.perf_counter()
    tracemalloc.start()

    # Select the heuristic function based on the heuristic type
    if heuristic_type == HeuristicType.MANHATTAN_DISTANCE.value:
        heuristic = calculate_manhattan_distance
    elif heuristic_type == HeuristicType.AIRPLANE_DISTANCE.value:
        heuristic = calculate_airplane_distance
    else:
        return algorithm_map, 401, -1, [], 0.0, 0.0

    # Initialize the priority queue
    rows, cols = len(map), len(map[0])
    directions = [(-1, 0), (1, 0), (0, -1), (0, 1)]
    priority_queue = []
    heapq.heappush(priority_queue, (0, start_position, 0))
    previous_positions = {}
    g_scores = {start_position: 0}
    path_positions = []
    path_length = -1

    # Mark the start position on the algorithm map
    algorithm_map[start_position[0]][start_position[1]] = 0

    # Main loop for A*-algorithm
    while priority_queue:
        _, current_position, g_score = heapq.heappop(priority_queue)
        if current_position == goal_position:
            path_length = g_score
            break

        for direction in directions:
            neighbor = (current_position[0] + direction[0], current_position[1] + direction[1])

            if (
                0 <= neighbor[0] < rows and
                0 <= neighbor[1] < cols and
                map[neighbor[0]][neighbor[1]] != 1
            ):
                tentative_g_score = g_score + 1
                if tentative_g_score < g_scores.get(neighbor, float('inf')):
                    g_scores[neighbor] = tentative_g_score
                    f_score = tentative_g_score + heuristic(neighbor, goal_position)
                    heapq.heappush(priority_queue, (f_score, neighbor, tentative_g_score))
                    previous_positions[neighbor] = current_position
                    algorithm_map[neighbor[0]][neighbor[1]] = tentative_g_score

    # Reconstruct the path if a solution was found
    if path_length != -1:
        current = goal_position
        while current != start_position:
            path_positions.append(current)
            current = previous_positions[current]
        path_positions.append(start_position)
        path_positions.reverse()
    else:
        status_code = 404

    # Measure memory and time usage
    _, peak = tracemalloc.get_traced_memory()
    tracemalloc.stop()

    end_time = time.perf_counter()
    computing_time = end_time - start_time
    memory_usage = peak / 10**6

    return algorithm_map, status_code, path_length, path_positions, computing_time, memory_usage


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="A*-Algorithmus auf Basis einer Map in einer CSV-Datei und Ausgabe in einer JSON-Datei")
    parser.add_argument("csv_path", help="Pfad zur Eingabe-CSV-Datei")
    parser.add_argument("json_path", help="Pfad zur Ausgabe-JSON-Datei")
    parser.add_argument(
        "heuristic_type",
        choices=[type.value for type in HeuristicType],
        help="Die heuristische Methode zur Berechnung der Distanz zwischen einzelnen Feldern: 'Manhattan'- oder 'Airplane'-Distanz"
    )
    args = parser.parse_args()

    # Load map and apply A* algorithm
    map, algorithm_map, start_position, goal_position, status_code = read_from_csv(args.csv_path)
    algorithm_map, status_code, path_length, path_positions, computing_time, memory_usage = a_star(map, algorithm_map, start_position, goal_position, args.heuristic_type, status_code)
    write_to_json(args.json_path, algorithm_map, status_code, path_length, path_positions, computing_time, memory_usage)
