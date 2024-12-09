import java.util.HashMap;
import java.util.Map;

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
        // Calls Garbage Collector to clean memory
        System.gc();

        // Saves the startTime and starts the observation of the memory usage
        long startTime = System.nanoTime();
        long memoryBefore = Runtime.getRuntime().totalMemory() - Runtime.getRuntime().freeMemory();

        HashMap<String, String> test = new HashMap<>();
        test.put(goalRowIndex + "," + goalColumnIndex, startRowIndex + "," + startColumnIndex);
        System.out.println("test");
        PathReconstructor pathReconstructor = new PathReconstructor(startRowIndex, startColumnIndex, goalRowIndex, goalColumnIndex, test);
        int[][] path = pathReconstructor.reconstructPath();

        // The tracking of the memory usage is stopped
        long memoryAfter = Runtime.getRuntime().totalMemory() - Runtime.getRuntime().freeMemory();

        // The endTime gets saved and the computingTime in seconds is calculated by subtracting the startTime
        long endTime = System.nanoTime();
        double computingTime = (double)(endTime - startTime)/1_000_000_000.0;

        // The memory usage gets converted to MB
        double memoryUsage = (double)(memoryBefore - memoryAfter) / 1_048_576.0;

        System.out.println(computingTime);
        System.out.println(memoryUsage);
        //return new AlgorithmResult(algorithmMap, goalColumnIndex, goalColumnIndex, algorithmMap, goalColumnIndex, goalColumnIndex);
    }
}
