
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
