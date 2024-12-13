/**
 * BrushfireMain is the class that is the entry point for starting the Brushfire algorithm
 */
public class BrushfireMain {
    /**
     * Constructor of the BrushfireMain class
     * @param args The arguments needed for starting the Brushfire-algorithm
     *             args[0] The path towards the CSV-file with the map the Brushfire-algorithm is supposed to be applied to
     *             args[1] The path towards the JSON-file where the results of the Brushfire-algorithm are saved
     */
    public static void main(String[] args) {
        // Reads in the map, applies the Brushfire-algorithm to it and saves the results to a JSON-file
        CSVReader csvReader = new CSVReader(args[0]);
        CSVReaderResult csvResult = csvReader.readFromCSV();
        Brushfire brushfire = new Brushfire(csvResult.getMap(), csvResult.getAlgorithmMap(), csvResult.getStartRowIndex(), csvResult.getStartColumnIndex(), csvResult.getGoalRowIndex(), csvResult.getGoalColumnIndex(), csvResult.getStatusCode());
        AlgorithmResult algorithmResult = brushfire.applyBrushfire();
        JSONWriter jsonWriter = new JSONWriter(args[1], algorithmResult);
        jsonWriter.writeToJSON();
    }
}
