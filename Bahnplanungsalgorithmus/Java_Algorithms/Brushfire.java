import java.lang.management.ManagementFactory;
import java.lang.management.MemoryPoolMXBean;
import java.math.BigDecimal;
import java.math.RoundingMode;
import java.util.ArrayDeque;
import java.util.Deque;

/**
 * Brushfire is the class that applies the Brushfire-algorithm
 */
public class Brushfire {
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
     * The prepared map in form of a multidimensional array that will be overwritten by the Brushfire-algorithm
     */
    private final int[][] algorithmMap;

    /**
     * The status code returned when reading the CSV-file
     */
    private final int statusCode;

    /**
     * Constructor of the Brushfire class
     * @param map The map in form of a multidimensional array
     * @param algorithmMap The prepared map in form of a multidimensional array that will be overwritten by the Brushfire-algorithm
     * @param statusCode The status code returned when reading the CSV-file
     */
    public Brushfire (int[][] map, int[][] algorithmMap, int statusCode) {
        this.map = map;
        this.algorithmMap = algorithmMap;
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
                        (algorithmMap[rowIndex][columnIndex] == -1)
        );
    }

    /**
     * The actual Brushfire-algorithm
     * @return The results of the algorithm in form of an AlgorithmResult
     */
    public AlgorithmResult applyBrushfire() {
        // Checks if an error was encountered prior to the Brushfire-algorithm
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

        // Initializes all the components needed for the Brushfire-algorithm
        Deque<int[]> positionQueue = new ArrayDeque<>();
        int pathLength = -1;
        int[][] pathPositions = new int[][]{};

        // Initialize distances for obstacles
        for (int rowIndex = 0; rowIndex < map.length; rowIndex++) {
            for (int columnIndex = 0; columnIndex < map[rowIndex].length; columnIndex++) {
                if (map[rowIndex][columnIndex] == 1) {
                    algorithmMap[rowIndex][columnIndex] = 0;
                    positionQueue.addLast(new int[]{rowIndex, columnIndex, 0});
                }
            }
        }

        // Performs the Brushfire-algorithm as long as there are still tiles in the positionQueue with unchecked neighbours
        while (!positionQueue.isEmpty()) {
            // Reads a position and saved distance from the position_queue according to the First-In-First-Out principle
            int[] position = positionQueue.removeFirst();
            int rowIndex = position[0];
            int columnIndex = position[1];
            int distance = position[2];

            // For each of the four tiles next to the read position the distance towards an obstacle is saved
            for (int[] direction : DIRECTIONS) {
                int currentRowIndex = rowIndex + direction[0];
                int currentColumnIndex = columnIndex + direction[1];

                // Prior to saving the distance it is checked, if the tile is a valid neighbour
                if (isValidNeighbourTile(currentRowIndex, currentColumnIndex)) {
                    // The determined distance gets saved to the algorithmMap and the field gets added to the positionQueue
                    algorithmMap[currentRowIndex][currentColumnIndex] = distance + 1;
                    positionQueue.addLast(new int[]{currentRowIndex, currentColumnIndex, distance + 1});
                }
            }
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

        // The computingTime in milliseconds is calculated by subtracting the startTime
        double computingTime = (double)(endTime - startTime)/1_000_000.0;

        // The memory usage gets converted to MB
        double memoryUsage = (double)(memoryAfter - memoryBefore) / 1_048_576.0;

        BigDecimal decimalComputingTime = new BigDecimal(computingTime).setScale(19, RoundingMode.HALF_UP);

        return new AlgorithmResult(algorithmMap, statusCode, pathLength, pathPositions, decimalComputingTime, memoryUsage);
    }
}
