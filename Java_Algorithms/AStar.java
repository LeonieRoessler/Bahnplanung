import java.util.Comparator;
import java.util.HashMap;
import java.util.PriorityQueue;
import java.util.function.BiFunction;

/**
 * AStar is the class that applies the A*-algorithm
 */
public class AStar {
    /**
     * The four directions to neighbour tiles
     */
    private static final int[][] DIRECTIONS = {
            {-1, 0}, {1, 0}, {0, -1}, {0, 1}
    };

    /**
     * The map in form of a multidimensional array
     */
    private final int[][] map;

    /**
     * The prepared map in form of a multidimensional array that will be overwritten by the A*-algorithm
     */
    private final int[][] algorithmMap;

    /**
     * The start position row index on the map for the A*-algorithm
     */
    private final int startRowIndex;

    /**
     * The start position column index on the map for the A*-algorithm
     */
    private final int startColumnIndex;

    /**
     * The goal position row index on the map for the A*-algorithm
     */
    private final int goalRowIndex;

    /**
     * The goal position column index on the map for the A*-algorithm
     */
    private final int goalColumnIndex;

    /**
     * The heuristic function type to calculate the distance towards the goal position
     */
    private final String heuristicType;

    /**
     * The status code returned when reading the CSV-file
     */
    private int statusCode;

    /**
     * Constructor of the AStar class
     * @param map The map in form of a multidimensional array
     * @param algorithmMap The prepared map in form of a multidimensional array that will be overwritten by the A*-algorithm
     * @param startRowIndex The start position row index on the map for the A*-algorithm
     * @param startColumnIndex The start position column index on the map for the A*-algorithm
     * @param goalRowIndex The goal position row index on the map for the A*-algorithm
     * @param goalColumnIndex The goal position column index on the map for the A*-algorithm
     * @param heuristicType The heuristic function type to calculate the distance towards the goal position
     * @param statusCode The status code returned when reading the CSV-file
     */
    public AStar (int[][] map, int[][] algorithmMap, int startRowIndex, int startColumnIndex, int goalRowIndex, int goalColumnIndex, String heuristicType, int statusCode) {
        this.map = map;
        this.algorithmMap = algorithmMap;
        this.startRowIndex = startRowIndex;
        this.startColumnIndex = startColumnIndex;
        this.goalRowIndex = goalRowIndex;
        this.goalColumnIndex = goalColumnIndex;
        this.heuristicType = heuristicType;
        this.statusCode = statusCode;
    }

    /**
     * Calculate the Manhattan distance between two tiles
     * @param rowIndex The row index of the current position
     * @param columnIndex The columns index of the current position
     * @return The Manhattan distance between the current position and the goal
     */
    private double calculateManhattanDistance(int rowIndex, int columnIndex) {
        return (Math.abs(rowIndex - goalRowIndex) + Math.abs(columnIndex - goalColumnIndex));
    }

    /**
     * Calculate the Airplane distance between two tiles
     * @param rowIndex The row index of the current position
     * @param columnIndex The columns index of the current position
     * @return The Airplane distance between the current position and the goal
     */
    private double calculateAirplaneDistance(int rowIndex, int columnIndex) {
        return (Math.sqrt(Math.pow((rowIndex - goalRowIndex), 2) + Math.pow((columnIndex - goalColumnIndex), 2)));
    }

    /**
     * Check if a given tile is part of the map, not an obstacle, and was not visited before
     * @param rowIndex The row index of the current position
     * @param columnIndex The columns index of the current position
     * @return Whether the tile is a valid neighbour or not
     */
    private boolean isValidNeighbourTile(int rowIndex, int columnIndex) {
        return (
                (0 <= rowIndex && rowIndex < map.length) &&
                (0 <= columnIndex && columnIndex < map.length) &&
                (map[rowIndex][columnIndex] != 1)
        );
    }

    /**
     * The actual A*-algorithm
     * @return The results of the algorithm in form of an AlgorithmResult
     */
    public AlgorithmResult applyAStar() {
        // Checks if an error was encountered prior to the A*-algorithm
        if (statusCode != 200) {
            return new AlgorithmResult(algorithmMap, statusCode, -1, new int[][]{}, 0, 0);
        }

        // Calls Garbage Collector to clean memory
        System.gc();

        // Saves the startTime and starts the observation of the memory usage
        long startTime = System.nanoTime();
        long memoryBefore = Runtime.getRuntime().totalMemory() - Runtime.getRuntime().freeMemory();

        // Returns error code, if wrong heuristic_type argument was given
        if (!(heuristicType.equalsIgnoreCase(AStarHeuristicType.MANHATTAN_DISTANCE.getValue())|| heuristicType.equalsIgnoreCase(AStarHeuristicType.AIRPLANE_DISTANCE.getValue()))) {
            return new AlgorithmResult(algorithmMap, 401, -1, new int[][]{}, 0, 0);
        } else {
            // Selects the heuristic function based on the heuristicType argument
            BiFunction<Integer, Integer, Double> heuristic;
            if (heuristicType.equalsIgnoreCase(AStarHeuristicType.MANHATTAN_DISTANCE.getValue())) {
                heuristic = this::calculateManhattanDistance;
            } else {
                heuristic = this::calculateAirplaneDistance;
            }

            // Initializes all the components needed for the A*-algorithm
            PriorityQueue<double[]> openPriorityQueue = new PriorityQueue<>(Comparator.comparingDouble((double[] a) -> a[0])
                    .thenComparingDouble(a -> a[1])
                    .thenComparingDouble(a -> a[2])
                    .thenComparingDouble(a -> a[3]));
            HashMap<String, String> previousPositions = new HashMap<>();
            HashMap<String, Integer> gScores = new HashMap<>();
            int pathLength = -1;
            int[][] pathPositions = new int[][]{};

            // Starts the A*-algorithm at the start position by setting its distance and adding it to the priority queue
            algorithmMap[startRowIndex][startColumnIndex] = 0;
            openPriorityQueue.add(new double[]{0, 0, startRowIndex, startColumnIndex});
            gScores.put(startRowIndex + "," + startColumnIndex, 0);

            while (!openPriorityQueue.isEmpty()) {
                double[] position = openPriorityQueue.poll();
                int gScore = (int) position[1];
                int rowIndex = (int) position[2];
                int columnIndex = (int) position[3];

                // If the read position is the goal position its distance is saved as the pathLength and the algorithm stops
                if ((rowIndex == goalRowIndex) && (columnIndex == goalColumnIndex)) {
                    pathLength = gScore;
                    break;
                }

                // For each of the four tiles next to the read position the distance is saved
                for (int[] direction : DIRECTIONS) {
                    int currentRowIndex = rowIndex + direction[0];
                    int currentColumnIndex = columnIndex + direction[1];

                    // Prior to saving the distance it is checked, if the tile is a valid neighbour
                    if (isValidNeighbourTile(currentRowIndex, currentColumnIndex)) {
                        // The gScore of the current position is calculated
                        int currentGScore = gScore + 1;

                        // The current values only get added to the openPriorityQueue if there was no gScore calculated for the position which is lower than the currentGScore
                        if (currentGScore < gScores.getOrDefault(currentRowIndex + "," + currentColumnIndex, Integer.MAX_VALUE)) {
                            // The fScore of the current position is calculated
                            double currentFScore = currentGScore + heuristic.apply(currentRowIndex, currentColumnIndex);

                            // The determined gScore which represents the distance is saved to the algorithmMap, added to gScores, and all the values get added to the openPriorityQueue
                            algorithmMap[currentRowIndex][currentColumnIndex] = currentGScore;
                            gScores.put(currentRowIndex + "," + currentColumnIndex, currentGScore);
                            openPriorityQueue.add(new double[]{currentFScore, currentGScore, currentRowIndex, currentColumnIndex});

                            // Saves the previous position to each current position to determine the path towards the goal at a later step
                            previousPositions.put(currentRowIndex + "," + currentColumnIndex, rowIndex + "," + columnIndex);
                        }
                    }
                }
            }


            // If no path towards the goal could be found, the corresponding status_code gets returned
            if (pathLength == -1){
                statusCode = 404;
            } else {
                // Reconstruct the A*-algorithms path
                PathReconstructor pathReconstructor = new PathReconstructor(startRowIndex, startColumnIndex, goalRowIndex, goalColumnIndex, previousPositions);
                pathPositions = pathReconstructor.reconstructPath();
            }

            // The tracking of the memory usage is stopped
            long memoryAfter = Runtime.getRuntime().totalMemory() - Runtime.getRuntime().freeMemory();

            // The endTime gets saved and the computingTime in seconds is calculated by subtracting the startTime
            long endTime = System.nanoTime();
            double computingTime = (double)(endTime - startTime)/1_000_000_000.0;

            // The memory usage gets converted to MB
            double memoryUsage = (double)(memoryAfter - memoryBefore) / 1_048_576.0;

            return new AlgorithmResult(algorithmMap, statusCode, pathLength, pathPositions, computingTime, memoryUsage);
        }
    }
}