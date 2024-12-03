import argparse
import time
import tracemalloc
from collections import deque

from read_from_csv import read_from_csv
from write_to_json import write_to_json

def is_valid_neighbour_tile(row_index, column_index, map, algorithm_map):
    """Check if a given tile is part of the map, not an obstacle, and was not visited before.

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
        and map[row_index][column_index] != 1
        and algorithm_map[row_index][column_index] == -1
    )

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
        list[tuple[int, int]]: The shortest determined path in form of the positions towards the goal.
        float: The time the Wavefront-algorithm needs to finish in seconds.
        float: The memory used during the execution in MB.
    """
    # Checks if an error was encountered prior to the Wavefront-algorithm
    if status_code != 200:
        return algorithm_map, status_code, -1, 0.0, 0.0

    # Starts start_time gets saved and the observation of the memory usage is started
    start_time = time.perf_counter()
    tracemalloc.start()

    # Initializes all the components needed for the Wavefront-algorithm
    directions = [(-1, 0), (1, 0), (0, -1), (0, 1)]
    position_queue = deque()

    # Starts the Wavefront-algorithm at the start position by setting the distance
    algorithm_map[start_position[0]][start_position[1]] = 0
    position_queue.append((start_position[0], start_position[1], 0))
    previous_positions = {}

    # Initializing the path_length and an empty path
    path_length = -1
    path_positions = []

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

            # Prior to saving the distance it is checked, if the tile is a valid neighbour
            if is_valid_neighbour_tile(current_row_index, current_column_index, map, algorithm_map):
                    # The determined distance gets saved to the algorithm_map and the field gets added to the position_queue and visited_positions
                    algorithm_map[current_row_index][current_column_index] = distance + 1
                    position_queue.append((current_row_index, current_column_index, distance + 1))

                    # Save the previous position to each current position to determine the path towards the goal at a later step
                    previous_positions[(current_row_index, current_column_index)] = (row_index, column_index)

    # If no path towards the goal could be found, the corresponding status_code gets returned
    if path_length == -1:
        status_code = 404
    else:
        # Reconstruct the path reading the previous position of a position from the previous_positions
        current_position = goal_position
        while current_position != start_position:
            path_positions.append(current_position)
            current_position = previous_positions[current_position]
        path_positions.append(start_position)
        path_positions.reverse()

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
    """The main function of the Wavefront-algorithm.

    Arguments:
        csv_path (string): The path towards the CSV-file with the map the Wavefront-algorithm is supposed to be applied to.
        json_path (string): The path towards the JSON-file where the results of the Wavefront-algorithm are ssaved.
    """
    parser = argparse.ArgumentParser(description="Wavefront-Algorithmus auf Basis einer Map in einer CSV-Datei und Ausgabe in einer JSON-Datei")
    parser.add_argument("csv_path", help="Pfad zur Eingabe-CSV-Datei")
    parser.add_argument("json_path", help="Pfad zur Ausgabe-JSON-Datei")
    args = parser.parse_args()

    # Reads in the map, applies the Wavefront-algorithm to it and saves the results to a JSON-file
    map, algorithm_map, start_position, goal_position, status_code = read_from_csv(args.csv_path)
    algorithm_map, status_code, path_length, path_positions, computing_time, memory_usage = wavefront(map, algorithm_map, start_position, goal_position, status_code)
    write_to_json(args.json_path, algorithm_map, status_code, path_length, path_positions, computing_time, memory_usage)