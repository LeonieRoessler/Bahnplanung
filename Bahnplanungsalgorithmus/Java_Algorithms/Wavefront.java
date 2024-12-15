public class Wavefront {

    private static final int[][] DIRECTIONS = {
        {-1, 0}, {1, 0}, {0, -1}, {0, 1}
    };

    private int[][] map;
    private int[][] algorithmMap;
    private int startRowIndex;
    private int startColumnIndex;
    private int goalRowIndex;
    private int goalColumnIndex;
    private int statusCode;

    private Wavefront (int[][] map, int[][] algorithmMap, int startRowIndex, int startColumnIndex, int goalRowIndex, int goalColumnIndex, int statusCode) {
        this.map = map;
        this.algorithmMap = algorithmMap;
        this.startRowIndex = startRowIndex;
        this.startColumnIndex = startColumnIndex;
        this.goalRowIndex = goalRowIndex;
        this.goalColumnIndex = goalColumnIndex;
        this.statusCode = statusCode;
    }

    public static void main(String[] args) {
        CSVReader csvReader = new CSVReader(args[0]);
        CSVReaderResult csvResult = csvReader.readFromCSV();
        Wavefront wavefront = new Wavefront(csvResult.getMap(), csvResult.getAlgorithmMap(), csvResult.getStartRowIndex(), csvResult.getStartColumnIndex(), csvResult.getGoalRowIndex(), csvResult.getGoalColumnIndex(), csvResult.getStatusCode());
        wavefront.applyWavefront();
        JSONWriter jsonWriter = new JSONWriter(args[1], new AlgorithmResult(csvResult.getMap(), 0, 0, csvResult.getMap(), 0, 0));
        jsonWriter.writeToJSON();
    }

    private void applyWavefront() {
        System.out.println("test");
        //return new AlgorithmResult(algorithmMap, goalColumnIndex, goalColumnIndex, algorithmMap, goalColumnIndex, goalColumnIndex);
    }
}
