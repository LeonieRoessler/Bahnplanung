import com.google.gson.annotations.SerializedName;

import java.math.BigDecimal;
import java.math.RoundingMode;

/**
 * AlgorithmResult is the class that represents the result after applying a path planning algorithm
 */
public class AlgorithmResult {
    /**
     * The map created by the path planning algorithm filled with the values determined by the algorithm
     */
    @SerializedName("algorithm_map")
    private final int[][] algorithmMap;

    /**
     * The status code returned by the path planning algorithm symbolizing success or a specific error type
     */
    @SerializedName("status_code")
    private final int statusCode;

    /**
     * The shortest path length determined by the algorithm in the form of tiles towards the goal
     */
    @SerializedName("path_length")
    private final int pathLength;

    /**
     * The positions of the path from start to goal
     */
    @SerializedName("path")
    private final int[][] pathPositions;

    /**
     * The time the algorithm needed to finish in seconds
     */
    @SerializedName("computing_time")
    private final BigDecimal computingTime;

    /**
     * The used memory in MB
     */
    @SerializedName("memory_usage")
    private final double memoryUsage;

    /**
     * Constructor of the AlgorithmResult class
     * @param algorithmMap The map created by the path planning algorithm filled with the values determined by the algorithm
     * @param statusCode The status code returned by the path planning algorithm symbolizing success or a specific error type
     * @param pathLength The shortest path length determined by the algorithm in the form of tiles towards the goal
     * @param pathPositions The positions of the path from start to goal
     * @param computingTime The time the algorithm needed to finish in seconds
     * @param memoryUsage The used memory in MB
     */
    public AlgorithmResult(int[][] algorithmMap, int statusCode, int pathLength, int[][] pathPositions, BigDecimal computingTime, double memoryUsage) {
        this.algorithmMap = algorithmMap;
        this.statusCode = statusCode;
        this.pathLength = pathLength;
        this.pathPositions = pathPositions;
        this.computingTime = computingTime;
        this.memoryUsage = memoryUsage;
    }

    /**
     * Constructor of the AlgorithmResult class
     * @param algorithmMap The map created by the path planning algorithm filled with the values determined by the algorithm
     * @param statusCode The status code returned by the path planning algorithm symbolizing success or a specific error type
     * @param pathLength The shortest path length determined by the algorithm in the form of tiles towards the goal
     * @param pathPositions The positions of the path from start to goal
     * @param computingTime The time the algorithm needed to finish in seconds
     * @param memoryUsage The used memory in MB
     */
    public AlgorithmResult(int[][] algorithmMap, int statusCode, int pathLength, int[][] pathPositions, double computingTime, double memoryUsage) {
        this.algorithmMap = algorithmMap;
        this.statusCode = statusCode;
        this.pathLength = pathLength;
        this.pathPositions = pathPositions;
        this.computingTime = new BigDecimal(computingTime).setScale(19, RoundingMode.HALF_UP);
        this.memoryUsage = memoryUsage;
    }
}
