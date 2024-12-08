import json

def write_to_json(json_path, algorithm_map, status_code, path_length, path, computing_time, memory_usage):
    """
    Writes the results of a path planning algorithm to a JSON-file.

    Arguments:
        json_path (str): The path to the destination of the JSON-File including its name.
        algorithm_map (list[list[int]]): The map created by the path planning algorithm filled with the values determined by the algorithm.
        status_code (int): The status code returned by the path planning algorithm symbolizing success or a specific error type.
        path_length (int): The shortest path length determined by the algorithm in the form of tiles towards the goal.
        computing_time (float): The time the algorithm needed to finish in seconds.
    """
    # Transforms Arguments to a directory
    json_dictionary = {
        "algorithm_map": algorithm_map,
        "status_code": status_code,
        "path_length": path_length,
        "path": path,
        "computing_time": computing_time,
        "memory_usage": memory_usage
    }

    # Writes json_dictionary to JSON-file
    json_object = json.dumps(json_dictionary, indent=4)

    with open(json_path, "w") as json_file:
        json_file.write(json_object)