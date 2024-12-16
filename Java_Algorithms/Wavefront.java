import java.lang.management.MemoryPoolMXBean;
import java.util.ArrayDeque;
import java.util.Deque;
import java.util.HashMap;
import java.lang.management.ManagementFactory;
import java.lang.management.MemoryMXBean;
import java.lang.management.MemoryUsage;

/**
 * Wavefront is the class that applies the Wavefront-algorithm
 */
public class Wavefront {
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
     * The prepared map in form of a multidimensional array that will be overwritten by the Wavefront-algorithm
     */
    private final int[][] algorithmMap;

    /**
     * The start position row index on the map for the Wavefront-algorithm
     */
    private final int startRowIndex;

    /**
     * The start position column index on the map for the Wavefront-algorithm
     */
    private final int startColumnIndex;

    /**
     * The goal position row index on the map for the Wavefront-algorithm
     */
    private final int goalRowIndex;

    /**
     * The goal position column index on the map for the Wavefront-algorithm
     */
    private final int goalColumnIndex;

    /**
     * The status code returned when reading the CSV-file
     */
    private int statusCode;

    /**
     * Constructor of the Wavefront class
     * @param map The map in form of a multidimensional array
     * @param algorithmMap The prepared map in form of a multidimensional array that will be overwritten by the Wavefront-algorithm
     * @param startRowIndex The start position row index on the map for the Wavefront-algorithm
     * @param startColumnIndex The start position column index on the map for the Wavefront-algorithm
     * @param goalRowIndex The goal position row index on the map for the Wavefront-algorithm
     * @param goalColumnIndex The goal position column index on the map for the Wavefront-algorithm
     * @param statusCode The status code returned when reading the CSV-file
     */
    public Wavefront (int[][] map, int[][] algorithmMap, int startRowIndex, int startColumnIndex, int goalRowIndex, int goalColumnIndex, int statusCode) {
        this.map = map;
        this.algorithmMap = algorithmMap;
        this.startRowIndex = startRowIndex;
        this.startColumnIndex = startColumnIndex;
        this.goalRowIndex = goalRowIndex;
        this.goalColumnIndex = goalColumnIndex;
        this.statusCode = statusCode;
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
                (map[rowIndex][columnIndex] != 1) &&
                (algorithmMap[rowIndex][columnIndex] == -1)
        );
    }

    /**
     * The actual Wavefront-algorithm
     * @return The results of the algorithm in form of an AlgorithmResult
     */
    public AlgorithmResult applyWavefront() {
        // Checks if an error was encountered prior to the Wavefront-algorithm
        if (statusCode != 200) {
            return new AlgorithmResult(algorithmMap, statusCode, -1, new int[][]{}, 0, 0);
        }

        // Saves the startTime and starts the observation of the memory usage
        long memoryBefore = 0;

        for (MemoryPoolMXBean memoryPool : ManagementFactory.getMemoryPoolMXBeans()) {
            long usedMemory = memoryPool.getUsage().getUsed();
            if (usedMemory > 0) {
                memoryBefore = memoryBefore + usedMemory;
            }
        }

        long startTime = System.nanoTime();

        // Initializes all the components needed for the Wavefront-algorithm
        Deque<int[]> positionQueue = new ArrayDeque<>();
        HashMap<String, String> previousPositions = new HashMap<>();
        int pathLength = -1;
        int[][] pathPositions = new int[][]{};

        // Starts the Wavefront-algorithm at the start position by setting the distance
        algorithmMap[startRowIndex][startColumnIndex] = 0;
        positionQueue.addLast(new int[]{startRowIndex, startColumnIndex, 0});

        // Performs the Wavefront-algorithm as long as there are still tiles in the positionQueue with unchecked neighbours
        wavefrontAlgorithm:
        while (!positionQueue.isEmpty()) {
            // Reads a position and saved distance from the position_queue according to the First-In-First-Out principle
            int[] position = positionQueue.removeFirst();
            int rowIndex = position[0];
            int columnIndex = position[1];
            int distance = position[2];

            // For each of the four tiles next to the read position the distance is saved
            for (int[] direction : DIRECTIONS) {
                int currentRowIndex = rowIndex + direction[0];
                int currentColumnIndex = columnIndex + direction[1];

                // Prior to saving the distance it is checked, if the tile is a valid neighbour
                if (isValidNeighbourTile(currentRowIndex, currentColumnIndex)) {
                    // The determined distance gets saved to the algorithmMap and the field gets added to the positionQueue
                    algorithmMap[currentRowIndex][currentColumnIndex] = distance + 1;
                    positionQueue.addLast(new int[]{currentRowIndex, currentColumnIndex, distance + 1});

                    // Saves the previous position to each current position to determine the path towards the goal at a later step
                    previousPositions.put(currentRowIndex + "," + currentColumnIndex, rowIndex + "," + columnIndex);

                    // If the read position is the goal position its distance is saved as the pathLength and the algorithm stops
                    if ((currentRowIndex == goalRowIndex) && (currentColumnIndex == goalColumnIndex)) {
                        pathLength = distance + 1;
                        break wavefrontAlgorithm;
                    }
                }
            }
        }

        // If no path towards the goal could be found, the corresponding status_code gets returned
        if (pathLength == -1){
            statusCode = 404;
        } else {
            // Reconstruct the Wavefront-algorithms path
            PathReconstructor pathReconstructor = new PathReconstructor(startRowIndex, startColumnIndex, goalRowIndex, goalColumnIndex, previousPositions);
            pathPositions = pathReconstructor.reconstructPath();
        }

        // The endTime gets saved
        long endTime = System.nanoTime();

        // The tracking of the memory usage is stopped
        long memoryAfter = 0;

        for (MemoryPoolMXBean memoryPool : ManagementFactory.getMemoryPoolMXBeans()) {
            long usedMemory = memoryPool.getUsage().getUsed();
            if (usedMemory > 0) {
                memoryAfter = memoryAfter + usedMemory;
            }
        }

        // The computingTime in seconds is calculated by subtracting the startTime
        double computingTime = (double)(endTime - startTime)/1_000_000_000.0;

        // The memory usage gets converted to MB
        double memoryUsage = (double)(memoryAfter - memoryBefore) / 1_048_576.0;

        return new AlgorithmResult(algorithmMap, statusCode, pathLength, pathPositions, computingTime, memoryUsage);
    }
}
