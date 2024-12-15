public class CSVReaderResult {
    private int[][] map;
    private int[][] algorithmMap;
    private int startRowIndex;
    private int startColumnIndex;
    private int goalRowIndex;
    private int goalColumnIndex;
    private int statusCode;

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
