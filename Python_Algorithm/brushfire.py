import argparse
import time
import tracemalloc
from collections import deque

from read_from_csv import read_from_csv
from write_to_json import write_to_json

def is_valid_neighbour_tile(row_index, column_index, map, algorithm_map):
    """Check if a given tile is part of the map and was not visited before.

    Arguments:
        row_index (int): The row index of the current position.
        column_index (int): The columns index of the current position.
        map (list[list[int]]): The map on which the Wavefront-algorithm is applied.
        algorithm_map (list[list[int]]): The current map written by the Wavefront-algorithm.

    Returns:
        boolean: Returns whether the tile is a valid neighbour or not.
    """
    return (
        0 <= row_index < len(map)
        and 0 <= column_index < len(map[0])
        and algorithm_map[row_index][column_index] == -1
    )


def brushfire(map, algorithm_map, status_code):
    """Apply the Brushfire Algorithm.

    Arguments:
        map (list[list[int]]): The map in form of a list of lists of integers.
        algorithm_map (list[list[int]]): The prepared map in form of a list of lists of integers that will be overwritten by the Brushfire-algorithm.
        status_code (int): The status code returned when reading the CSV-file.

    Returns:
        list[list[int]]: The map created by the Brushfire-algorithm filled with the determined values.
        int: The status code symbolizing success or a specific error type prior to or while performing the Brushfire-algorithm.
        int: The shortest determined path length in the form of tiles towards the goal.
        list[tuple[int, int]]: The shortest determined path in form of the positions towards the goal.
        float: The time the Brushfire-algorithm needs to finish in seconds.
        float: The memory used during the execution in MB.
    """
    # Checks if an error was encountered prior to the Brushfire-algorithm
    if status_code != 200:
        return algorithm_map, status_code, -1, [], 0.0, 0.0

    # Saves the start_time and the observation of the memory usage is started
    start_time = time.perf_counter()
    tracemalloc.start()

    # Initializes all the components needed for the Brushfire-algorithm
    directions = [(-1, 0), (1, 0), (0, -1), (0, 1)]
    position_queue = deque()
    path_length = -1
    path_positions = []

    # Initialize distances for obstacles
    for row_index in range(len(map)):
        for column_index in range(len(map[0])):
            if map[row_index][column_index] == 1:
                algorithm_map[row_index][column_index] = 0
                position_queue.append((row_index, column_index, 0))

    # Performs the Brushfire-algorithm as long as there are still tiles in the position_queue with unchecked neighbours
    while position_queue:
        # Reads a position and saved distance from the position_queue according to the First-In-First-Out principle
        row_index, column_index, current_distance = position_queue.popleft()

        # For each of the four tiles next to the read position the distance towards an obstacle is saved
        for row_direction, column_direction in directions:
            current_row_index = row_index + row_direction
            current_column_index = column_index + column_direction

            # Prior to saving the distance it is checked, if the tile is a valid neighbour
            if is_valid_neighbour_tile(current_row_index, current_column_index, map, algorithm_map):
                # The determined distance gets saved to the algorithm_map and the field gets added to the position_queue
                algorithm_map[current_row_index][current_column_index] = current_distance + 1
                position_queue.append((current_row_index, current_column_index, current_distance + 1))

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
    """The main function of the Brushfire-algorithm.

    Arguments:
        csv_path (string): The path towards the CSV-file with the map the Brushfire-algorithm is supposed to be applied to.
        json_path (string): The path towards the JSON-file where the results of the Brushfire-algorithm are saved.
    """
    parser = argparse.ArgumentParser(description="Brushfire-Algorithmus auf Basis einer Map in einer CSV-Datei und Ausgabe in einer JSON-Datei")
    parser.add_argument("csv_path", help="Pfad zur Eingabe-CSV-Datei")
    parser.add_argument("json_path", help="Pfad zur Ausgabe-JSON-Datei")
    args = parser.parse_args()

    # Reads in the map, applies the Brushfire-algorithm to it and saves the results to a JSON-file
    map, algorithm_map, _, _, status_code = read_from_csv(args.csv_path)
    algorithm_map, status_code, path_length, path_positions, computing_time, memory_usage = brushfire(map, algorithm_map, status_code)
    write_to_json(args.json_path, algorithm_map, status_code, path_length, path_positions, computing_time, memory_usage)