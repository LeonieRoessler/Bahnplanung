/**
 * AStarMain is the class that is the entry point for starting the A*-algorithm
 */
public class AStarMain {
    /**
     * Constructor of the AStarMain class
     * @param args The arguments needed for starting the A*-algorithm
     *             args[0] The path towards the CSV-file with the map the A*-algorithm is supposed to be applied to
     *             args[1] The path towards the JSON-file where the results of the A*-algorithm are saved
     *             args[2] The heuristic function type to calculate the distance towards the goal position
     */
    public static void main(String[] args) {
        // Checks if the given heuristic type is valid and lets the user know if that is not the case
        if(!(args[2].equalsIgnoreCase(AStarHeuristicType.MANHATTAN_DISTANCE.getValue())|| args[2].equalsIgnoreCase(AStarHeuristicType.AIRPLANE_DISTANCE.getValue()))) {
            System.out.println("Invalid heuristicType. Valid options are: '"+ AStarHeuristicType.MANHATTAN_DISTANCE.getValue() + "', '" + AStarHeuristicType.AIRPLANE_DISTANCE.getValue() + "'");
            System.exit(1);
        } else {
            // Reads in the map, applies the A*-algorithm to it and saves the results to a JSON-file
            CSVReader csvReader = new CSVReader(args[0]);
            CSVReaderResult csvResult = csvReader.readFromCSV();
            AStar aStar = new AStar(csvResult.getMap(), csvResult.getAlgorithmMap(), csvResult.getStartRowIndex(), csvResult.getStartColumnIndex(), csvResult.getGoalRowIndex(), csvResult.getGoalColumnIndex(), args[2], csvResult.getStatusCode());
            AlgorithmResult algorithmResult = aStar.applyAStar();
            JSONWriter jsonWriter = new JSONWriter(args[1], algorithmResult);
            jsonWriter.writeToJSON();
        }
    }
}
