# Arbeitsplan

- [x] **1. Micropython Embed Port unter Windows**
    - [x] 1.1. Code:Blocks Umgebung einrichten  
    - [x] 1.2. Compilieren und Laden einer MPY-Binär-Datei  
    - [x] 1.3. Erstes Modul Registrieren & Mockup-Erstellen  
    - [x] 1.4. Erster Entwurf der Micropython-VM Schnittstellen
    - [x] 1.5. User-Interrupt Integrieren & Thread-Safety Testen/Analysieren  
    - [x] 1.6. Micropython- & Program-Compile-Prozess in den Code::Blocks Build Prozess einbauen  
    > Nach diesem Schritt ist das Windows Grundgerüst gebaut. 
    - [x] 1.7. Vollständiges Umestzen der ORB-Funktions-Module & Mockups 
    - [x] 1.8. Python-Api Dokumentation Schreiben
    - [x] 1.9. Program-Compile-Prozess in Code;Blocks Build Prozess einbauen 
    > Ab diesen Punkt sollten alle Funktionen unter Windows Testbar sein.
    - [x] 1.10. Erstellen der Windows-Test-Spezifiakation
    - [x] 1.11. Testen der Micropython-Funktionalitäten nach Spezifiakation

- [ ] **2. Integration in EMBitz Projekt**
    - [x] 2.1. Ausführen der VM auf einem Microcontroller (Mit MPY-Binary in die Firmware eingebaut - d.h. als uint_8t array).
    > Hier kann das ORB-Firmware Projekt als grundgerüst genutzt werden, 
    ohne das verwendet der eigentlichen ORB-Funktionen. Funktionen wie usrLed.set(1) können erst einmal als Platzhalter in die ORB-C-Interfaces eingebaut werden.
    - [x] 2.2. Übertragen und Laden der Programmes aus Flash-Speicher
    > Verwenden des Entwicklungs-Gerüstes (2.1.).
    - [x] 2.3. Umsetzen der Tatsächlichen C-Interface-Klassen
    - [x] 2.4. Erstellen der Python-Task. Und ausführen als solche.
    - [x] 2.5. Entwicklungs Gerüst erweitern um USB-Update 
    >Für ORB-Monitor-Kommunikation.
    - [ ] 2.6. Testen & Dokumentieren der ORB-Python-Funktionalitäten
    - [ ] 2.7. Erster Entwurf der Micropython-VM-Api Dokumentation
    > Festgehalten im Sphinx-Api-Doc

- [ ] **3. Vollständige Firmware Integration** 
    - [x] 3.1. Wiederherstellen der ORB-Programm-Logik
    - [x] 3.2. Konflikte zwischen Tasks identifizieren & Dokumentieren
    > Hier z.b. das UserInterface schaltet Motoren aus wenn keien AppTask läuft und der Monitor nicht überträt das diese eingeschaltet seien sollen.
    - [x] 3.3. Konflike Lösen & Dokumentieren
    - [ ] 3.4. Kontrollfluss Dokumentation (aus 3.2. & 3.3.) erstellen.
    >Welcher Button startet Python-Vm welcher AppTask, etc. diser schritt ist abhängig von vorher getroffenen Designentscheidungen.
    - [x] 3.5. Testen & Dokumentieren der ORB-Python-Funktionalitäten im zusammenhang mit Kontrollfluss und ORB-Firmware Kompatibilität.
    - [ ] 3.6. Micropython-VM-Api Dokumentation nach bessern
    > Das hier könnte eine eigene Einheit sein, bis zu diesem Punkt wurde erste einem mal nur der Fokus darauf gesetzt das die PythonTask ihre Aufgabe erfüllen kann. Jetzt muss das Zusammenspiel mit den anderen Komponenten untersucht werden (hier also vllt. `4. Analyse des zusammenspiels zwischen bestehender ORB-Firmware-Architektur & PythonTask`)
    - [ ] 3.7. Analyse der ORB-Python Anforderungen  
        - [ ] 3.7.1. System Anforderungen an die PythonTask
            - [ ] 3.7.1.1. Wie groß kann / sollte die HeapSize sein.
            - [ ] 3.7.1.2. Wie große kann / sollte der PythonTask Stack sein.
            - [ ] 3.7.1.3. Welche Speicherblöcke bekommt die PythonTask um die MPY-Binary abzulegen
