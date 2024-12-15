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
        
        # Befehl basierend auf dem Algorithmus und der Sprache
        if name == "Wavefront":
            script_name = "wavefront"
        elif name == "Bushfire":
            script_name = "bushfire"
        elif name == "AStar":
            script_name = "a_star"
        else:
            print(f"Unbekannter Algorithmus: {name}")
            continue
        
        # Bestimme den Pfad und den Befehl basierend auf der Sprache
        if language == "Python":
            folder = "Python_Algorithms"
            # Prüfe, ob der Ordner und die Datei existieren
            if not os.path.isfile(f"{folder}/{script_name}.py"):
                print(f"Fehler: Die Datei {folder}/{script_name}.py wurde nicht gefunden.")
                continue
            command = f"python3 {folder}/{script_name}.py {input_map} result_{name}_Python.json"
        elif language == "Java":
            folder = "Java_Algorithms"
            # Prüfe, ob die .jar-Datei existiert
            if not os.path.isfile(f"{folder}/{script_name}.jar"):
                print(f"Fehler: Die Datei {folder}/{script_name}.jar wurde nicht gefunden.")
                continue
            command = f"java -jar {folder}/{script_name}.jar {input_map} result_{name}_Java.json"
        elif language == "Cpp":
            folder = "Cpp_Algorithms"
            # Prüfe, ob die ausführbare Datei existiert
            if not os.path.isfile(f"{folder}/{script_name}"):
                print(f"Fehler: Die Datei {folder}/{script_name} wurde nicht gefunden.")
                continue
            command = f"./{folder}/{script_name} {input_map} result_{name}_Cpp.json"
        else:
            print(f"Unbekannte Sprache: {language}")
            continue
        
        print(f"Auszuführender Befehl: {command}")  # Ausgabe des generierten Befehls
        # Fuehre das Programm aus
        run_program(command, f"{name} ({language})")

    print("Alle Programme wurden ausgeführt.")

if __name__ == "__main__":
    main()
