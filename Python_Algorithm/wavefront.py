import argparse
import time
import tracemalloc
from collections import deque

from read_from_csv import read_from_csv
from write_to_json import write_to_json

def wavefront(map, algorithm_map, start_position, goal_position, status_code):
    """Apply the Wavefront Algorithm.

    Arguments:
        map (list[list[int]]): The map in form of a list of lists of integers.
        algorithm_map (list[list[int]]): The prepared map in form of a list of lists of integers that will be overwritten by the Wavefront-algorithm.
        start_position (tuple(int, int)): The start position on the map for the Wavefront-algorithm.
        goal_position (tuple(int, int)): The goal position on the map for the Wavefront-algorithm.
        status_code (int): The status code returned when reading the CSV-file.

    Returns:
        list[list[int]]: The map created by the Wavefront-algorithm filled with the determined values.
        int: The status code symbolizing success or a specific error type prior to or while performing the Wavefront-algorithm.
        int: The shortest determined path length in the form of tiles towards the goal.
        float: The time the Wavefront-algorithm needs to finish in seconds.
        float: The memory used during the execution in MB.
    """
    # Checks if an error was encountered prior to the Wavefront-algorithm
    if status_code != 200:
        return algorithm_map, status_code, -1, 0.0, 0.0

    # Starts start_time gets saved and the observation of the memory usage is started
    start_time = time.time()
    tracemalloc.start()

    # Initializes all the components needed for the Wavefront-algorithm
    row_number, column_number = len(map), len(map[0])
    directions = [(-1, 0), (1, 0), (0, -1), (0, 1)]
    position_queue = deque()
    visited_positions = set()

    # Starts the Wavefront-algorithm at the start position by setting the distance
    algorithm_map[start_position[0]][start_position[1]] = 0
    position_queue.append((start_position[0], start_position[1], 0))
    visited_positions.add(start_position)

    # Initializing the path_length
    path_length = -1

    # Performs the Wavefront-algorithm as long as there are still tiles in the position_queue with unchecked neighbours
    while position_queue:

        # Reads a position and saved distance from the position_queue according to the First-In-First-Out principle
        row_index, column_index, distance = position_queue.popleft()

        # If the read position is the goal position its distance is saved as the path_length and the algorithm stops
        if (row_index, column_index) == goal_position:
            path_length = distance
            break

        # For each of the four tiles next to the read position the distance is saved
        for row_direction, column_direction in directions:
            current_row_index = row_index + row_direction
            current_column_index = column_index + column_direction

            # Prior to saving the distance it is checked, if the tile even is part of the map, was not visited before, and is not an obstacle
            if 0 <= current_row_index < row_number and 0 <= current_column_index < column_number and (current_row_index, current_column_index) not in visited_positions:
                if map[current_row_index][current_column_index] != 1:

                    # The determined distance gets saved to the algorithm_map and the field gets added to the position_queue and visited_positions
                    algorithm_map[current_row_index][current_column_index] = distance + 1
                    position_queue.append((current_row_index, current_column_index, distance + 1))
                    visited_positions.add((current_row_index, current_column_index))

    # The memory usage gets saved and the tracking is stopped
    _, peak = tracemalloc.get_traced_memory()
    tracemalloc.stop()

    # The end_time gets saved and the computing_time in seconds is calculated by subtracting the start_time and rounding the result to two decimal places
    end_time = time.time()
    computing_time = round(end_time - start_time, 2)

    # The memory usage gets converted to MB
    memory_usage = peak / 10**6

    # If no path towards the goal could be found, the corresponding status_code gets returned
    if path_length == -1:
        status_code = 404

    return algorithm_map, status_code, path_length, computing_time, memory_usage


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="CSV zu JSON Konvertierung")
    parser.add_argument("csv_path", help="Pfad zur Eingabe-CSV-Datei")
    parser.add_argument("json_path", help="Pfad zur Ausgabe-JSON-Datei")
    args = parser.parse_args()

    map, algorithm_map, start_position, goal_position, status_code = read_from_csv(args.csv_path)
    algorithm_map, status_code, path_length, computing_time, memory_usage = wavefront(map, algorithm_map, start_position, goal_position, status_code)
    write_to_json(args.json_path, algorithm_map, status_code, path_length, computing_time, memory_usage)