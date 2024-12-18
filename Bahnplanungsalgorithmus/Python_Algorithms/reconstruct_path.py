def reconstruct_path(start_position, goal_position, previous_positions):
    """Reconstruct a path from the results of a path planning algorithm.

    Arguments:
        start_position (tuple[int, int]): The start position on the map.
        goal_position (tuple[int, int]): The goal position on the map.
        previous_positions (dict): A set that includes the previous position for every visited position on the map ({position: previous position}).

    Returns:
        list[tuple[int, int]]: The list of path positions.
    """
    # Reconstructs the path of a path planning algorithm starting from the goal and looping through the previous positions
    path_positions = []
    current_position = goal_position
    while current_position != start_position:
        path_positions.append(current_position)
        current_position = previous_positions[current_position]
    path_positions.append(start_position)
    path_positions.reverse()
    return path_positions