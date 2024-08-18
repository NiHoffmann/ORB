# Inhaltsverzeichnis

1. [Warum den MicroPython Embed Port verwenden?](#warum-den-micropython-embed-port-verwenden)
2. [Aufsetzen des GitHub-Projekts](#aufsetzen-des-github-projekts)
3. [ORB-Modul hinzufügen](#orb-modul-hinzufügen)
   - [Anpassungen an micropython_embed.mk](#anpassungen-an-micropython_embedmk)
   - [Micropython-Types](#micropython-types)
     - [Module-Type](#module-type)
     - [Class-Type](#class-type)
4. [System Boundaries]
5. [Design Flow]
6. [Argument Parsing]
7. [Problematik mit Namespaces](#problematik-mit-namespaces) 
   -  [Super- & Submodule](#super-&-submodule)
   -   [Dictionaries und QStrings](#dictionaries-und-qstrings)
   -   [Limitierungen des MicroPython-Interpreters](#limitierungen-des-micropython-interpreters)
   - [Submodule als QString Alias](#submodule-als-qstring-alias)
   - [Kombination von Submodulen, QString und Supermodulen](#kombination-von-submodulen-qstring-und-supermodulen)
   - [Schlussfolgerung zu diesen Problemen](#schlussfolgerung-zu-diesen-problemen)
8. [Micropython Flags](#micropython-flags)
9. [Code::Blocks und Mockup](#codeblocks-und-mockup)
10. [Das Problem des Micropython Heaps](#das-problem-des-micropython-heaps)
11. [Wie man eine Micropython-Ausführung stoppt](#Wie-man-eine-Micropython-Ausführung-stoppt)
12. [Micropython und Defines](#Micropython-und-Defines)



### Warum den MicroPython Embed Port verwenden?

Im Rahmen meiner Bachelorarbeit wurden zwei mögliche Ansätze zur Integration des MicroPython-Interpreters in die ORB-Firmware identifiziert. Der erste Ansatz basiert auf der Verwendung des MicroPython STM-Port. Dieser Ansatz würde die MicroPython-Umgebung als Grundlage nutzen, in die die ORB-Firmware integriert werden müsste. Allerdings erachtete ich diesen Weg als eher unpraktisch, da er Komplikationen im Build-Prozess mit sich bringen könnte. Möglicherweise wären umfangreiche Anpassungen oder sogar das Umschreiben von MicroPython in C++ erforderlich. 

Da die ORB-Firmware idealerweise nur an den notwendigsten Stellen angepasst werden sollte und ein kompletter Umbau vermieden werden soll, ist dieser Ansatz nicht optimal. Wie im Exposé beschrieben, ist mein Ziel, den MicroPython-Interpreter als eigenständige Komponente einzubinden, anstatt die ORB-Firmware in das MicroPython-Projekt zu integrieren.

Der zweite Ansatz, für den ich mich entschieden habe, ist die Verwendung des MicroPython Embed Ports. Dieser Ansatz bietet den klaren Vorteil, dass der MicroPython-Interpreter als eigenständige Komponente in die ORB-Firmware eingebunden werden kann, ohne dass die Firmware selbst stark verändert werden muss. So muss ich lediglich sinnvolle Schnittstellen definieren. Der MicroPython Embed Port bleibt in C implementiert, was bedeutet, dass keine Änderungen am Interpreter-Code erforderlich sind. Die Schnittstellen zur ORB-Firmware ermöglichen dann später die Abbildung von C++- auf C-Funktionen.

### Aufsetzen des GitHub-Projekts

1. **Repository erstellen**: Erstellen Sie ein neues Repository auf GitHub. Zunächst sollte es sich um ein leeres Git-Projekt mit einer `README.md`-Datei handeln.

2. **Fork des MicroPython-Projekts erstellen**: Erstellen Sie einen Fork des MicroPython-Projekts, um Anpassungen vorzunehmen und zukünftige Updates durchzuführen, ohne den ursprünglichen Quellcode zu beeinflussen.

3. **MicroPython als Submodul hinzufügen**: Fügen Sie das geforkte MicroPython-Projekt als Submodul zum GitHub-Projekt hinzu, indem Sie folgenden Befehl ausführen:
    ```bash
    git submodule add https://github.com/NiHoffmann/micropython micropython
    ```

4. **MicroPython-Projekt einrichten**:
    - Bauen Sie den `mpy-cross`-Compiler mit dem Befehl `make`.
    - Beachten Sie, dass der Embed-Port keine externen Abhängigkeiten hat, daher ist es nicht erforderlich, `make submodules` auszuführen, wie in der MicroPython-Dokumentation beschrieben ([Link](https://github.com/NiHoffmann/micropython/readme.md)).

5. **Erste Projektkonfiguration mit MicroPython Embed**: Für die erste Einrichtung des Projekts benötigen Sie drei Dateien aus dem MicroPython-Embed-Projekt:
    - `micropython_embed.mk`: Diese Datei enthält die Make-Regeln zum Bauen des Embed-Ports.
    - `mpconfigport.h`: Diese Konfigurationsdatei enthält die Definitionen für die erforderlichen Komponenten des MicroPython-Projekts.

6. **Änderungen am geforkten MicroPython-Projekt**: Änderungen an der Datei `mpconfigport_common.h` sind erforderlich. Diese Datei ist für den Build-Prozess unter Linux vorgesehen und enthält Definitionen aus `alloca.h` für die Speicherzuweisung. Der äquivalente Windows-Header `malloc.h` ist jedoch nicht enthalten. Daher musste ich eine If/Else-Anweisung hinzufügen.

7. **Pfadanpassung in `micropython_embed.mk`**: In der Make-Datei `micropython_embed.mk` musste der Pfad für `MICROPYTHON_TOP` auf Folgendes geändert werden:
    ```makefile
    MICROPYTHON_TOP = ../micropython
    ```

8. **Kompilieren des MicroPython-Projekts**: Das MicroPython-Projekt kann nun kompiliert werden. Stellen Sie sicher, dass die Umgebung konfiguriert ist, einschließlich der Installation erforderlicher Werkzeuge wie MSYS2, Cygwin, GCC und Python. Starten Sie die Kompilierung mit folgendem Befehl:
    ```bash
    make -f .\micropython_embed.mk
    ```

9. **Einrichten des Code::Blocks-Projekts**: Erstellen Sie ein leeres Projekt in Code::Blocks und konfigurieren Sie es mit einer Debug-Einstellung, die den GCC-Compiler verwendet.

10. **`main.c` erstellen und Suchverzeichnisse konfigurieren**: Entwickeln Sie eine `main.c`-Datei und richten Sie die Suchverzeichnisse für den Compiler ein, um folgende Pfade einzuschließen:
    - `src/`
    - `libs/`
    - `libs/micropython_embed`
    - `libs/micropython_embed/port`

11. **Compiler-Flags hinzufügen**: Fügen Sie die Flags `-Og` und `-g` hinzu, um ein korrektes Debugging zu ermöglichen. Diese Flags sind entscheidend für die korrekte Kompilierung des Projekts mit dem GCC-Compiler in Code::Blocks. Ohne diese Flags kann es zu Kompilierungsproblemen kommen, deren genaue Ursachen noch untersucht werden müssen.

    > Die Verwendung des `-Og`-Flags beim Kompilieren von MicroPython mit Code::Blocks stellt sicher, dass der generierte Code so nah wie möglich am Quellcode bleibt und das ursprüngliche Verhalten sowie die Debugging-Fähigkeiten erhalten bleiben. Dieser Ansatz minimiert das Risiko von Problemen durch aggressivere Optimierungen. Wenn der Code ohne dieses Flag nicht korrekt ausgeführt wird, liegt es wahrscheinlich daran, dass die Optimierungen kritische Teile der MicroPython-Operationen oder die Interaktion zwischen dem Code und der Hardware-Plattform beeinträchtigen.
  
12. **Code:Blocks Build Targets anpassen**: Zu diesem Zeitpunkt ist der Build Prozess des Code:Blocks Projektes von dem des MP-Embed-Port los gelöst. Im bestenfall sollte aus der Code:Blocks umgebung auch der MP-Embed-Port gebaut werden können. Zu diesem Zweck habe ich dem Code:Blocks Projekt 2 Build-Target gegeben : *Build* und *Rebuild* während build der vorher configurierte Prozess ist, ist rebuild erweitert durch einen Pre-Build-Step. Das *Rebuild*-Build-Target ruft ein batch script auf welches die commandos für das 'clear' und 'build' des MP-Embed-ports enhält. Die Verwendung der Target ist wie Folgt gedacht, entwickelt man nur in der Code:Blocks umgebung ohne neue Module/Funktionen hinzuzfügen so reicht das Build-Target aus. Da die MP-Embed-Port ressourcen nicht immer neu generiert werden müssen. Kommen nun neue Module hinzu, werden alte Umbgenannt oder neue Funktionen eingeführt, so kann man einmal das Rebuild-Target ausführen. Danach verwendet man wieder wie gewohn Build.

### ORB-Modul hinzufügen

#### Anpassungen an `micropython_embed.mk`:

1. **USER_C_MODULES Pfad erweitern**: Hier das Root-Verzeichnis für Module angeben und den entsprechenden Pfad definieren.
2. **CFLAGS für Include-Verzeichnisse hinzufügen**: Diese Verzeichnisse sollten in den CFLAGS aufgenommen werden.
3. **`embed.mk` verlinken**: Die Datei `embed.mk` einbinden mit der Zeile `include $(MICROPYTHON_TOP)/ports/embed/embed.mk` und den Pfad zu `MICROPYTHON_TOP` angeben.

Zurzeit gibt es zwei Include-Verzeichnisse: `Mockups` und `Module`.

Die `micropython_embed.mk` Datei sucht im `USER_C_MODULES` Pfad nach Ordnern, geht in diese hinein und ruft jede Datei mit dem Namen `micropython.mk` auf. Im `modules`-Ordner selbst können jedoch keine `micropython.mk` Dateien liegen, da diese ignoriert werden.

Deshalb habe ich einen Ordner `ORB` erstellt, der als Modulbasis für alle Implementierungen des ORB-Moduls dient. In diesem Modul werden weitere Klassen und Typen angebunden und bereitgestellt.

#### Micropython-Types
Für die Umsetzung meines Projektes sind 2 Arten von Types die mit Micropython abgebildet werden können wichtig. Die Modul und Typ klassen. Im Folgenden werden diese Genauer erklärt.

#### Module-Type
Modul Klassen sind vom Daten-Typ `mp_obj_module_t` und werden verwendet um Module abzubilden. Im einfachten Fall bestehen diese aus zwei Teilen, bzw. definierungs schritten. 
1. Erstellen eines Dictionarys, diese werden verwendet um Modul Meta Informationen zu verwalten. Wie z.b. Module name, oder die Init funktion welche bei Import des Moduls aufgerufen wird.
    ```cpp
    static const mp_rom_map_elem_t devices_globals_table[] = {
        { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_devices) },
        { MP_ROM_QSTR(MP_QSTR___init__), MP_ROM_PTR(&devices___init___obj) },
        { MP_ROM_QSTR(MP_QSTR_motor),  MP_ROM_PTR(&motor_type) },
    };
    static MP_DEFINE_CONST_DICT(devices_globals, devices_globals_table);
    ```
    Solche Dictionaries bestehen idr. aus einem paar aus `MP_ROM_QSTR` hier wird der Name der Funktionalität deffiniert. Hier ist `___name___` eine in Micropython eingabaute Funktion.
    Es wird dem Modul der Name 'devices' gegeben. Dies ist nicht der Name des Moduls für Imports, sondern der Name den das Modul für sich selber verwaltet gibt. Es ist wichtig solche Namen immer als 
    QStrings mit Hilfe des Micropython Makros zu generieren. Die Zeile `{ MP_ROM_QSTR(MP_QSTR_motor),  MP_ROM_PTR(&motor_type) },` wird verwendet um eine Micropython Class-Type unter dem Module zu 
    registrieren. Diese ist dann später durch das Modul erreichbar. Zu letzt muss man das mp_rom_map_elem_t array zu einem Dictionary umwandeln, durch das makro `MP_DEFINE_CONST_DICT`.   

2. Als nächstes muss das Dictionary unter einem Module registriert werden. Hierfür gibt es den vorher erwähnte typ `mp_obj_module_t`. Dieser hat für uns an dieser Stelle 2 wichtige attribute.
    ```cpp
       const mp_obj_module_t devices_module = {
        .base = { &mp_type_module },
        .globals = (mp_obj_dict_t *)&devices_globals,
        };
        MP_REGISTER_MODULE(MP_QSTR_devices, devices_module);
    ```
    Die base, eine Pointer welcher eine art Typ zuweisung für den Micropython-Interpreter ist. Die Modul base ist hier `&mp_type_module` jedes modul hat diesen pointer als Base. Und erhält somit 
    alle Funktionalitäten und Eigenschaften die man von einem Modul erwarten würde. Das `globals` attribut ist die durch das Modul errichbaren Klassen, Funktionen, etc. kurz gesagt das even 
    generierte Dictionary. `MP_REGISTER_MODULE` registriert das Module, hier wird ein QString angegeben werden welcher den Name des Moduls deffiniert. Dies ist dann später der Name welcher für einen
    import verwendet wird.
      
    Aus der Datei makemodulesdef.py ist zu entnehmen das es 2 arten gibt Module zu registrieren MP_REGISTER_MODULE, ,MP_REGISTER_EXTENSIBLE_MODULE and MP_REGISTER_MODULE_DELEGATION.
    MP_REGISTER_MODULE Declare a module as a builtin.  
    MP_REGISTER_EXTENSIBLE_MODULE allow this module to be extended from the filesystem.  
    MP_REGISTER_MODULE_DELEGATION is used to delegate the registration or initialization of a module to an external function.  

#### Class-Type
//TODO

## Problematik mit Namespaces

#### Super- & Submodule

Hier möchte ich kurz erklären, was ich mit Super- und Submodulen meine. Ein Modul kann in diesem weiten Sinne auch synonym mit Unterklassen oder registrierten Funktionen verwendet werden. Möglicherweise wird hier ein anderer Name gebraucht.

#### Dictionaries und QStrings

Dictionaries halten die Informationen über QStrings (Namen von Modulen, Funktionen, Typen, etc.).

#### Limitierungen des MicroPython-Interpreters

Imports von Modulen funktionieren nur eine Ebene tiefer. Hat man z.B. ein Modul `Devices` und registriert darin das Submodul `Sensors`, welches wiederum einen spezifischen Sensor wie `EV3-Lichtsensor` enthält, so könnte man erwarten, dass die Syntax `import Devices.Sensors.EV3-Lichtsensor` oder `from Devices.Sensors import EV3-Lichtsensor` funktioniert. Jedoch findet der MicroPython-Interpreter diese Module nicht.

Für solche Submodule bietet MicroPython ein alternatives Vorgehen an, wie es auch in dessen Submodule-Beispiel erklärt wird. Diese Methode bringt jedoch andere Probleme mit sich.

#### Submodule als QString Alias

Ein alternatives Vorgehen für solche Submodul-Strukturen wäre, das Modul `Devices` gar nicht umzusetzen. Dadurch kann man diesem jedoch keine eigenen Funktionen zuweisen (erste Einschränkung). Zudem ist man beim Importieren der Module in die andere Richtung begrenzt.

Man würde ein Modul `Sensor` schreiben, registriert dieses jedoch nicht bei einem Supermodul, sondern gibt ihm den Namen `MP_QSTR_devices_dot_sensors`. Anschließend muss man eine QString-Datei erstellen (welche in den MicroPython-Build-Prozess eingebunden wird) und darin die Strings ohne Punkt bereitstellen, z.B. `Q(devices.sensors)`. Erst dadurch erhält das Modul den Namen mit ".".

Jetzt kann man jedoch das Modul `devices` selbst nicht importieren, sondern muss stattdessen `from devices.sensors import EV3-Lichtsensor` verwenden.
<!-- TODO: Kann man auch `import devices.sensors.EV3-Lichtsensor` schreiben? -->

#### Kombination von Submodulen, QString und Supermodulen

Man könnte auf die Idee kommen, beide Ansätze zu kombinieren. Man erstellt ein Supermodul `devices`, registriert dieses sowie das zugehörige Untermodule. Das Untermodule selbst wird ebenfalls registriert. Dieses Vorgehen scheint auf den ersten Blick zu funktionieren und löst die oben beschriebenen Probleme, hat jedoch auch unerwartetes Verhalten.

Führt man `import devices.sensors` aus, so scheint der Import korrekt zu funktionieren. Bei näherer Betrachtung wird jedoch nicht das Modul `sensors` importiert, sondern es wird das Modul `sensors` unter dem Namen `devices` importiert. Führt man dann `print(devices)` aus, so wird der Name des `sensors` Moduls ausgegeben und überschreibt das `devices` Modul, zumindest bis `import devices` ausgeführt wurde.

#### Schlussfolgerung zu diesen Problemen

Ich hatte ursprünglich geplant, den Namespace `orb` zu verwenden, um Module zu organisieren, die Klassen und Funktionen zusammenfassen, wie z.B. `orb.sensors`, `orb.devices`, usw. Aufgrund der oben genannten Probleme ist dies jedoch nicht möglich. Der `orb`-Prefix muss gestrichen werden, um das von Python erwartete Verhalten zu gewährleisten.

Es gibt daher die Einschränkung, Module nur eine Ebene tief zu gestalten. Funktionen und Klassen auf einer dritten Ebene registriere ich nur, wenn sie ausschließlich im Zusammenhang mit dem Supermodul Sinn machen, z.B. Funktionen einer Klasse, die zuvor instanziiert werden muss.

### Micropython Flags
  
Die Micropython Flags werden in der Datei `mpconfigport.h` definiert, ein auszug aus dieser Datei könnte so aussehen.
```cpp
#define MICROPY_CONFIG_ROM_LEVEL                (MICROPY_CONFIG_ROM_LEVEL_MINIMUM)

#define MICROPY_PERSISTENT_CODE_LOAD            (1)
#define MICROPY_ENABLE_COMPILER                 (1)

#define MICROPY_ENABLE_GC                       (1)
#define MICROPY_PY_GC                           (1)
#define MICROPY_FLOAT_IMPL                      (MICROPY_FLOAT_IMPL_FLOAT)
```
Diese Flags werden benutzt um Micropython mitzuteilen welche Module zu dem Port hinzu geladen werden sollen und welche nicht. Es gibt eine ganze mänge an Standart Module welche für unseren Zweck nicht braucht werden. Daher ist als erstes die verwendung von   
`#define MICROPY_CONFIG_ROM_LEVEL (MICROPY_CONFIG_ROM_LEVEL_MINIMUM)`  
hervor zu heben, dies bewirkt das wir keine Standart Module , sondern nud das absolut minimale grund gerüst des MP-Interpreters hinzufügen.
Darauf folgen kommen zweit weitere Wichtige Flags, `MICROPY_PERSISTENT_CODE_LOAD` fügt den Micropython-Byte-Interpreter hinzu. Dieser ist später einmal für mein Projekt der einzige weg Micropython-Programme auszuführen.  
`MICROPY_ENABLE_COMPILER` Hingegen in der String Interpreter, dieser ist aus bequemlichkeits zwecken zu diesem Zeitpunkt noch hinzu geschaltet, und kann auch (solange er dies nur für Entwicklungs zwecke ist, aktiv bleiben). Im fertigen Projekt wird diese Flag jedoch nicht mehr aktiv sein.  


## Code::Blocks und Mockup

Wie in [Warum den MicroPython Embed Port verwenden?](#warum-den-micropython-embed-port-verwenden) bereits beschrieben, soll der MicroPython Embed Port ohne die direkte Einbindung der ORB-Firmware kompiliert werden. Zu diesem Zweck werden sogenannte "Mockups" eingeführt. Diese Mockups sind C-Dateien, die die C-Interface-Methoden der ORB-Firmware simulieren (welche ich später selbst in das ORB-Firmware-Projekt einfügen werde).

Das bedeutet, dass Mockups die gleichen Enums, Funktionen und Variablen enthalten, die später von der ORB-Firmware benötigt werden, um die Funktionalitäten für die MicroPython-Umgebung bereitzustellen. Sie bieten jedoch selbst keine vollständige Implementierung dieser Funktionen an. Ein Beispiel könnte folgendermaßen aussehen:

Es gibt eine Motor-Mockup-Datei mit der folgenden Funktion:

```cpp
void setMotor(uint8_t port, uint8_t mode, int16_t speed, int pos) {
    printf("set motor port(%u) mode(%u) speed(%d) pos(%d)\n", port, mode, speed, pos);
}
```

Das Motor-Modul kann dann diese Mockup-Funktion verwenden:
```cpp
static mp_obj_t set(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    enum { ARG_mode, ARG_speed, ARG_position };

    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_mode, MP_ARG_REQUIRED | MP_ARG_KW_ONLY | MP_ARG_INT, {.u_int = 0 } },
        { MP_QSTR_speed, MP_ARG_KW_ONLY | MP_ARG_INT, {.u_int = 0 } },
        { MP_QSTR_position, MP_ARG_KW_ONLY | MP_ARG_INT, {.u_int = 0 } },
    };

    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args - 1, pos_args + 1, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    motor_obj_t *self = MP_OBJ_TO_PTR(pos_args[0]);

    //use the mockup funktion 
    setMotor(self->port, args[ARG_mode].u_int, args[ARG_speed].u_int * self->direction, args[ARG_position].u_int * self->direction);


    return MP_OBJ_FROM_PTR(self);
}
static MP_DEFINE_CONST_FUN_OBJ_KW(set_obj, 1, set);
```

Im Gegensatz dazu wird die ORB-Firmware anstelle der Mockup-Datei eine andere Motor-Datei einbinden, die die tatsächliche Funktionalität als C-Code bereitstellt.


### Das Problem des Micropython Heaps

Micropython hat einen sehr begrenzten Heap für die Speicherverwaltung. Dies stellt einige Probleme dar, die bei der Implementierung einer Micropython-Firmware sowie beim Arbeiten mit Micropython selbst berücksichtigt werden müssen. 
Ein Problem, das ich gefunden habe und das das Arbeiten mit Micropython kompliziert, ist folgendes. (Ich werde das Problem beispielhaft für einen Datentyp besprechen, dieses Konzept gilt auch für andere Typen.) Betrachtet man sich Floats in Micropython an fällt folgendes auf. In Micropython sind Floats einfach Objekte, die das Verhalten aufweisen, das man von Floats erwarten würde; sie sind keine primitiven Typen.   
Nehmen wir an, wir haben den folgenden Code:  
```python
import gc
a = 0.5
while a < 10000:
    a = a + 0.5
    gc.mem_free()
```
  
Jedes Mal, wenn a = a + 0.5 aufgerufen wird, liest die virtuelle Maschine den alten Wert von a und erstellt ein neues Objekt mit dem neuen Wert. Das alte Objekt bleibt jedoch im Speicher. Dieses kleine Programm kann sehr schnell zu einem Speichermangel führen, aufgrund unzureichender Heap-Speicherkapazität. Wir können dieses Überlaufen verfolgen, indem wir den freien Speicher (`gc.mem_free()`) ausgeben. Eine Lösung für dieses Problem besteht darin, den integrierten Garbage Collector zu verwenden.  

```python
import gc
a = 0.5
while a < 10000:
    a = a + 0.5
    gc.collect()
    gc.mem_free()
```

Wenn wir nun die Ausgabe von `gc.mem_free()` betrachten, sehen wir, dass der verwendete Speicher nicht unbegrenzt ansteigt. Dies ist eine gute Lösung, aber der Benutzer muss über dieses Problem Bescheid wissen.  

Eine mögliche Lösung für dieses Problem ist das Integrieren eines Aufrufs zum Garbage Collector in die Routine zur Erstellung neuer Objekte. Dies könnte eine Lösung sein, wenn das Hinzufügen von möglicherweise unnötigem Overhead kein Problem darstellt. In den Meisten fällen hat man bei Microcontrollern jedoch hier eine starke begrenzung, daher ist die wohl zumeist keine gute lösung. Für mich sind die Klassen, die von diesem Problem betroffen sind, Sensoren, Motoren usw. Alle diese Klassen entsprechen einem an einem Port angeundenen Gerät. Daher bestand meine Lösung darin, Listen mit einem Objekt pro Port zu erstellen. Hier für die 2 Servo-Ports: 
  
```cpp
servo_obj_t servo_obj_list[2] = {
    { .base = { .type = &servo_type }, .port = 0, .speed = 0, .angle = 0 },
    { .base = { .type = &servo_type }, .port = 1, .speed = 0, .angle = 0 },
};
```

Wann immer ein Benutzer ein neues Objekt erstellt , erhält er eines dieser zuvor vorbereiteten Objekte. Das Ausführen des folgenden Codes:
```python
from devices import servo
a = servo(0)
b = servo(0)
a.set(speed=10, angle=20)
```
wird nicht nur den Zustand von Servo `a` aktualisieren, sondern auch Servo `b` beeinflussen, da sie ein gemeinsames Objekt haben. Auf diese Weise treten keine Speicherprobleme im Zusammenhang mit dem zuvor erwähnten Problem mehr auf. Der Benutzer hat immer Objekte, die den aktuellen Zustand der realen Geräte oder zumindest der angewendeten Einstellungen repräsentieren.  

```python
a = servo(0)
b = servo(0)
a.set(speed=20)
b.set(angle=30)
```
Der Servo wird nun auf `angle(30)` mit `speed(20)` bewegen.


## Wie man eine Micropython-Ausführung stoppt

Für das Anbinden des Python-VM muss es eine möglichkeit geben dies VM-Ausführung durch die ORB-Firmware zu unterbrechen.  
Betrachtung der bereits bestehenden Systeme wie das Microbit haben folgende erkenntnis gebracht. Bereits bestehende Systeme setzen einfach den gesamten Mikrocontroller zurück. Dies ist für einen Microcontroller, der nur Microypthon ausführt eine gut lösung. Für unseren Anwedungsfalls jedoch eher eine unpassende Lösung.  

Microbit:
```cpp
static mp_obj_t microbit_reset_(void) {
    NVIC_SystemReset();
    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_0(microbit_reset_obj, microbit_reset_);
```
oder auch Zephyr:  
```cpp
static mp_obj_t machine_reset(void) {
    sys_reboot(SYS_REBOOT_COLD);
    // Won't get here, Zephyr has infiniloop on its side
    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_0(machine_reset_obj, machine_reset);
```
Da der Embed-Port für diese Micropython-Implementierung eine separate Task sein sollte und nicht den gesamten Controller bei einem Programm-Reset zurücksetzen sollte, müssen wir hier eine eigene Lösung finden.  

Meine Beobachtung wurde bestätigt, als ich durch die Micropython-Community-Chats schaute: „You can reset the processor from within an ISR using pyd.hard_reset() or machine.reset(). Otherwise, you'd need to set a flag and have the main script exit when it detects that flag is set.“ (https://forum.micropython.org/viewtopic.php?t=2521)  
  
Die Lösung hier besteht darin, ein neues Flag einzuführen. Ich habe die Überprüfung meines Flags in die Datei vm.c am Anfang der dispatch_loop aufgenommen. Diese Schleife ist die Logik, die bestimmt, wie eine Zeile Code verarbeitet werden soll.  

Da Micropython nicht threadsicher ist, sind einige wichtige Überlegungen zu beachten: Ich sollte nur innerhalb der VM, in den von der VM verwendeten Speicher schreiben. Daher habe ich die gesamte Logik zur Erstellung und Injection meiner Exceptuon an den Anfang der dispatch_loop verschoben , diese erkennt das eine Exception geplant ist und handbat sie dann im folgenden.  
(vm.c Zeile: 309).  
```cpp
<...>
dispatch_loop:
       //This is the Main Logic, orb_interrupt will only ever be written from outside mp
       //so this flag is never a race condition
       //inside here we create the exception so we never get mem error
       //we have to do this at the top to bypass controll flow
       #ifdef ORB_ENABLE_INTERRUPT
       if(MP_STATE_VM(orb_interrupt)){
           static mp_obj_exception_t system_exit;
           system_exit.base.type = &mp_type_SystemExit;
           //since this is a user interrupt the traceback will be empty
           system_exit.traceback_alloc = 0;
           system_exit.traceback_data = NULL;
   
           //we pass a single argument in our tuple, the error message
           system_exit.args = (mp_obj_tuple_t*) mp_obj_new_tuple(1, NULL);
           mp_obj_t mp_str = mp_obj_new_str("User Interrupt", 14);
           system_exit.args->items[0] = mp_str;
           MP_STATE_THREAD(mp_pending_exception) = &system_exit;
           MP_STATE_VM(orb_interrupt_injected) = true;
       }
       #endif

<...>
```
Von außerhalb der VM muss nun im falle eines Interrupts nur das Flag `orb_interrupt` gesetzt werden. Es ist wichtig zu beachten, dass dieses Flag nicht von innerhalb des Micropython-Projekts beschrieben wird, auch nicht zum Zurücksetzen des Flags.   

Dieser Code plant eine einfache Exception. Der hier Nachteile ist, dass dies (mit einem unveränderten Micropython) von einem Try/Catch-Block erfasst wird.   
  
Somit musste ich noch eine weitere Änderung vornehmen. Daher musste ich den folgenden Teil der VM ändern, um die Try-Catch-Logik zu ignorieren, falls das Interrupt-Flag gesetzt ist   
(vm.c Zeile: 1473).  
```cpp
<...>
if (exc_sp >= exc_stack
    //this part of the code handles try/catch blocks, we dont want them to be treated as such, if orb interrupts treate any error as un-handled
    #ifdef ORB_ENABLE_INTERRUPT
    && !MP_STATE_VM(orb_interrupt_injected)
    #endif
    ) {
    // catch exception and pass to byte code
 #endif
) {
<...>
```
Nun kann der VM-Interrupt geplant werden. Da die Dispatch-Schleife jedes Mal besucht wird, wenn etwas verarbeitet werden muss, wird dies immer dazu führen, dass die Ausführung nach Abschluss der aktuellen Befehlsverarbeitung unterbrochen wird.  
  
## Micropython und Defines

//TODO
Idee der Defines, für z.b. Sensor Klasse 
vergleichbar mit vererbungs-struktur eine OOP-Sprache

