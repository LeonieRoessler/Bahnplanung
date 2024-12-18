import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Scanner;

/**
* CSVReader is the class that reads a map from a CSV-file and returns the result in the form of a list of lists of integers
*/
public class CSVReader {
    /**
    * The path to the CSV-file
    */
    private final String csvPath;

    /**
    * The map in form of a multidimensional array
    */
    private int[][] map;

    /**
    * The prepared map in form of a multidimensional array that can be overwritten by the path planning algorithms to show how they work
    */
    private int[][] algorithmMap;

    /**
    * The start row index on the map for the path planning algorithm
    */
    private int startRowIndex;

    /**
    * The start column index on the map for the path planning algorithm
    */
    private int startColumnIndex;

    /**
    * The goal row index on the map for the path planning algorithm
    */
    private int goalRowIndex;

    /**
    * The goal column index on the map for the path planning algorithm
    */
    private int goalColumnIndex;

    /**
    * A status code to represent success or a specific error type when reading the CSV-file
    */
    private int statusCode;

    /**
     * Constructor of the CSVReader class
     * @param csvPath The path to the CSV-file
     */
    public CSVReader(String csvPath) {
        this.csvPath = csvPath;
        this.map = null;
        this.algorithmMap = null;
        this.startRowIndex = -1;
        this.startColumnIndex = -1;
        this.goalRowIndex = -1;
        this.goalColumnIndex = -1;
        this.statusCode = -1;
    }

    /**
     * Reads a map from a CSV-file and returns the results
     * @return The results of reading a map and initializing the map for the path planning algorithm in form of a CSVReaderResult
     */
    public CSVReaderResult readFromCSV() {
        try (Scanner scanner = new Scanner(new File(this.csvPath))) {
            // Reads CSV-file from csvPath and transforms it into the form of multidimensional array
            ArrayList<int[]> dataList = new ArrayList<>();
            while (scanner.hasNextLine()) {
                String line = scanner.nextLine();
                String[] values = line.split(",");

                int[] row = new int[values.length];
                for (int column_index = 0; column_index < values.length; column_index++) {
                    row[column_index] = Integer.parseInt(values[column_index].trim());
                }
                dataList.add(row);
            }

            map = new int[dataList.size()][];
            for (int row_index = 0; row_index < dataList.size(); row_index++) {
                map[row_index] = dataList.get(row_index);
            }

            // Calls method to initialize the map for the path planning algorithm
            initializeAlgorithmMap();
            return new CSVReaderResult(this.map, this.algorithmMap, this.startRowIndex, this.startColumnIndex, this.goalRowIndex, this.goalColumnIndex, statusCode);

        // Returns corresponding error codes in case of errors
        } catch (FileNotFoundException e) {
            return new CSVReaderResult(null, null, -1, -1, -1, -1, 504);
        } catch (NumberFormatException e) {
            return new CSVReaderResult(null, null, -1, -1, -1, -1, 500);
        } catch (Exception e) {
            return new CSVReaderResult(null, null, -1, -1, -1, -1, 509);
        }
    }

    /**
     * Initializes a map for a path planning algorithm and searches for start and end positions
     */
    private void initializeAlgorithmMap() {
        // Initializes algorithmMap with same number of rows and columns as map and sets all values to -1 (not visited)
        algorithmMap = new int[map.length][];
        for (int row_index = 0; row_index < map.length; row_index++) {
            algorithmMap[row_index] = new int[map[row_index].length];
            Arrays.fill(algorithmMap[row_index], -1);
        }

        // Searches for start position (2) and goal position (3) to save and return them
        for (int rowIndex = 0; rowIndex < map.length; rowIndex++) {
            for (int columnIndex = 0; columnIndex < map[rowIndex].length; columnIndex++) {
                int value = map[rowIndex][columnIndex];
                if (value == 2) {
                    // Checks if more than one start position was accidentally assigned and sets the corresponding error code in this case and exits the method
                    if (startRowIndex != -1 || startColumnIndex != -1) {
                        algorithmMap = new int[][]{};
                        startRowIndex = -1;
                        startColumnIndex = -1;
                        goalRowIndex = -1;
                        goalColumnIndex = -1;
                        statusCode = 422;
                        return;
                    }
                    startRowIndex = rowIndex;
                    startColumnIndex = columnIndex;
                } else if (value == 3) {
                    // Checks if more than one goal position was accidentally assigned and sets the corresponding error code in this case and exits the method
                    if (goalRowIndex != -1 || goalColumnIndex != -1) {
                        algorithmMap = new int[][]{};
                        startRowIndex = -1;
                        startColumnIndex = -1;
                        goalRowIndex = -1;
                        goalColumnIndex = -1;
                        statusCode = 423;
                        return;
                    }
                    goalRowIndex = rowIndex;
                    goalColumnIndex = columnIndex;
                }
            }
        }

        // Checks if no start position was assigned and returns the corresponding error code in this case
        if (startRowIndex == -1 || startColumnIndex == -1){
            algorithmMap = new int[][]{};
            startRowIndex = -1;
            startColumnIndex = -1;
            goalRowIndex = -1;
            goalColumnIndex = -1;
            statusCode = 402;
            return;
        }

        // Checks if no goal position was assigned and returns the corresponding error code in this case
        if (goalRowIndex == -1 || goalColumnIndex == -1){
            algorithmMap = new int[][]{};
            startRowIndex = -1;
            startColumnIndex = -1;
            goalRowIndex = -1;
            goalColumnIndex = -1;
            statusCode = 403;
            return;
        }
        statusCode = 200;
    }
}
