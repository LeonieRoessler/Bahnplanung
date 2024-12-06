import argparse
import time
import tracemalloc
import heapq
from math import sqrt
from enum import Enum

from read_from_csv import read_from_csv
from reconstruct_path import reconstruct_path
from write_to_json import write_to_json

class HeuristicType(Enum):
    """An enumeration representing different types of heuristics for the A*-algorithm.
    
    Attributes:
        MANHATTAN_DISTANCE: Represents the Manhattan distance.
        AIRPLANE_DISTANCE: Represents the Airplane distance.
    """
    MANHATTAN_DISTANCE = "manhattan"
    AIRPLANE_DISTANCE = "airplane"


def calculate_manhattan_distance(current_position, goal_position):
    """Calculate the Manhattan distance between two tiles.

    Arguments:
        current_position (tuple[int, int]): The current position on the map.
        goal_position (tuple[int, int]): The goal position on the map.

    Returns:
        int: The Manhattan distance between the current position and the goal.
    """
    return abs(current_position[0] - goal_position[0]) + abs(current_position[1] - goal_position[1])


def calculate_airplane_distance(current_position, goal_position):
    """Calculate the Airplane distance between two tiles.

    Arguments:
        current_position (tuple[int, int]): The current position on the map.
        goal_position (tuple[int, int]): The goal position on the map.

    Returns:
        float: The Airplane distance between the current position and the goal.
    """
    return sqrt((current_position[0] - goal_position[0])**2 + (current_position[1] - goal_position[1])**2)

def is_valid_neighbour_tile(row_index, column_index, map):
    """Check if a given tile is part of the map, not an obstacle, and was not visited before.

    Arguments:
        row_index (int): The row index of the current position.
        column_index (int): The columns index of the current position.
        map (list[list[int]]): The map on which the A*-algorithm is applied.

    Returns:
        boolean: Returns whether the tile is a valid neighbour or not.
    """
    return (
        0 <= row_index < len(map)
        and 0 <= column_index < len(map[0])
        and map[row_index][column_index] != 1
    )


def a_star(map, algorithm_map, start_position, goal_position, heuristic_type, status_code):
    """Apply the A* Algorithm.

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
    # Checks if an error was encountered prior to the A*-algorithm
    if status_code != 200:
        return algorithm_map, status_code, -1, [], 0.0, 0.0

    # Saves the start_time and the observation of the memory usage is started
    start_time = time.perf_counter()
    tracemalloc.start()

    # Selects the heuristic function based on the heuristic_type argument
    if heuristic_type == HeuristicType.MANHATTAN_DISTANCE.value:
        heuristic = calculate_manhattan_distance
    elif heuristic_type == HeuristicType.AIRPLANE_DISTANCE.value:
        heuristic = calculate_airplane_distance
    else:
        # Returns error code, if wrong heuristic_type argument was given
        return algorithm_map, 401, -1, [], 0.0, 0.0

    # Initializes all the components needed for the A*-algorithm
    directions = [(-1, 0), (1, 0), (0, -1), (0, 1)]
    open_priority_queue = []
    previous_positions = {}
    path_length = -1

    # Starts the A*-algorithm at the start position by setting its distance and adding it to the priority queue
    algorithm_map[start_position[0]][start_position[1]] = 0
    heapq.heappush(open_priority_queue, (0, start_position[0], start_position[1], 0))
    f_scores = {start_position: 0}

    # Performs the A*-algorithm as long as there are still tiles in the open_priority_queue with unchecked neighbours
    while open_priority_queue:
        # Reads a position and saved distance to the start position (g_score) from the open_priority_queue based on the lowest estimated distance towards the goal position (f_score)
        f_score, row_index, column_index, g_score = heapq.heappop(open_priority_queue)

        print(f_score, row_index, column_index, g_score)

        # If the read position is the goal position its distance is saved as the path_length and the algorithm stops
        if (row_index, column_index) == goal_position:
            path_length = g_score
            break

        # For each of the four tiles next to the read position the g_score and f_score is saved
        for row_direction, column_direction in directions:
            current_row_index = row_index + row_direction
            current_column_index = column_index + column_direction

            # Prior to saving the g_score and f_score it is checked, if the tile is a valid neighbour
            if is_valid_neighbour_tile(current_row_index, current_column_index, map):
                # The g_score and f_score of the current position is calculated
                current_g_score = g_score + 1
                current_f_score = current_g_score + heuristic((current_row_index, current_column_index), goal_position)

                # The current values only get added to the open_priority_queue if there was no f_score calculated for the position which is lower than the current_f_score
                if current_f_score < f_scores.get((current_row_index, current_column_index), float('inf')):
                    # The determined g_score which represents the distance is saved to the algorithm_map, the f_score gets added to f_scores, and all the values get added to the open_priority_queue
                    algorithm_map[current_row_index][current_column_index] = current_g_score
                    f_scores[current_row_index, current_column_index] = current_f_score
                    heapq.heappush(open_priority_queue, (current_f_score, current_row_index, current_column_index, current_g_score))

                    # Saves the previous position to each current position to determine the path towards the goal at a later step
                    previous_positions[(current_row_index, current_column_index)] = (row_index, column_index)

    # If no path towards the goal could be found, the corresponding status_code gets returned
    if path_length == -1:
        status_code = 404
    else:
        # Reconstruct the A*-algorithms path
        path_positions = reconstruct_path(start_position, goal_position, previous_positions)

    # The memory usage gets saved and the tracking is stopped
    _, peak = tracemalloc.get_traced_memory()
    tracemalloc.stop()

    # The end_time gets saved and the computing_time in seconds is calculated by subtracting the start_time and rounding the result to two decimal places
    end_time = time.perf_counter()
    computing_time = end_time - start_time

    # The memory usage gets converted to MB
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
