/**
 * CSVReaderResult is the class that represents the result after reading a map from a CSV-file and initializing the map for a path planning algorithm
 */
public class CSVReaderResult {
    /**
     * The map from the CSV-file
     */
    private int[][] map;

    /**
     * The initialized map for the path planning algorithm
     */
    private int[][] algorithmMap;

    /**
     * The row index of the start position
     */
    private int startRowIndex;

    /**
     * The column index of the start position
     */
    private int startColumnIndex;

    /**
     * The row index of the goal position
     */
    private int goalRowIndex;

    /**
     * The column index of the goal position
     */
    private int goalColumnIndex;

    /**
     * The status code after reading from the CSV-file
     */
    private int statusCode;

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

    public int[][] getMap() {
        return map;
    }

    public void setMap(int[][] map) {
        this.map = map;
    }

    public int[][] getAlgorithmMap() {
        return algorithmMap;
    }

    public void setAlgorithmMap(int[][] algorithmMap) {
        this.algorithmMap = algorithmMap;
    }

    public int getStartRowIndex() {
        return startRowIndex;
    }

    public void setStartRowIndex(int startRowIndex) {
        this.startRowIndex = startRowIndex;
    }

    public int getStartColumnIndex() {
        return startColumnIndex;
    }

    public void setStartColumnIndex(int startColumnIndex) {
        this.startColumnIndex = startColumnIndex;
    }

    public int getGoalRowIndex() {
        return goalRowIndex;
    }

    public void setGoalRowIndex(int goalRowIndex) {
        this.goalRowIndex = goalRowIndex;
    }

    public int getGoalColumnIndex() {
        return goalColumnIndex;
    }

    public void setGoalColumnIndex(int goalColumnIndex) {
        this.goalColumnIndex = goalColumnIndex;
    }

    public int getStatusCode() {
        return statusCode;
    }

    public void setStatusCode(int statusCode) {
        this.statusCode = statusCode;
    }
}
