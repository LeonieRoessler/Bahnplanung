import csv

def initialize_algorithm_map(map):
    """
    Initializes a map for a path planning algorithm and searches for start and end positions.

    Arguments:
        map (list[list[int]]): The input map.

    Returns:
        list[list[int]]: The prepared map in form of a list of lists of integers for the path planning algorithm.
        tuple(int, int): The start position on the map for the path planning algorithm.
        tuple(int, int): The goal position on the map for the path planning algorithm.
        int: A status code to represent success or a specific error type when initializing the algorithm map.
    """
    start_position = None
    goal_position = None

    # Initializes algorithm_map with same number of rows and columns as map and sets all values to -1 (not visited)
    algorithm_map = [[-1 for _ in row] for row in map]

    # Searches for start position (2) and goal position (3) to save and return them
    for row_index, row in enumerate(map):
        for column_index, value in enumerate(row):
            if value == 2:
                # Checks if more than one start position was accidentally assigned and returns the corresponding error code in this case
                if start_position is not None:
                    return [], None, None, 422
                start_position = (row_index, column_index)
            elif value == 3:
                # Checks if more than one goal position was accidentally assigned and returns the corresponding error code in this case
                if goal_position is not None:
                    return [], None, None, 433
                goal_position = (row_index, column_index)

    # Checks if no start position was assigned and returns the corresponding error code in this case
    if start_position is None:
        return [], None, None, 402

    # Checks if no goal position was assigned and returns the corresponding error code in this case
    if goal_position is None:
        return [], None, None, 403

    return algorithm_map, start_position, goal_position, 200


def read_from_csv(csv_path):
    """
    Reads a map from a CSV-file and returns the result in the form of a list of lists of integers.

    Arguments:
        csv_path (str): The path to the CSV-file.

    Returns:
        list[list[int]]: The map in form of a list of lists of integers.
        list[list[int]]: The prepared map in form of a list of lists of integers that can be overwritten by the path planning algorithms to show how they work.
        tuple(int, int): The start position on the map for the path planning algorithm.
        tuple(int, int): The goal position on the map for the path planning algorithm.
        int: A status code to represent success or a specific error type when reading the CSV-file.
    """
    try:
        # Reads CSV-file from csv_path and transforms it into the form of list[list[int]]
        with open(csv_path, mode='r', encoding='utf-8') as csv_file:
            reader = csv.reader(csv_file)
            map = [[int(cell) for cell in row] for row in reader]

            # Calls method to initialize the map for the path planning algorithm
            algorithm_map, start_position, goal_position, status_code = initialize_algorithm_map(map)
        return map, algorithm_map, start_position, goal_position, status_code

    # Returns corresponding error codes in case of errors
    except FileNotFoundError:
        return [], [], 504
    except ValueError:
        return [], [], 500
    except Exception:
        return [], [], 509