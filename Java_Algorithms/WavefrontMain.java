/**
 * WavefrontMain is the class that is the entry point for starting the Wavefront algorithm
 */
public class WavefrontMain {
    /**
     * Constructor of the WavefrontMain class
     * @param args The arguments needed for starting the Wavefront-algorithm
     *             args[0] The path towards the CSV-file with the map the Wavefront-algorithm is supposed to be applied to
     *             args[1] The path towards the JSON-file where the results of the Wavefront-algorithm are saved
     */
    public static void main(String[] args) {
        // Reads in the map, applies the Wavefront-algorithm to it and saves the results to a JSON-file
        CSVReader csvReader = new CSVReader(args[0]);
        CSVReaderResult csvResult = csvReader.readFromCSV();
        Wavefront wavefront = new Wavefront(csvResult.getMap(), csvResult.getAlgorithmMap(), csvResult.getStartRowIndex(), csvResult.getStartColumnIndex(), csvResult.getGoalRowIndex(), csvResult.getGoalColumnIndex(), csvResult.getStatusCode());
        AlgorithmResult algorithmResult = wavefront.applyWavefront();
        JSONWriter jsonWriter = new JSONWriter(args[1], algorithmResult);
        jsonWriter.writeToJSON();
    }
}
