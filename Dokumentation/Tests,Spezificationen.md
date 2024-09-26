## Inhaltsverzeichnis
- [1. Testen in der Code::Blocks Umgebung](#1-testen-in-der-codeblocks-umgebung)

### 1. Testen in der Code::Blocks Umgebung
Im Ordner `"<PATH_TO_ORB>ORB-Python\Program"` befindet sich eine Datei program.py. Dies kann verwendet werden um Funktionalitäten des Micropython-VM zu Testen. Diese Datei wird von der ORB-Windows-Application geladen und ausgeführt. Hier kann man wenn man von der Code::Blocks umgebung aus Testen möchte sein Programm umschreiben.

Der Python-Api dokumentation (sphinx-docs) ist zu entnehmen welche Funktionen bereit gestellt werden. Die erwarteten Ausgaben der Funktionen sind den Mockup-Dateien zu entnehmen. 
Grundsätzlich reicht hier einfaches Testen, für diesen zweck habe ich ein Test-Directory erstellt (`ORB-Python/program/tests/).

Die Datei `_execute_(drag_test_here).bat` ist ein kleines hilfs-script um Tests einfach ausführen zu können. (Wichtig hier das Code::Blocks Projekt muss einmal gebaut werden.)

#### 1.1. Windows Test-Spezifikation  

- Es sollen alle Funktionen der Python-Api getested werden. Es sollen alle Funktionen einmal ausgeführt werden. Anstelle der tatsächlichen ORB-Funktionalität ist bei:
    - Jedes Modul bekommt einen erigenen Test in dem die Folgenden Punkte getestet werden.
    - Funktionen mit Rückgabe-Wert: der befehl print(`<funktions aufruf>`) zu verdenden.
    - Funktionen mit Liste als Rückgabe-Wert: Hier wird zusätlich auf die Listen Elemente zugegriffen.
    - Er soll die Korrekte verwendung von Objekt-Referenzen (wie z.b. bei Motoren überprüft werden)
    - Für jeden Test kann er Exit-Status am ende der Ausführung überprüft werden.
- Es sollen die Funktionen der Python-VM getestet werden.
    - Exception-Handling
    - GC-Abort //dafür muss ich mir noch überlegen wie man es am besten testen kann
    - Interrupt-Programm

#### 1.2. Windows Tests 



<table>

<tr>
<td style="width: 10%;">
test.compilationError
</td>
<td style="width: 40%;">
Diese test ist für das Compile-Script. Er sollte einen Value-Error werfen. Zusätzlich kann man versuchen dieses Scrips als das `program.py` in code::blocks verwendet. Compiliert man die ORB-Firmware sollte der Build-Prozess mit entsprechenden Error abgebrochen werden.
</td>
</tr>

<tr>
<td style="width: 10%;">
test.exception
</td>
<td style="width: 40%;">
Dieser Fehler überprüft ob Micropython-Exceptions als Normale programm beendung erkannt werden. Zusätzliche sollte die Art der Exception al `Exception Type` ausgegeben werden. 
</td>
</tr>

<tr>
<td style="width: 10%;">
test.memFault
</td>
<td style="width: 40%;">
Diese Script Test überprüft ob Memory-Allocation-Errors d.h. ungenügender Speicher im Micropython-Heap. Die VM sollte normal beended werden Exit-Status: 0. Der Exit-Typ sollte einen memory-error anzeigen.
</td>
</tr>

<tr>
<td style="width: 10%;">
test.memory
</td>
<td style="width: 40%;">
Testen des Vollständigen Funktions-Umgeangs des Memory-Moduls.
</td>
</tr>

<tr>
<td style="width: 10%;">
test.monitor
</td>
<td style="width: 40%;">
Testen des Vollständigen Funktions-Umgeangs des Monitor-Moduls.
</td>
</tr>

<tr>
<td style="width: 10%;">
test.motor
</td>
<td style="width: 40%;">
Testen des Vollständigen Funktions-Umgeangs des Motor-Moduls.//ergänzen geteilte-objecte etc.
</td>
</tr>

<tr>
<td style="width: 10%;">
test.sensor
</td>
<td style="width: 40%;">
Testen des Vollständigen Funktions-Umgeangs des Sensor-Moduls.
</td>
</tr>

<tr>
<td style="width: 10%;">
test.servo
</td>
<td style="width: 40%;">
Testen des Vollständigen Funktions-Umgeangs des Servo-Moduls.
</td>
</tr>

<tr>
<td style="width: 10%;">
test.time
</td>
<td style="width: 40%;">
Testen des Vollständigen Funktions-Umgeangs des Time-Moduls.
</td>
</tr>

<tr>
<td style="width: 10%;">
test.userInterrupt
</td>
<td style="width: 40%;">
Testen des User-Interrupts. Der Exit-Status sollte 2 sein und aus dem Exit-Typ sollte heervor gehen das ein User-Interrupt verwendet wurde.
</td>
</tr>

</table>