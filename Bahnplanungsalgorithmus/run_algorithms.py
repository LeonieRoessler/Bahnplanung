import subprocess
import sys
import os

def run_program(command, description):
    try:
        print(f"Starte {description}...")
        result = subprocess.run(command, shell=True, check=True, capture_output=True, text=True)
        print(f"{description} erfolgreich ausgeführt.")
        if result.stdout:
            print(f"Ausgabe:\n{result.stdout}")
        if result.stderr:
            print(f"Fehlerausgabe:\n{result.stderr}")
    except subprocess.CalledProcessError as e:
        print(f"Fehler beim Ausfuehren von {description}:")
        print(f"Rueckgabecode: {e.returncode}")
        if e.stdout:
            print(f"Ausgabe:\n{e.stdout}")
        if e.stderr:
            print(f"Fehlerausgabe:\n{e.stderr}")
    except FileNotFoundError as e:
        print(f"Fehler: Datei oder Ordner nicht gefunden - {e.filename}")
    except Exception as e:
        print(f"Unbekannter Fehler: {str(e)}")

def main():
    # Map-Datei, die von deinem C++-Programm exportiert wurde
    input_map = "map.csv"
    
    # Prüfe, ob die Map-Datei existiert
    if not os.path.isfile(input_map):
        print(f"Fehler: Die Datei '{input_map}' wurde nicht gefunden.")
        return

    algorithms = sys.argv[1:]  # Alle Argumente ab dem zweiten Index (Index 0 ist der Skriptname)

    # Gib alle uebergebenen Algorithmen aus
    for alg in algorithms:
        name, language = alg.split(":")  # Trenne den Namen von der Sprache
        print(f"Algorithmus: {name}, Sprache: {language}")
        attribute = "none"
        # Befehl basierend auf dem Algorithmus und der Sprache
        if name == "Wavefront":
            script_name = "Wavefront"
        elif name == "Brushfire":
            script_name = "Brushfire"
        elif name == "AStarAP":
            script_name = "AStarAP"
        elif name == "AStarMH":
            script_name = "AStarMH"
        else:
            print(f"Unbekannter Algorithmus: {name}")
            continue
        
        # Bestimme den Pfad und den Befehl basierend auf der Sprache
        if language == "Python":
            folder = "Python_Algorithms"
            # Pruefe, ob der Ordner und die Datei existieren
            if name == "AStarAP":
                script_name = "A_star"
                attribute = "manhattan"
            elif name == "AStarMH":
                script_name = "A_star"
                attribute = "airplane"
            if not os.path.isfile(f"{folder}/{script_name}.py"):
                print(f"Fehler: Die Datei {folder}/{script_name}.py wurde nicht gefunden.")
                continue
            if attribute != "none":
                command = f"python3 {folder}/{script_name}.py {input_map} result_{name}_Python.json {attribute}"
            else:
                command = f"python3 {folder}/{script_name}.py {input_map} result_{name}_Python.json"

        elif language == "Java":
            folder = "Java_Algorithms"
            if name == "Wavefront":
                script_name = "WavefrontMain"
            elif name == "Brushfire":
                script_name = "BrushfireMain"
            elif name == "AStarAP":
                script_name = "AStarMain"
                attribute = "manhattan"
            elif name == "AStarMH":
                script_name = "AStarMain"
                attribute = "airplane"
            
            # Finde alle .java-Dateien im Ordner
            java_files = [f for f in os.listdir(folder) if f.endswith('.java')]
            
            if not java_files:
                print(f"Fehler: Es wurden keine Java-Dateien im Ordner {folder} gefunden.")
                continue
            
            # Kompiliere alle .java-Dateien
            compile_command = f"javac -cp {folder}/lib/gson-2.11.0.jar -d {folder} {folder}/*.java && echo 'Compiled Successfully'"
            compile_process = subprocess.run(compile_command, shell=True)
            
            if compile_process.returncode != 0:
                print(f"Fehler beim Kompilieren der Dateien im Ordner {folder}.")
                continue

            # Nachdem die Dateien erfolgreich kompiliert wurden, prüfe, ob die .class-Dateien existieren
            class_files = [f"{folder}/{f.replace('.java', '.class')}" for f in java_files]
            
            # Prüfe, ob alle .class-Dateien existieren
            for class_file in class_files:
                if not os.path.isfile(class_file):
                    print(f"Fehler: Die kompilierte Datei {class_file} wurde nicht gefunden.")
                    continue
            
            # Führe das kompilierte Java-Programm aus
            class_name = script_name 
            if attribute != "none":
                command = f"java -cp {folder};{folder}/lib/gson-2.11.0.jar {class_name} {input_map} result_{name}_Java.json {attribute}"
            else:
                command = f"java -cp {folder};{folder}/lib/gson-2.11.0.jar {class_name} {input_map} result_{name}_Java.json"
            run_process = subprocess.run(command, shell=True)
            
            if run_process.returncode != 0:
                print(f"Fehler beim Ausführen des Java-Programms {class_name}.")
            else:
                print(f"Das Java-Programm {class_name} wurde erfolgreich ausgeführt.")
        elif language == "Cpp":
            folder = "Cpp_Algorithms"
            if name == "AStarAP":
                script_name = "A_Star"
                attribute = "manhattan"
            elif name == "AStarMH":
                script_name = "A_Star"
                attribute = "airplane"

            # Prüfe, ob die ausführbare Datei existiert
            if not os.path.isfile(f"{folder}/{script_name}.cpp"):
                print(f"Fehler: Die Datei {folder}/{script_name}.cpp wurde nicht gefunden.")
                continue
            
            compile_command = f"g++ -o {folder}\\{script_name}.exe {folder}\\{script_name}.cpp -I{folder}\\include"
            exe_path = os.path.abspath(f"{folder}\\{name}.exe")
            # Versuche, das C++-Programm zu kompilieren
            try:
                print(f"Kompliliere das Programm: {compile_command}")
                subprocess.run(compile_command, shell=True, check=True)  # check=True sorgt für eine Ausnahme bei Fehlern
                print("Kompilierung erfolgreich!")
            except subprocess.CalledProcessError as e:
                print(f"Fehler beim Kompilieren: {e}")
                return
            if attribute != "none":
                command = f"{folder}\\{script_name}.exe {input_map} result_{name}_Cpp.json {attribute}"
            else:
                command = f"{folder}\\{script_name}.exe {input_map} result_{name}_Cpp.json"
        else:
            print(f"Unbekannte Sprache: {language}")
            continue
        
        print(f"Auszuführender Befehl: {command}")  # Ausgabe des generierten Befehls
        # Fuehre das Programm aus
        run_program(command, f"{name} ({language})")

    print("Alle Programme wurden ausgeführt.")

if __name__ == "__main__":
    main()
