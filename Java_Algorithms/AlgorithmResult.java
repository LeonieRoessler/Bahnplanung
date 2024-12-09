import com.google.gson.annotations.SerializedName;

/**
 * AlgorithmResult is the class that represents the result after applying a path planning algorithm
 */
public class AlgorithmResult {
    /**
     * The map created by the path planning algorithm filled with the values determined by the algorithm
     */
    @SerializedName("algorithm_map")
    private int[][] algorithmMap;

    /**
     * The status code returned by the path planning algorithm symbolizing success or a specific error type
     */
    @SerializedName("status_code")
    private int statusCode;

    /**
     * The shortest path length determined by the algorithm in the form of tiles towards the goal
     */
    @SerializedName("path_length")
    private int pathLength;

    /**
     * The positions of the path from start to goal
     */
    @SerializedName("path")
    private int[][] pathPositions;

    /**
     * The time the algorithm needed to finish in seconds
     */
    @SerializedName("computing_time")
    private double computingTime;

    /**
     * The used memory in MB
     */
    @SerializedName("memory_usage")
    private double memoryUsage;

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
        this.computingTime = computingTime;
        this.memoryUsage = memoryUsage;
    }

    public int[][] getAlgorithmMap() {
        return algorithmMap;
    }

    public void setAlgorithmMap(int[][] algorithmMap) {
        this.algorithmMap = algorithmMap;
    }

    public int getStatusCode() {
        return statusCode;
    }

    public void setStatusCode(int statusCode) {
        this.statusCode = statusCode;
    }

    public int getPathLength() {
        return pathLength;
    }

    public void setPathLength(int pathLength) {
        this.pathLength = pathLength;
    }

    public int[][] getPathPositions() {
        return pathPositions;
    }

    public void setPathPositions(int[][] pathPositions) {
        this.pathPositions = pathPositions;
    }

    public double getComputingTime() {
        return computingTime;
    }

    public void setComputingTime(double computingTime) {
        this.computingTime = computingTime;
    }

    public double getMemoryUsage() {
        return memoryUsage;
    }

    public void setMemoryUsage(double memoryUsage) {
        this.memoryUsage = memoryUsage;
    }
}
