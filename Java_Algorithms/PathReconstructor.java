import java.util.*;

/**
 * PathReconstructor is the class that reconstructs a path from the results of a path planning algorithm
 */
public class PathReconstructor {
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
     * A set that includes the previous position for every visit
     */
    private HashMap<String, String> previousPositions;

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
     */
    public int[][] reconstructPath() {
        // Initializes empty pathPositions and currentPosition
        List<int[]> pathPositions = new ArrayList<>();
        String currentPosition = goalRowIndex + "," + goalColumnIndex;
        int currentRowIndex = goalRowIndex;
        int currentColumnIndex = goalColumnIndex;

        // Loops through previousPositions until the start position is found
        while ((currentRowIndex != startRowIndex) && (currentColumnIndex != startColumnIndex)) {
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

    public HashMap<String, String> getPreviousPositions() {
        return previousPositions;
    }

    public void setPreviousPositions(HashMap<String, String> previousPositions) {
        this.previousPositions = previousPositions;
    }
}
