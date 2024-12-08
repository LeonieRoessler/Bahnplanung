public class AlgorithmResult {
    private int[][] algorithmMap;
    private int statusCode;
    private int pathLength;
    private int[][] pathPositions;
    private float computingTime;
    private float memoryUsage;

    public AlgorithmResult(int[][] algorithmMap, int statusCode, int pathLength, int[][] pathPositions, float computingTime, float memoryUsage) {
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

    public float getComputingTime() {
        return computingTime;
    }

    public void setComputingTime(float computingTime) {
        this.computingTime = computingTime;
    }

    public float getMemoryUsage() {
        return memoryUsage;
    }

    public void setMemoryUsage(float memoryUsage) {
        this.memoryUsage = memoryUsage;
    }
}
