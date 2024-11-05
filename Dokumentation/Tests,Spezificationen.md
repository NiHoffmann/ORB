## Inhaltsverzeichnis
- [1. Testen in der Code::Blocks Umgebung](#1-testen-in-der-codeblocks-umgebung)
    - [1.1. Windows Test-Spezifikation](#11-windows-test-spezifikation)
    - [1.2. Windows Tests](#12-windows-tests)
- [2. Testen der ORB-Firmware](#2-testen-der-orb-firmware)
    - [2.1. Firmware Test Spezifikation](#21-firmware-test-spezifikation)
    - [2.2. Firmware Tests](#22-firmware-tests)
        - [2.2.1. Modules](#221-modules)
            - [2.2.1.1. Devices - Servo Module](#2211-devices---servo-module)
            - [2.2.1.2. Devices - Motor Module](#2212-devices---motor-module)
            - [2.2.1.3. Devices - Sensor Module](#2213-devices---sensor-module)
            - [2.2.1.4. Memory](#2214-memory)
            - [2.2.1.5. Time](#2215-time)
        - [2.2.2. Builtin Functions](#222-builtin-functions)
        - [2.2.3. Error-Tests](#223-error-tests)

### 1. Testen in der Code::Blocks Umgebung
Im Ordner `"<PATH_TO_ORB>ORB-Python\Program"` befindet sich eine Datei program.py. Dies kann verwendet werden um Funktionalitäten des Micropython-VM zu Testen. Diese Datei wird von der ORB-Windows-Application geladen und ausgeführt. Hier kann man wenn man von der Code::Blocks umgebung aus Testen möchte sein Programm umschreiben.

Der Python-Api dokumentation (sphinx-docs) ist zu entnehmen welche Funktionen bereit gestellt werden. Die erwarteten Ausgaben der Funktionen sind den Mockup-Dateien zu entnehmen. 
Grundsätzlich reicht hier einfaches Testen, für diesen zweck habe ich ein Test-Directory erstellt (`ORB-Python/program/tests/).

Die Datei `_execute_(drag_test_here).bat` ist ein kleines hilfs-script um Tests einfach ausführen zu können. (Wichtig hier das Code::Blocks Projekt muss einmal gebaut werden.)

#### 1.1. Windows Test-Spezifikation  

- Es sollen alle Funktionen der Python-Api getested werden. Es sollen alle Funktionen einmal ausgeführt werden. Anstelle der tatsächlichen ORB-Funktionalität ist bei:
    - Jedes Modul bekommt einen eigener Test in dem die Folgenden Punkte getestet werden.
    - Funktionen mit Rückgabe-Wert: der befehl print(`<funktions aufruf>`) zu verdenden.
    - Funktionen mit Liste als Rückgabe-Wert: Hier wird zusätlich auf die Listen Elemente zugegriffen.
    - Er soll die Korrekte verwendung von Objekt-Referenzen (wie z.b. bei Motoren überprüft werden)
    - Für jeden Test kann er Exit-Status am ende der Ausführung überprüft werden.
- Es sollen die Funktionen der Python-VM getestet werden.
    - Exception-Handling und Exception-Exit-Status
    - Interrupt-Programm und Interrupt-Exit-Status
    - Der Normal-Exit-Status ist implizit in den anderen Tests

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
Dieser Fehler überprüft ob Micropython-Exceptions als solche erkannt werden (Exit-Code 1). Zusätzliche sollte die Art der Exception ausgegeben werden. In diesem Fall `Exception`. 
</td>
</tr>

<tr>
<td style="width: 10%;">
test.memFault
</td>
<td style="width: 40%;">
Diese Script Test überprüft ob Memory-Allocation-Errors d.h. ungenügender Speicher im Micropython-Heap. Die VM soll mit dem Exit-Code Exception(1) Beendet werde. Der Exit-Typ sollte einen memory-error anzeigen.
</td>
</tr>

<tr>
<td style="width: 10%;">
test.memory
</td>
<td style="width: 40%;">
Ein einfacher Test der Memory Funktionen. Es sollen alle Funktionen des Memory-Moduls verwendet werden.
</td>
</tr>

<tr>
<td style="width: 10%;">
test.monitor
</td>
<td style="width: 40%;">
Ein einfacher Test der Monitor Funktionen. Es sollen alle Funktionen des Monitor-Moduls verwendet werden.
</td>
</tr>

<tr>
<td style="width: 10%;">
test.motor
</td>
<td style="width: 40%;">
Ein einfacher Test der Motor Funktionen. Es sollen alle Funktionen des Motor-Moduls verwendet werden.
</td>
</tr>

<tr>
<td style="width: 10%;">
test.sensor
</td>
<td style="width: 40%;">
Ein einfacher Test der Sensor Funktionen. Es sollen alle Funktionen des Sensor-Moduls verwendet werden.
</td>
</tr>

<tr>
<td style="width: 10%;">
test.servo
</td>
<td style="width: 40%;">
Ein einfacher Test der Servo Funktionen. Es sollen alle Funktionen des Servo-Moduls verwendet werden.
</td>
</tr>

<tr>
<td style="width: 10%;">
test.time
</td>
<td style="width: 40%;">
Ein einfacher Test der Time Funktionen. Es sollen alle Funktionen des Time-Moduls verwendet werden.
</td>
</tr>

<tr>
<td style="width: 10%;">
test.userInterrupt
</td>
<td style="width: 40%;">
Testen des User-Interrupts. Der Exit-Status sollte 2 sein und aus dem Exit-Typ sollte hervor gehen, das ein User-Interrupt verwendet wurde.
</td>
</tr>

</table>

#### 2. Testen der ORB-Firmware

Es wurde ein Test-Programm für die ORB-Firmware erstellt. Dieses befindet sich unter [ORB-Python-Examples/Tests/Firmware/firmware_test.py](./../ORB-Python-Examples/Tests/Firmware/firmware_test.py). Das Test-Programm kann mithilfe des ORB-Monitor übertragen und ausgeführt werden. Dazu wird der Befehlt `orb-util -f firmware_test.py` verwendet. Das Test-Programm bietet eine Interaktive Oberfläche um alle Funktionen des Python-Interpreters zu testen. Das Test-Programm unterteilt durch eine Menü-Führung die einzelnen zu Testenden Abschnitte. 

#### 2.1. Firmware Test Spezification

Es soll die Programm-Übertragunge getestet werden. Dafür soll sowohl ein beliebiges C++, als auch ein Python Programm übertragen werden.

Es sollen alle Python-Funktionen getestet werden. Der Fokus ist hier auf Korrekter Parameter übergabe. Die Rückgabe werte von Funktionen werden auf dem Monitor ausgegeben. Hier sollen im Wesentlichen verschiedene Devices in den Verschiedenen möglichen Konfigurationen getestet werden. 

Es sollen die Exit-Status ausgaben und das Korrekte Handhaben von Exceptions überprüft werden. Dazu gehört das Ausgeben von Fehlermeldungen und die wieder ausführbarkeit der MP-VM nach einer Exception. Es soll auch der Hard-Fault-Handler getestet werden.

Es sollen die zusätzliche einebundenen Buildin-Funktionen überprüft werden. Für eingebundene Module reicht hier die verfügbarkeit, solange diese nicht im rahmen dieses Projektes eitstanden sind. So z.B. das Math-Modul.

#### 2.2. Firmware Tests
Die Firmware-Tests sind als Interaktive Tests gedacht. Es können alle Funktionen des ORB aufgerufen und ausgegeben werden. Schon durch die möglichkeit das Test-Programm zu erstellen ist die Funktions-Fähigkeit der ORB-Firmawre zu einem großen Teil vallidiert. 
Das restliche Testen, neben der erreichbarkeit der Funktionen, ist das Korrekte übergeben von Parametern. Um dies Testen zu können ,soll das Firmware-Test-Programm verwendert werden. Es bietet die möglichkeit an die Python-Funktionen zu Konfigurieren. Hier wurden alle Parameter für die möglichen Funktions-Aufrufe getetet. Im Folgenden sind zusätzliche Informationen bei dem Vorgehen der Tests.

#### 2.2.1. Modules

#### 2.2.1.1. Devices - Servo Module
Der Servo-Module Test erlaubt es einem Nutzer alle Parameter eines Servo-Objektes einzustellen. Es ist der Port, die  Geschwindigkeit und der Winkel einstellbar. Hier kann auch Optional mit einer LED getestet werden. Da ein Sero-Motor mithilfe eines PWM-Signales angesteuert wird. Es konnte Validiert werden, das der Servo-Motor konfiguriertbar ist. 

#### 2.2.1.2. Devices - Motor Module
Der Motor-Test ist in 3 Teile aufgeteilt: config, set, get. Dies sind die drei Funktionen welche durch ein Motor-Objekt verwendbar werden. Der erstellte Motor-Test erlaubt es einen Nutzer einen Lego-Motor vollständig zu Configurieren. Einstellungen wie kp,ki, etc. also Motor-Spezifische Configurationen sind hier für den LEGO-Motor eingestellt. Diese wurden in dem Line-Follower mit anderer onfiguration verwendet. Somit ist die Konfiguration für einen Lego-Motor und einen MakeBlock-Motor vorgenommen und überprüft worden. Diese sind in diesem Test nicht einstellbar. Alle anderen Funktionen ließen sich Vallidieren.

#### 2.2.1.3. Devices - Sensor Module
Der Sensor-Test ist in 3 Teile aufgeteilt: config, get, pre-configured. Während get und config Sensoren einstellbar machen. Also die Sensor-Objekt Funktionen abbilden, ist über pre-configured ein voreingestellter Sensor verfügbar. Hier wurde ein NXT-Light-Sensor Konfiguriert und durch läuft eine Handvoll Test-Funktions aufrufen und Konfigurationen. In diesem Test sollen verschiedene Sensoren an dem ORB angeschlossen und Konfiguriert werden. Es wurden Tests für MB-Color-Sensor, MB-Ultra-Sonic-Sensor, NXT-Light-Sensor und NXT-Touch-Sensor durchgeführt. Im wesentlichen wurde hier mit den verschiedenen Funktions-Parametern für Sensoren Konfiguriert und Vallidiert das diese Konfigurierbar sind. 

#### 2.2.1.2. Memory
Der Alternative Programmstart mit "Write Mem" kann verwendet werden um einen Wert in den Nutzer-Speicher zu schreiben.
Mit dem Programm-Start "Tests" kann unter 1.Module/2.Memory die getMemory- und clearMemory-Funktion getestet werden.
getMemory gibt dabei der in "Write Mem"-Geschriebenen wert aus. Clear Memory setz den Vollständigen Nutzer-Speicher zu xFF zurück. Darin inbegriffen den Vorher geschriebenen Wert. Die Funktionalität der Memory-Funktionen konnte sicher gesetellt werden.

#### 2.2.1.3. Time
| Test-Name  | Test-Vorgang | Verhalten |
|      -     |      -       |     -      |
|   Get Time |   Ausführen.  |   Es wird der Wert von getTime wiedergegeben. | 
|   wait     |   Ausführen.  | Der Nutzer wird darüber Informiert das 2 Sekunden gewartet wird. Danach wird "Finished" ausgegeben. |

#### 2.2.2. Builtin Funktions
| Test-Name  | Test-Vorgang | Verhalten | Notiz |
|     -       |     -    | -           |   -    |
|    Math     |     -    | Die erste Zeile dieses Tests solle ausgeben ob das Math-Module verfügbar ist. | |
|    Exit     |  Ausführen. | Die MP-VM stopt ihr ausführung. Auf dem ORB-Monitor wird ein System-Exit angezeigt. || 
|    Min/Max  | Ausführen. Mit Buttons werte auswähelen. | Auf dem Monitor wird das Minimum und Maximum der beiden Werte angezeigt |
|    getArg   | Ausführen. | Der Arg-Werte wird ausgegeben. Dies ist 0. |

#### 2.2.3. Error-Tests

| Test-Name  | Test-Vorgang | Verhalten | Notiz |
|     -       |     -    | -           |   -    |
|  Hard-Fault |  Starte einen Motor mit dem Motor-Test. Geh zurück zum "Main Menu". Danach Hard-Fault-Test Ausführen- | ORB-Firmware unterbricht ausführung. Alle 3 Status LEDs gehen an ORB-Firmware muss Resettet werden. Der zuvor gestartete Motor wird gestoppt. |  Der Hard-Fault ist schwer zu Produzieren, nach änderungen an der Firmware könnte dieser Test nicht mehr Funktionieren. Die Aktuelle Firmware-Version produziert diesen jedoch Korrekt. Es wurde mit der Hardware-Version 0.22 getestet. |
| Memory-Error | Ausführen | Die MP-VM unterbricht die Ausführung. Es wird eine Exception-Message auf dem ORB-Monitor ausgegeben. Diese wießt auf einen Memory-Error hin.| |
| User-Exception | Ausführen | Die MP-VM unterbricht die Ausführung. Es wird eine Exception-Message auf dem ORB-Monitor ausgegeben. Diese wießt auf eine User-Exception hin. | |
| User-Interrupt | Ausführen, danach muss der Stop-Button des ORB gedrückt werden. | Die MP-VM unterbricht die Ausführung. Es wird der 'User Interrupt' auf dem ORB-Monitor ausgegeben.| |