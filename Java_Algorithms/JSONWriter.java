import java.io.FileWriter;
import java.io.IOException;
import java.util.Arrays;
import java.util.HashMap;
import java.util.LinkedHashMap;
import java.util.Map;

/**
* JSONWriter is the class that writes the results of a path planning algorithm to a JSON-file
*/
public class JSONWriter {
    /**
     * The path to the destination of the JSON-File including its name
     */
    private String jsonPath;

    /**
     * The results of the path planning algorithm in form of an AlgorithmResult
     */
    private AlgorithmResult algorithmResult;

    /**
     * Constructor of the JSONWriter class
     * @param jsonPath The path to the destination of the JSON-File including its name
     * @param algorithmResult The results of the path planning algorithm in form of an AlgorithmResult
     */
    public JSONWriter(String jsonPath, AlgorithmResult algorithmResult) {
        this.jsonPath = jsonPath;
        this.algorithmResult = algorithmResult;
    }

    /**
     * Writes the results of a path planning algorithm to a JSON-file
     */
    public void writeToJSON() {
        Map<String, Object> data = new LinkedHashMap<>();
        data.put("algorithm_map", algorithmResult.getAlgorithmMap());
        data.put("status_code", algorithmResult.getStatusCode());
        data.put("path_length", algorithmResult.getPathLength());
        data.put("path", algorithmResult.getPathPositions());
        data.put("computing_time", algorithmResult.getComputingTime());
        data.put("memory_usage", algorithmResult.getMemoryUsage());

        String indent = "   ";
        StringBuilder json = new StringBuilder();
        json.append("{\n");

        int i = 0;
        for (Map.Entry<String, Object> entry : data.entrySet()) {
            json.append(indent).append("\"").append(entry.getKey()).append("\": ");

            if (entry.getValue() instanceof String) {
                json.append("\"").append(entry.getValue()).append("\"");
            } else {
                json.append(entry.getValue());
            }

            if (i < data.size() - 1) {
                json.append(",\n");
            }
            i++;
        }

        json.append("\n}");
        String jsonObject = json.toString();

        try (FileWriter file = new FileWriter(jsonPath)) {
            file.write(jsonObject);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public String getJsonPath() {
        return jsonPath;
    }

    public void setJsonPath(String jsonPath) {
        this.jsonPath = jsonPath;
    }

    public AlgorithmResult getAlgorithmResult() {
        return algorithmResult;
    }

    public void setAlgorithmResult(AlgorithmResult algorithmResult) {
        this.algorithmResult = algorithmResult;
    }
}
