import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;

/**
 * PathReconstructor is the class that reconstructs a path from the results of a path planning algorithm
 */
public class PathReconstructor {
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
     * A set that includes the previous position for every visit
     */
    private final HashMap<String, String> previousPositions;

    /**
     * Constructor of the PathReconstructor class
     * @param startRowIndex The row index of the start position
     * @param startColumnIndex The column index of the start position
     * @param goalRowIndex The row index of the goal position
     * @param goalColumnIndex The column index of the goal position
     * @param previousPositions A set that includes the previous position for every visit
     */
    public PathReconstructor(int startRowIndex, int startColumnIndex, int goalRowIndex, int goalColumnIndex, HashMap<String, String> previousPositions) {
        this.startRowIndex = startRowIndex;
        this.startColumnIndex = startColumnIndex;
        this.goalRowIndex = goalRowIndex;
        this.goalColumnIndex = goalColumnIndex;
        this.previousPositions = previousPositions;
    }

    /**
     * Reconstructs the path of a path planning algorithm starting from the goal and looping through the previous positions
     * @return The list of path positions from start to goal
     */
    public int[][] reconstructPath() {
        // Initializes empty pathPositions and currentPosition
        List<int[]> pathPositions = new ArrayList<>();
        String currentPosition = goalRowIndex + "," + goalColumnIndex;
        int currentRowIndex = goalRowIndex;
        int currentColumnIndex = goalColumnIndex;

        // Loops through previousPositions until the start position is found
        while (!(currentRowIndex == startRowIndex && currentColumnIndex == startColumnIndex)) {
            pathPositions.add(new int[]{currentRowIndex, currentColumnIndex});
            currentPosition = previousPositions.get(currentPosition);

            // Transforms the currentPosition String to integers
            String[] parts = currentPosition.split(",");
            currentRowIndex = Integer.parseInt(parts[0]);
            currentColumnIndex = Integer.parseInt(parts[1]);
        }

        // Adds the start position as last element and reverts the pathPositions to make them start with the start position
        pathPositions.add(new int[]{startRowIndex, startColumnIndex});
        Collections.reverse(pathPositions);

        // Converts pathPositions to multidimensional array
        int[][] pathArray = new int[pathPositions.size()][2];
        for (int i = 0; i < pathPositions.size(); i++) {
            pathArray[i] = pathPositions.get(i);
        }

        return pathArray;
    }

}
