/**
 * AStarHeuristicType is the enum that defines the different heuristic function names to calculate the distance towards the goal position for the A*-algorithm
 */
public enum AStarHeuristicType {
    /**
     * The name of the manhattan distance
     */
    MANHATTAN_DISTANCE("manhattan"),

    /**
     * The name of the airplane distance
     */
    AIRPLANE_DISTANCE("airplane");

    /**
     * The string value for the heuristic type
     */
    private final String value;

    /**
     * Constructor of the AStarHeuristicType enum
     * @param value The string value for the heuristic type
     */
    AStarHeuristicType(String value) {
        this.value = value;
    }

    /**
     * Access the string value of a heuristic type
     * @return The string value for the heuristic type
     */
    public String getValue() {
        return value;
    }
}
