import subprocess
import sys

def run_program(command, description):
    try:
        print(f"Starte {description}...")
        result = subprocess.run(command, shell=True, check=True, capture_output=True, text=True)
        print(f"{description} erfolgreich ausgeführt.")
        print(f"Ausgabe:\n{result.stdout}")
    except subprocess.CalledProcessError as e:
        print(f"Fehler beim Ausfuehren von {description}:")
        print(f"Rueckgabecode: {e.returncode}")
        print(f"Fehlerausgabe:\n{e.stderr}")

def main():
    # Map-Datei, die von deinem C++-Programm exportiert wurde
    input_map = "map.csv"
    
    # Pfade und Befehle zu den Programmen
    python_command = f"python3 path_planning.py {input_map} result_python.csv"
    java_command = f"java -jar PathPlanning.jar {input_map} result_java.csv"
    cpp_command = f"./path_planning_cpp {input_map} result_cpp.csv"  # Beispiel fuer ein C++-Programm

    # Python-Skript ausfuehren
    run_program(python_command, "Python-Algorithmus")
    
    # Java-Programm ausfuehren
    run_program(java_command, "Java-Algorithmus")
    
    # C++-Programm ausfuehren (optional)
    run_program(cpp_command, "C++-Algorithmus")

    print("Alle Programme wurden ausgefuehrt.")

if __name__ == "__main__":
    main()
