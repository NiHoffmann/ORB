
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
