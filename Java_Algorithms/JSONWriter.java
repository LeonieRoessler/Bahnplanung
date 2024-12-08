import java.io.FileWriter;
import java.io.IOException;
import java.io.Writer;

import com.google.gson.Gson;
import com.google.gson.GsonBuilder;

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
        try {
            System.out.println("JSON");
            Writer writer = new FileWriter(jsonPath);
            Gson gson = new GsonBuilder().setPrettyPrinting().create();
            gson.toJson(algorithmResult, writer);
            writer.flush();
            writer.close();
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
