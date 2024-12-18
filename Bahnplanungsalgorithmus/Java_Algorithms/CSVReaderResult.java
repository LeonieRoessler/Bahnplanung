/**
 * CSVReaderResult is the class that represents the result after reading a map from a CSV-file and initializing the map for a path planning algorithm
 */
public class CSVReaderResult {
    /**
     * The map from the CSV-file
     */
    private final int[][] map;

    /**
     * The initialized map for the path planning algorithm
     */
    private final int[][] algorithmMap;

    /**
     * The row index of the start position
     */
    private final int startRowIndex;

    /**
     * The column index of the start position
     */
    private final int startColumnIndex;

    /**
     * The row index of the goal position
     */
    private final int goalRowIndex;

    /**
     * The column index of the goal position
     */
    private final int goalColumnIndex;

    /**
     * The status code after reading from the CSV-file
     */
    private final int statusCode;

    /**
     * Constructor of the CSVReaderResult class
     * @param map The map from the CSV-file
     * @param algorithmMap The initialized map for the path planning algorithm
     * @param startRowIndex The row index of the start position
     * @param startColumnIndex The column index of the start position
     * @param goalRowIndex The row index of the goal position
     * @param goalColumnIndex The column index of the goal position
     * @param statusCode The status code after reading from the CSV-file
     */
    public CSVReaderResult(int[][] map, int[][] algorithmMap, int startRowIndex, int startColumnIndex, int goalRowIndex, int goalColumnIndex, int statusCode) {
        this.map = map;
        this.algorithmMap = algorithmMap;
        this.startRowIndex = startRowIndex;
        this.startColumnIndex = startColumnIndex;
        this.goalRowIndex = goalRowIndex;
        this.goalColumnIndex = goalColumnIndex;
        this.statusCode = statusCode;
    }

    /**
     * Returns the map
     * @return The map from the CSV-file
     */
    public int[][] getMap() {
        return map;
    }

    /**
     * Returns the algorithmMap
     * @return The initialized map for the path planning algorithm
     */
    public int[][] getAlgorithmMap() {
        return algorithmMap;
    }

    /**
     * Returns the startRowIndex
     * @return The row index of the start position
     */
    public int getStartRowIndex() {
        return startRowIndex;
    }

    /**
     * Returns the startColumnIndex
     * @return The column index of the start position
     */
    public int getStartColumnIndex() {
        return startColumnIndex;
    }

    /**
     * Returns the goalRowIndex
     * @return The row index of the goal position
     */
    public int getGoalRowIndex() {
        return goalRowIndex;
    }

    /**
     * Returns the goalColumnIndex
     * @return The column index of the goal position
     */
    public int getGoalColumnIndex() {
        return goalColumnIndex;
    }

    /**
     * Returns the statusCode
     * @return The status code after reading from the CSV-file
     */
    public int getStatusCode() {
        return statusCode;
    }

}
