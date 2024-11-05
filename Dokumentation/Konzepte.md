# Inhaltsverzeichnis

- [Warum den MicroPython Embed Port verwenden?](#warum-den-micropython-embed-port-verwenden)
- [MicroPython-Types](#micropython-types)  
    - [MicroPython-Object-Type](#micropython-object-type)
    - [MicroPython-Typ-Klassifizierungen](#micropython-typ-klassifizierungen)
        - [Nicht-Konkrete-Typen](#nicht-konkrete-typen)
        - [Konkrete-Typen](#konkrete-typen)
            - [Module-Type](#module-type)  
            - [Klassen-Type](#klassen-type)
                - [Selbst Deffinierte Klassen](#selbst-deffinierte-klassen)
                - [Bereit gestellte Funktionen und Konzepte](#bereit-gestellte-funktionen-und-konzepte)
    - [Funktions-Objekt-Definition](#funktions-objekt-definition)
- [MicroPython Typ Zuordnung](#micropython-typ-zuordnung)
    - [Worauf Stützt sich die Typ-Zuordnung](#worauf-stützt-sich-die-typ-zuordnung)
    - [Wie funktioniert die Typ-Zuordnung (Non-Concrete-Types)](#wie-funktioniert-die-typ-zuordnung-non-concrete-types)
    - [Wie funktioniert die Typ-Zuordnung (Concrete-Types)](#wie-funktioniert-die-typ-zuordnung-concrete-types)
- [Problematik bei der Verwendung von Namespaces](#problematik-bei-der-verwendung-von-namespaces) 
    - [Super- & Submodule](#super-&-submodule)
    - [Limitierungen des MicroPython-Interpreters](#limitierungen-des-micropython-interpreters)
    - [Submodule als QString Alias](#submodule-als-qstring-alias)
    - [Kombination von Submodulen, QString und Supermodulen](#kombination-von-submodulen-qstring-und-supermodulen)
    - [Schlussfolgerung zu diesen Problemen](#schlussfolgerung-zu-diesen-problemen)
- [MicroPython Flags](#micropython-flags)
- [Reduzierung des MicroPython-Heap-Verbrauchs durch Objektreferenzen](#reduzierung-des-micropython-heap-verbrauchs-durch-objektreferenzen)
- [Thread Safety](#thread-safety)
- [Windows-Bug: Falsches Register bei Non-Local Return-Adressierung](#windows-bug-falsches-register-bei-non-local-return-adressierung)
    - [Definition und Funktionsweise des Non-Local Return](#definition-und-funktionsweise-des-non-local-return)
    - [Beschreibung des Bugs](#beschreibung-des-bugs)
    - [Vorgehensweise zur Fehlerbehebung](#vorgehensweise-zur-fehlerbehebung)   
- [Compiler Flag Kompatibilität](#compiler-flag-kompatibilität)
    - [MicroPython Compiler Flags](#micropython-compiler-flags)
    - [ORB-Firmware Compiler Flags](#orb-firmware-compiler-flags)
    - [Angepassten Compiler Flags](#angepassten-compiler-flags)
- [QStrings](#qstrings)

## Warum den MicroPython Embed Port verwenden?

Im Rahmen dieser Bachelorarbeit wurden zwei mögliche Ansätze, für die Integration des MicroPython-Interpreters in die ORB-Firmware identifiziert. Der erste Ansatz basiert auf der Verwendung eines der MicroPython-STM-Ports. Dafür könnte man z.B. den [ADAFRUIT_F405_EXPRESS-Port](./../../../../micropython/tree/master/ports/stm32/boards/ADAFRUIT_F405_EXPRESS) wählen. Dieser Ansatz würde die MicroPython-Umgebung als Grundlage nutzen. Somit müsste die ORB-Firmware als Erweiterung des MicroPython-Port eingebunden werden. Allerdings kann man diesen Weg als eher unpraktisch ansehen. Da er Komplikationen im Build-Prozess mit sich bringen könnte. Möglicherweise wären auch umfangreiche Anpassungen an dem MicroPython-Port vorzunehmen. Grade durch die Natur der ORB-Firmware als C++-Projekt.

Die ORB-Firmware sollte idealerweise nur an den notwendigsten Stellen angepasst werden. Wie bereits im Exposé beschrieben, ist das Ziel meiner Bachelorarbeit, den MicroPython-Interpreter als eigenständige Komponente in die ORB-Frimware einzubinden. Somit ist dies nicht das erwünschte Vorgehen.

Der zweite Ansatz ist die Verwendung des MicroPython Embed Ports. Dieser Ansatz bietet einen klaren Vorteil. Der MicroPython-Interpreter kann eigenständige Komponente in die ORB-Firmware eingebunden werden kann. Und dies, ohne dass die ORB-Firmware stark verändert werden muss. Es müssen lediglich sinnvolle Schnittstellen definiert werden. Dies wären C++-Schnittstellen-Klassen, um die MicroPython-VM bereit zustellen. So wie Schnittstellen der ORB-Firmware, welche dessen C++- auf C-Funktionen abbilden sollen und dadurch verwendbar im MicroPython-Code machen.

Gleichzeitig hat dieser Ansatz den Vorteil, das der MicroPython-Embed-Port auch unter Windows compiliert werden kann. Hier kann mithilfe von Code::Blocks oder einer anderen Entwicklungs-Umgebung eine Debug-Umgebung geschafft werden. Mithilfe von dieser sollte der Prozess zur Entwicklung für MicroPython bedeutend erleichtert werden.

Aufgrund dieser Vorteile wird der zweite Ansatz für das Umsetzen dieses Projektes verwendet.

## MicroPython-Types
Es gibt eine Vielzahl an MicroPython-Typen die abgebildet werden können. Diese werden in die Ober-Gruppe Konkrete- und nicht Konkrete-Typen eingeteilt. Die Konkreten-Typen sind grade für die Entwicklung eines MicroPython-Ports interessant. Diese erlauben das Abbilden von Modulen, Klassen und Objekten. Im Folgenden werden diese genauer Erklärt.

- [MicroPython-Object-Type](#micropython-object-type)  
- [MicroPython-Typ-Klassifizierungen](#micropython-typ-klassifizierungen)
    - [Nicht-Konkrete-Typen](#nicht-konkrete-typen)
    - [Konkrete-Typen](#konkrete-typen)
        - [Module-Type](#module-type)  
        - [Klassen-Type](#klassen-type)
            - [Selbst Deffinierte Klassen](#selbst-deffinierte-klassen)
            - [Bereit gestellte Funktionen und Konzepte](#bereit-gestellte-funktionen-und-konzepte)
    - [Funktions-Objekt-Definition](#funktions-objekt-definition)

#### MicroPython-Object-Type
Ein MicroPython-Objekt (mp_obj_t), ist ein abstrakter Zeiger. Dieser wird genutzt, um verschiedene Objekttypen generisch zu behandeln. 
> [!NOTE]  
> MicroPython verwaltet nahezu alles als MicroPython-Objekt. Wird von einem MicroPython-Objekt gesprochen, so handelt es sich um eine mit 'mp_obj_t' abstrahierten Daten-Typ. Es werden selbst an ein Objekt gebundene Funktionen oder Module als MicroPython-Objekt verwaltet.

`mp_obj_t` ist wie folgt definiert:  
```cpp
// This is the definition of the opaque MicroPython object type.
// All concrete objects have an encoding within this type and the
// particular encoding is specified by MICROPY_OBJ_REPR.
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
typedef uint64_t mp_obj_t;
typedef uint64_t mp_const_obj_t;
#else
typedef void *mp_obj_t;
typedef const void *mp_const_obj_t;
#endif
```
[[MP, 'micropython/py/obj.h', ab Zeile: 40]](./../../../../micropython/tree/master/py/obj.h#L40-L46)


Da die MICROPY_OBJ_REPR-Definition in diesem Projekt, durch die Standardkonfiguration vorgegeben ist, wird MICROPY_OBJ_REPR_A verwendet.  
```cpp
#ifndef MICROPY_OBJ_REPR
#define MICROPY_OBJ_REPR (MICROPY_OBJ_REPR_A)
#endif
```
[[MP, 'micropython/py/mpconfig.h', ab Zeile: 153]](./../../../../micropython/tree/master/py/mpconfig.h#L153-L155)


Damit wird `mp_obj_t` als Alias für einen `Void-Pointer`, also einen typfreien Zeiger verwendet.
Aus dem Kommentar zu MICROPY_OBJ_REPR_A lässt sich eine weitere Erkenntnis ableiten.  
```cpp
// A MicroPython object is a machine word having the following form:
//  - xxxx...xxx1 : a small int, bits 1 and above are the value
//  - xxxx...x010 : a qstr, bits 3 and above are the value
//  - xxxx...x110 : an immediate object, bits 3 and above are the value
//  - xxxx...xx00 : a pointer to an mp_obj_base_t (unless a fake object)
#define MICROPY_OBJ_REPR_A (0)
```
[[MP, 'micropython/py/mpconfig.h', ab Zeile: 112]](./../../../../micropython/tree/master/py/mpconfig.h#L112-L117)

Der MicroPython-Objekt-Pointer wird nicht ausschließlich als Void-Pointer genutzt. Sondern enthält auch Informationen über das gespeicherte Objekt. In einigen Fällen, wie bei 'Small Int', wird der variable Wert im Pointer gespeichert. Dafür werden die hier beschriebenen gesetzten Bits später für Typ-Zuordnung verwendet.
> [!NOTE]
> Mehr dazu unter [MicroPython-Typ-Zuordnung](#micropython-typ-zuordnung).

Da jetzt geklärt ist, was der MicroPython-Objekt-Typ ist. Kann nun geklärt werden welche Rolle er spielt, wo und wie er verwendet wird.

Funktionen, die in MicroPython geschrieben werden und auf MicroPython-Objekten arbeiten, erwarten Argumente stets als mp_obj_t. Dazu gehört auch das Self-MicroPython-Objekt, wie in folgendem Beispiel:
```cpp
static mp_obj_t get_button(mp_obj_t self_id) {
    button_obj_t *self = MP_OBJ_TO_PTR(self_id);
    bool ret = getSensorDigital(self->id);
    return mp_obj_new_bool(ret);
}
```
Auch der Rückgabewert einer Methode ist immer vom Typ `mp_obj_t`. Selbst spezielle Rückgabewerte wie None, True oder False sind als solche Objekte gekapselt.  
```cpp
#define mp_const_none (MP_OBJ_FROM_PTR(&mp_const_none_obj))
#define mp_const_false (MP_OBJ_FROM_PTR(&mp_const_false_obj))
#define mp_const_true (MP_OBJ_FROM_PTR(&mp_const_true_obj))
```
[[MP, 'micropython/py/obj.h', ab Zeile:890]](./../../../../micropython/tree/master/py/obj.h#L890-L892)


> [!NOTE]  
> Das Makro `MP_OBJ_FROM_PTR` wird verwendet, um einen beliebigen Pointer zu einem `mp_obj_t` zu casten.

Es gibt bei der Verwaltung von MicroPython-Objekten ein paar unterschiede. So werden "Vollwertige" Integer als Objekte verwaltet und haben ein 'mp_obj_int_t-struct'. Wie oben beschrieben, stellen jedoch 'Small Int' eine Sonderrolle dar.
Der Unterschied im Zugriff lässt sich an der folgenden Methode gut erkennen:  
```cpp
mp_int_t mp_obj_int_get_truncated(mp_const_obj_t self_in) {
    if (mp_obj_is_small_int(self_in)) {
        return MP_OBJ_SMALL_INT_VALUE(self_in);
    } else {
        const mp_obj_int_t *self = self_in;
        return self->val;
    }
}
```
[[MP, 'micropython/py/objint_longlong.c', ab Zeile:284]](./../../../../micropython/tree/master/py/mpconfig.h#L284-292)

Daten-Typen wie 'Small Int' verhalten sich dabei Konzeptionell, wie jedes andere MicroPython-Objekt. Sie werden genauso wie jedes anderes MicroPython-Objekt als Pointer verwaltet. Haben jedoch einen Unterschied bei dem Zugriff auf Daten-Werte. 

Im Gegensatz dazu:
Arbeitet man mit selbst implementierten oder bereit gestellten Objekten, so sieht in zugriff in der Regel wie folgt aus:
```cpp
<...>
static void mp_funktion(..., mp_obj_t obj_input, ...){
button_obj_t *button = MP_OBJ_TO_PTR(obj_input);
<...>
```
Man hat ein 'Struct' welches das Objekt beschreibt in diesem Fall 'button_obj_t'. Mithilfe des Makros 'MP_OBJ_TO_PTR' wird aus dem 'mp_obj_t'
ein Pointer zu der eigentlichen Struktur. Dieses kann dann in Folge Operationen verwendet werden.

> [!NOTE]
>Wobei das Makro 'MP_OBJ_TO_PTR' ein einfacher Cast ist. Es ist keine komplexe Operation und eher syntaktischer Zucker:  
>```cpp
>#define MP_OBJ_TO_PTR(o) ((void *)(o))
>```
> Angesichts der Komplexität des MicroPython-Projektes ist eine solche
> Vereinfachung eine große Hilfe und erleichtert vor allem die Lesbarkeit von
> MicroPython C-API Code.   
'MP_OBJ_TO_PTR' ist das Gegenstück zu dem Anfang erwähnten 'MP_OBJ_FROM_PTR'.

Es ist wichtig, diesen Cast nur vorzunehmen, wenn Typ-Sicherheit gegeben ist. MicroPython bietet jedoch auch eine Möglichkeit für Typprüfungen an. Der Typ-Vergleich eines MicroPython-Objektes setzt voraus, das dieses Korrekt aufgebaut wurde. Es ist wichtig zu beachten, dass die 'mp_obj_base_t' eines MicroPython-Objektes an erster Stelle der Objekt-Struktur seht. Mehr dazu unter [Wie funktioniert die Typ-Zuordnung (Concrete-Types)](#wie-funktioniert-die-typ-zuordnung-concrete-types).

### MicroPython-Typ-Klassifizierungen
Im Folgenden werden die verschiedenen Arten der MicroPython-Typen erläutert. Dies sind alles Ausprägungen des Abstrakten MicroPython-Objektes. Nicht-Konkrete sowie Konkrete-Typen stellen dabei eine Ober-Klassifizierung dar. Diese werden im Folgenden kurz qualitativ erklärt.

### Nicht-Konkrete-Typen
Nicht-Konkrete-Typen sind solche Typen, welche kein eigenes Typ-Struct haben. Sie verwalten ihre Informationen als Teil ihres `mp_obj_t` pointer. Dabei sind sie kein Zeiger auf ein Strukt. Sondern im wesentlichen, ähnlich wie ein einfach Integer ein Daten-Wert träger.  Bei diesen wird ein Teil des `mp_obj_t`-pointer in bereiche für Daten-Werte und Typ-Informationen aufgeteilt. Darunter zählen zum Beispiel 'Small Ints'.    
```cpp
// as its first member (small ints, qstr objs and inline floats are not concrete).
```
[[MP, 'micropython/py/obj.h' ab Zeile:52]](./../../../../micropython/tree/master/py/obj.h#L52)  

Wie Nicht-Konrekte-Typen aufgebaut sind, ist in zusammenhang mit der Typ Zuordnung am besten zu verstehen. Dies kann unter [MicroPython Typ Zuordnung](#micropython-typ-zuordnung) nachgelesen werden. 

### Konkrete-Typen
Konkrete-Typen sind MicroPython-Objekte welche ein eigenen Typ-Struct besitzen. Sie müssen zu diesem gecastet werden, und erlauben somit das Verwalten von Komplexen Objekten.
Zu den Konkreten-Typen gehören Module, Klassen & Objekt-Instanzen. Konkrete-Typen erwarten, wie bereits erwähnt eine 'mp_obj_base_t' an erster Stelle ihres 'mp_obj_t'-pointers.   
```cpp  
// Anything that wants to be a concrete MicroPython object must have mp_obj_base_t  
```
[[MP, 'micropython/py/obj.h' ab Zeile:52]](./../../../../micropython/tree/master/py/obj.h#L52)  

Diese Base und der dazugehörige Typ werden verwendet, um das MicroPython-Objekt zu verwalten. Die MP-VM ist darauf angewiesen das dieser Typ korrekt konfiguriert ist.So wird dieser z.B. dafür verwendet, um eine Objekt-Instanz ihren zugehörigen C-Funtionen zuzuordnen. Ein Typ loses Objekt bzw. eines mit falscher Typ-Information führt zu einem Undefinierten verhalten. Im besten Fall kommt es zu einem Speicherzugriffsfehler.

Im Folgenden werden ein Paar von MicroPython bereit gestellte Konkrete-Typen genauer betrachtet.

### Module-Type

Module werden mithilfe des Daten-Typ 'mp_obj_module_t' abgebildet. Im einfachsten Fall setzen sie sich aus zwei Komponenten bzw. Definitionsschritten zusammen.
1. Erstellung eines Dictionaries, das zur Verwaltung von Modul-Metainformationen verwendet wird. Beispielsweise werden Modulname und die Init-Funktion, die beim Import des Moduls aufgerufen wird, definiert.
    ```cpp
    static const mp_rom_map_elem_t devices_globals_table[] = {
        { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_devices) },
        { MP_ROM_QSTR(MP_QSTR___init__), MP_ROM_PTR(&devices___init___obj) },
        { MP_ROM_QSTR(MP_QSTR_motor),  MP_ROM_PTR(&motor_type) },
    };
    static MP_DEFINE_CONST_DICT(devices_globals, devices_globals_table);
    ```
    Solche Dictionaries bestehen in der Regel aus einem 'MP_ROM_QSTR' und 'MP_ROM_PTR' Paar. Dies ist eine String-Zuordnung zu einem Befehl oder einer Funktionalität. Und das an diesen String gebundene MicroPython-Objekt. Dies kann auch eine als MicroPython-Objekt gekapselte Funktion, Konstante oder Modul sein. Diese werden als 'MP_ROM_PTR' gebunden. Im Wesentlichen ist dies ein Makro, welches MicroPython-Objekte vor einem Garbage-Collector-Clean schützt. Im Falle von 'MP_ROM_QSTR(MP_QSTR___name__)', handelt es sich um einen Sonderfall. Dies ist eine in MicroPython eingebaute Funktionalität. Dem Modul wird der Name 'devices' zugewiesen.
    > [!NOTE]   
    > Dies ist nicht der Import-Name, sondern der Modul-Interne-Name, den das Modul für sich selbst verwaltet. Wie zum Beispiel für die Print-Funktion. So würde `print(time)` den hier für das Time-Modul definierten Namen ausgeben.

    Es ist wichtig, solche Namen stets als sogennante QString mithilfe des MicroPython-Makros zu generieren. 
    > [!NOTE]
    > Unter [QString](#qstrings) findet sich eine qualitative Erklärung zu diesen.

    Zu letzt muss das Macro `MP_DEFINE_CONST_DICT` verwendet werden. Dieses wandelt `mp_rom_map_elem_t` zu einem MicroPython-Dictionary-Objekt um.

2. Als nächstes muss das MicroPython-Dictionary-Objekt unter einem Module registriert werden. Hierfür gibt es den vorher erwähnte typ `mp_obj_module_t`. Dieser hat für uns an dieser Stelle 2 wichtige attribute.
    ```cpp
       const mp_obj_module_t devices_module = {
        .base = { &mp_type_module },
        .globals = (mp_obj_dict_t *)&devices_globals,
        };
        MP_REGISTER_MODULE(MP_QSTR_devices, devices_module);
    ```
    Die `base`, ein Pointer, welcher die Typzuweisung für ein MicroPython-Objekt ist. Mehr dazu unter [MicroPython Typ Zuordnung](#micropython-typ-zuordnung). Die Modul-Base ist hier `&mp_type_module`. Jedes Modul hat diesen Pointer als Base. Durch diese zuweisung verwaltet die MP-VM dieses Objekt als ein Modul. Es erhält somit alle Funktionalitäten und Eigenschaften die man von einem Modul erwarten würde. Das Globals-Attribut umfasst die durch das Modul zugänglichen Klassen, Funktionen, usw. kurz gesagt das oben definierte Dictionary. `MP_REGISTER_MODULE` registriert das Modul, hier wird ein QString angegeben werden welcher den Namen des Moduls vorgibt. Dieser Name wird auch für den Import verwendet.
      
    //TODO Link & überarbeiten
    Der Datei [`micropython/py/makemodulesdef.py`](./../../../../micropython/tree/master/py/makemodulesdef.py) ist zu entnehmen, das es drei Arten gibt Module zu registrieren.
    >  `makemodulesdef.py` ist ein Python-Script weches im MicroPython-Build-Prozess verwendet wird
    
    **MP_REGISTER_MODULE** Ein Modul als *Builtin*-Modul deklarieren.  
    **MP_REGISTER_EXTENSIBLE_MODULE** Dieses Modul soll vom Dateisystem aus erweitert werden können.
    > Da wir kein Dateisystem implementieren, ist dieser Punkt uninteressant.

    **MP_REGISTER_MODULE_DELEGATION** Wird verwendet, um die Registrierung oder Initialisierung eines Moduls an eine externe Funktion zu delegieren.
    > Da die umzusetzenden Funktionen klar deffiniert sind spiel diese Modul registrierung, für dieses Projekt, keine Rolle.

### Klassen-Type
1. [Selbst Deffinierte Klassen](#selbst-deffinierte-klassen)
2. [Bereit gestellte Funktionen und Konzepte](#bereit-gestellte-funktionen-und-konzepte)

#### Selbst Deffinierte Klassen

Die Besonderheit von Klasen-Typen im Gegensatz zu Modul-Typen ist das sie ihren Typ-Pointer selber verwalten.
Ein Klassen-Typ hat immer eine Definition für den Typ-Pointer in dieser Form:
```cpp
    const mp_obj_type_t <typ_name>;
```
Die Adresse dieses Pointers ist nach Initialisierung eindeutig und einmalig. Solange er nicht fälschlicherweise verändert wird. Mehr dazu unter [MicroPython Typ Zuordnung](#micropython-typ-zuordnung). Dieser Pointer wird verwendet, um Objekte diesem Typ zuzuordnen. Dies entspricht dem `mp_type_module`
Modul-Pointer. Hier wird die erste Parallel in der Verwaltung von Objekte und Modulen klar. 

Zusätzlich zu diesem Typ-Pointer kann eine selbst definierte Struktur erstellt werden.
In der Form:
```cpp
typedef struct _<struct> {
    mp_obj_base_t base;
    <
    Frei wählbare Felder. 
    >
} <struct>_t;
```
Dieses Struct wird später an Objekt Instanzen gebunden. In diesem Struct können Informationen geschrieben werden, die man für die Verwaltung eines Objektes braucht. Das MicroPython-Objekt wäre hier eine abstrakte Referenz auf diesen Daten-Typ.

Wichtig ist das diese Struct immer über eine Objekt-Base verfügen. Alle anderen Felder, sind jedoch frei definierbar.

In diesem Projekt werden an ein Port gebundene Objekte gesondert behandelt. So wie z.B. Motoren welche nur an Port 1 bis 4 angeschlossen werden können. Es wird zusätzlich ein statisches Objekt-Array erstellt. Das die zu verwaltenden MicroPython-Objekte einmalig anlegt. So sind alle MicroPython-Objekte welche an einen Port gebunden sind, geteilt Objekte. Dies soll den MicroPython-Heap-Verbrauch minimieren und zu einem Konsistenten verwalten von angeschlossenen Geräten führen. Mehr dazu unter [Reduzierung des MicroPython-Heap-Verbrauchs durch Objektreferenzen](#reduzierung-des-micropython-heap-verbrauchs-durch-objektreferenzen) genauer erklärt.

Wie oben beschrieben verwaltet das Struct eines Objektes Informationen über dieses. Wie zum Beispiel den Port, mit dem eine Sensor-Instanz initialisiert wurde.

MicroPython-Funktionen welche an Objekt-Instanzen gebunden werden habe eine besonderheit. Sie erhalten das MicroPython-Objekt, welches sie selber sind, als erstes Argument einer jeden Funktion.
Wie bereits im Vorfeld angedeutet, ist dies vergleichbar mit dem `self`-Konzept aus der Python-Programmierung.
Hat man also eine Funktion einer Klasse:  
```cpp
static mp_obj_t get_button(mp_obj_t self_id) {
    button_obj_t *self = MP_OBJ_TO_PTR(self_id);
    bool ret = getSensorDigital(self->id);
    return mp_obj_new_bool(ret);
}
```
So ist das erste Argument, welches der Funktion mitgegeben wird das `self`. Also ein `mp_obj_t`, welches auf das korrekte Struct gecastet werden kann. Dieses wird dann verwendet, um Operationen in Relation zu dem Objekt-Zustand durchzuführen. Es sollten also alle Instanz-Spezifischen Informationen eines MicroPython-Objektes, in dessen Strukt verwaltet werden.

#### Bereit gestellte Funktionen und Konzepte

MicroPython bietet von sich aus auch verschiedene Objekt-Typen an, wie z.B. Listen oder Dictionaries, diese sind konzeptionell dasselbe wie eine Objekt-Instanz einer selbst definierten klasse. Dafür stellt MicroPython vordefinierte Modul-Klassen und Typ-Strukturen bereit. Und setzen die MicroPython-Objekte auf gleiche Art und Weise um, wie selbst definierte Klassen. 
```cpp
typedef struct _mp_obj_list_t {
    mp_obj_base_t base;
    size_t alloc;
    size_t len;
    mp_obj_t *items;
} mp_obj_list_t;

void mp_obj_list_init(mp_obj_list_t *o, size_t n);
mp_obj_t mp_obj_list_make_new(const mp_obj_type_t *type_in, size_t n_args, size_t n_kw, const mp_obj_t *args);
``` 
[[MP, 'micropython/py/objlist.h', ab Zeile:31]](./../../../../micropython/tree/master/py/objlist.h#L31-39)

Dadurch ist es möglich MicroPython-Objekte oder Module auf gleiche Art und Weise zu erweitern wie man es bei einem Selbst-Definierten-Datentyp machen würde. Zum Beispiel könnte man das hier gezeigte MicroPython-Listen-Objekt, um eine Get-Index-Funktion Erweitern.

Es gibt auch eine Vielzahl an anderen Konzepten die MicroPython-Objekte umsetzen.
Aus dem oben angegeben Code-Schnipsel lassen sich zwei dieser Konzepte herauslesen.
Der `new`-Operator, hier durch die Funktion `mp_obj_list_make_new` abgebildet.
Enthält immer den Typen des gewünschten Elements als erstes Parameter, hier angegeben als `mp_obj_type_t *type_in`. Das is der in [Klassen-Type](#klassen-type) beschriebene MicroPython-Objekt-Typ-Pointer. Und eine Variable Liste an Argumenten. Diese werden durch `size_t n_args, size_t n_kw, const mp_obj_t *args` angegeben. Vergleichbar ist diese Art, der Parameter übergab mit dem 'argc' und 'argv' einer C++-Main-Methode. Die besonderheit in MicroPython ist das die Angabe von Key-Word-Arugments erlaubt. Also zum Beispiel `sensor(port = 0)` wobei 'port' hier ein Key-Word-Argument ist.

Diese besonderen Methoden, welche durch das MicroPython-Klassen-Konzept, bereitgestellt werden. Werden in dem `MP_DEFINE_CONST_OBJ_TYPE`-Makro angegeben und somit die MP-VM angebunden.  

```cpp
MP_DEFINE_CONST_OBJ_TYPE(
    mp_type_list,
    MP_QSTR_list,
    MP_TYPE_FLAG_ITER_IS_GETITER,
    make_new, mp_obj_list_make_new,
    print, list_print,
    unary_op, list_unary_op,
    binary_op, list_binary_op,
    subscr, list_subscr,
    iter, list_getiter,
    locals_dict, &list_locals_dict
    );
```
[[, 'micropython/py/objlist.c' ab Zeile:455]](./../../../../micropython/tree/master/py/objlist.c#L455-466) 


> [!IMPORTANT]  
> Eines der wohl wichtigsten Konzept hier das Anbinden eines 'locals_dict'. Dies ist konzeptionell dasselbe wie das Globals-Dictionary der [Modul-Typen](#module-type). Hier wird eine Dictionary für Funktions-Registrierungen übergeben.

Das Listen-Obejkt bildet eine Vielzahl an diesen eingebauten Konzepten an.
An erster Stelle ist immer die `mp_obj_type_t` Referenz. An dieser Stelle bindet MicroPython, den Typ-Pointer an die definierten Funktionen. In diesem Fall ist der Typ durch `mp_type_list` gegeben. 

//TODO tabelle überarbeiten.
| MP-Konzept          |       Bedeutung     | Verwendung |  
| ------------------- | -------------   |  ------- |
| MP_QSTR_`name`      | Der Objekt Name | Dieser wird z.b. von dem get-type python operator verwendet um den Objekt-Name auszulesen | 
| make_new | Der `new` Operator  | Die Funktion die bei der erstellung einer neuen Objekt-Instanz aufgerufen wird. Hier wird das MicroPython-Objekt als return-wert wieder gegeben. |
| print      | Die zum Objekt gehörende print Funktion     | Das Binding für die Print Funktion, `print(objekt)`. |
| unary_op | Unary Operation | Dies Sind Operationen auf dem Objekt selber z.B. der boolsche vergleich|
| binary_op | Binäre Operationen | Dies sind Operationen mit anderen Objekten z.B. der `+`- oder `-`-Operator|
| locals_dict | Das Objekt-Dictionary | Im Objekt-Dictionary werden weitere Funktionen des Objektes festgehalten und Deffiniert. Für Klasse-Typen sind diese Instanz gebunden. |
| MP_TYPE_FLAG_ITER_IS_GETITER | Der Objekt Iterrier Typ  | Zuweisung der Iterator Implementation, also welche der verschiedenen MicroPython-Iteratier verfahren verwendet werden soll.  |
| subscr | Subscription | Dieses Konzept ist meistens als `Indexing` bekannt z.B. `a[1]` |
| iter | Der Iterator | Dieses Objekt kann z.B. in einer for-each loop verwendet werden. |

## Funktions-Objekt-Definition

Wie in [Klassen-Typ : Bereit gestellte Funktionen und Konzepte](#bereit-gestellte-funktionen-und-konzepte) und [Modul-Typ](#module-type) beschrieben, bieten Klassen so wie Module, die Möglichkeit ein Dictionary für Funktions-Registrierung anzubinden. Im Folgenden wird qualitativ erklärt, wie die hier anzubindenden Funktionen implementiert werden. 
 
Wie bereits beschrieben werden Funktionen mithilfe eines [QStrings](#qstrings) an ihren Funktions-Namen gebunden. Dies ist Teil des Objekt-Dictionaries:
```cpp
{ MP_ROM_QSTR(MP_QSTR_<FuntionsName>), MP_ROM_PTR(&<MicroPython-Funktions-Objekt>) },
```

Das MicroPython-Funktions-Objekt ist eine C-Funktion, welche zu einem MicroPython-Objekt konvertiert wurde. Für diesen Zweck werden verschiedene Makros angeboten.

Die Möglichkeiten MicroPython-Funktionen zu implementieren kann beliebig Komplex werden. Daher wird, die Erklärung dieses Prozesses, auf das `MP_DEFINE_CONST_FUN_OBJ_1` Makro beschränkt. Dieses ermöglicht das Erstellen eines MicroPython-Objektes aus einer C-Funktion mit einem Argument. Im wesentlich lässt sich das Verwenden dieses Makros durch das Folgende Muster darstellen:
```cpp
static mp_obj_t <FuntionsName>(mp_obj_t <arg1>) {
    <...>
    return <MicroPython-Objekt>;
}
static MP_DEFINE_CONST_FUN_OBJ_1(<MicroPython-Funktions-Objekt>, <FuntionsName>);
``` 
Es wird eine C-Funktions definiert. Diese hat ein Argument. Das Argument der C-Funktion muss ein MicroPython-Objekt sein. Das hier übergebene Argument kann mithilfe des in [MicroPython Typ Zuordnung](#micropython-typ-zuordnung) vorgestellten Konzepte auf einen Typ-Geprüft und verwendet werden. Der Rückgabewert einer in MicroPython eingebundenen Funktion ist immer ein MicroPython-Objekt. Wobei Null auch ein valider Rückgabewert ist. Die durch MicroPython bereitgestellten Makros legen ein MicroPython-Funktions-Objekt an. Diese wird in ein Dictionary eingebunden, um dieses einem Modul oder einer Klasse zuzuordnen.

Im Laufe des Projektes sind komplexere Funktionen umgesetzt worden. Um einen Einblick in Argument Parsing und Funktionen mit Variablen Parameter zu erlangen, können die [Modul-Dateien](./../ORB-Python/src/modules) dieses Projektes betrachtet werden. Hier ist jedoch anzumerken des Prozesses der MicroPython-C-API durch das Einführen von weiteren Makros teilweise abstrahiert wurde.

## MicroPython Typ Zuordnung
MicroPython stellt eine Möglichkeit bereit, um verschieden MicroPython-Object-Typen zu unterscheiden und unterschiedlich zu verarbeiten. Im Folgenden werden in diesem Zusammenhabt, ein paar Fragen geklärt:

1. [Worauf Stützt sich die Typ-Zuordnung](#worauf-stützt-sich-die-typ-zuordnung)
2. [Wie funktioniert die Typ-Zuordnung (Non-Concrete-Types)](#wie-funktioniert-die-typ-zuordnung-non-concrete-types)
2. [Wie funktioniert die Typ-Zuordnung (Concrete-Types)](#wie-funktioniert-die-typ-zuordnung-concrete-types)

### Worauf Stützt sich die Typ-Zuordnung
Wie in [MicroPython-Object-Type](#micropython-object-type) beschrieben wird der `mp_obj_t` Pointer verwendet, um zusätzliche Informationen über das MicroPython-Object zu verwalten.
Die für uns Bereit gestellten Objekt-Klassifizierungen werden wie Folgt unterschieden:

| Typ                 |       Bit     |
| ------------------- | ------------- |
| Small Int           | xxxx...xxx1   |
| QStrings            | xxxx...x010   |
| Intermediate Object | xxxx...x110   |
| MP Object Base      | xxxx...xx00   |

'Intermediate Objects' sind temporäre Objekte, die Daten während der Ausführung von C- und Python-Code austauschen und umwandeln. Diese spielen bei der Implementierung eines eigenen MicroPython-Port keine große Rolle. Das Arbeiten mit diesen Objekten ist durch die MicroPython-C-API genauso wie, mit jedem anderen Objekt. Intermedia-Objekts sind z.B. True, False, Strings und Integer. Man könnte diese als durch MicroPython bereitgestellte Standardobjekte betrachten.
QStrings sind, wie in [QStrings-Type](#qstrings) beschrieben, eine spezielle Repräsentation von Strings.
MP-Object-Base sind wie in [MicroPython-Object-Type](#micropython-object-type) beschrieben, im Wesentlichen entweder Modul-, Klassen-Objekte, Funktionen oder Konstanten bzw. Variablen. Eben solche Objekte welche eine MicroPython Objekt-Base besitzen.

Wichtig ist hier anzumerken, wie dieser Prozess der Speicherung von Typinformationen funktioniert.

MicroPython verfügt nicht über einen speziellen Mechanismus, um sicherzustellen, dass MP Object Base Pointer immer mit -xxx...xx00 enden. Stattdessen verlässt sich MicroPython hier auf Pointer-Alignment. Diese Annahme basiert darauf, dass der GCC-Compiler verwendet wird. Da der GCC-Compiler dem C-Standard folgt, geht er immer von ausgerichteten Zeigern aus. Definiert wird dies in dem C-Standart:

"3.2
alignment requirement that objects of a particular type be located on storage boundaries withaddresses that are particular multiples of a byte address" [C-s, „3.2 alignment“ , Seite 17]

Da MicroPython bis zu 3 Bits verwendet, um Typinformationen zu speichern. Nehmen die MicroPython-Entwickler hier an, dass die Kompilierung für das Zielsystem mindestens ein 3-Bit-Alignment aufweist.

Aus der ARM Compiler Toolchain Dokumentation ist zu entnehmen das alle pointer 32-Bit aligned sind. [vlg. ARM-al]

Somit ist die von den MicroPython-Entwicklern gemacht Annahme, für den von uns gewählten Mikrocontroller zulässig. Ohne zusätzliche Änderungen im Compile-Prozess vorzunehmen.

### Wie funktioniert die Typ-Zuordnung (Non-Concrete-Types)

Die oben angegebenen Daten-Typen sind im Sinne der Typ-Zuordnung ähnlich implementiert, daher hier 'Small Int' als Beispiel: 
```cpp
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A

static inline bool mp_obj_is_small_int(mp_const_obj_t o) {
    return (((mp_int_t)(o)) & 1) != 0;
}
#define MP_OBJ_SMALL_INT_VALUE(o) (((mp_int_t)(o)) >> 1)
#define MP_OBJ_NEW_SMALL_INT(small_int) ((mp_obj_t)((((mp_uint_t)(small_int)) << 1) | 1))
```
[[MP, 'micropython/py/obj.h', ab Zeile:86]](./../../../../micropython/tree/master/py/obj.h#L86-90)

Im Gegensatz zu `mp_obj_base_t`-Pointer wird hier der `mp_obj_t`-Pointer nicht als tatsächlicher Zeiger, sondern als Nutzdaten-Träger für den Wert des 'Small Int' verwendet. Und als Informations-Träger, für die Typ-Zuordnung. Dies funktioniert, da 'Small Int' weniger Bits benötigt als der Void-Pointer und somit Platz für die zusätzliche Information des Datentyps bietet. Hier ein Code-Snippet, welches die Funktionsweise verdeutlicht: 
```cpp
mp_obj_t b = MP_OBJ_NEW_SMALL_INT(0x1); //b soll den wert 1 haben
printf("Ausgabe(%d)", b); //Ausgabe(3) d.h. 0x..0011
                          //also 0x00000..x1 wir haben den typ small int
                          //mit dem Nutzdaten-Wert von (0x..011 > 1 =) 1
```

> [!NOTE]  
> Arbeitet man mit mp_obj_t und erhält fälschlicherweise einen einfachen Datentyp als Parameter seiner Methoden. So ist es wichtig, Folgendes zu beachten: Diese besonderen Datentypen werden anders behandelt. Anders als mp_obj_t, welches direkt auf ein Struct referenzieren. Vor dem Casten auf ein tatsächlichhes MicroPython-Object solle man prüfen, ob das eingabe object vom richtigen typ ist. MicroPython bietet Funktionen für diese Typ-Prüfungen an. So kann man entweder eine der in die C-API eingebauten Funktionen zur Typ-Prüfung verwenden. Wie z.B. oben beschrieben `mp_obj_is_small_int` oder die MicroPython-Objekt-Base für den Typ-Vergleich verwenden. Mehr dazu im nächsten Abschnitt.

### Wie funktioniert die Typ-Zuordnung (Concrete-Types)

Konrete-Typen sind wie bereits in [Konkrete-Typen](#konkrete-typen) beschrieben eben solche Typen, welche über eine Objekt-Base verfügen. Und damit auch über eine `mp_obj_type_t` also einen MicroPython-Objekt-Typ. 
```cpp
// Anything that wants to be a concrete MicroPython object must have mp_obj_base_t
// as its first member (small ints, qstr objs and inline floats are not concrete).
struct _mp_obj_base_t {
    const mp_obj_type_t *type MICROPY_OBJ_BASE_ALIGNMENT;
};
typedef struct _mp_obj_base_t mp_obj_base_t;
```
[[MP, 'micropython/py/obj.h', ab Zeile:52]](./../../../../micropython/tree/master/py/obj.h#L52-57):  

Aus der Typ-Tabelle in [Worauf Stützt sich die Typ-Zuordnung](#worauf-stützt-sich-die-typ-zuordnung) ist beschrieben das 'MP Object Base' eine `mp_obj_t`-Adresse mit dem Letzten Bits auf 'xx...xx00' gesetzt hat. Dies bedeutet dies sind durch das Pointer-Alignment entstandene Variablen. Wird dieses Muster bei einem MicroPython-Objekt erkannt, so kann davon ausgegangen werden, das es sich um einen Konkreten-Typ handelt. Auf diese annahme gestützt kann der `mp_obj_t`-pointer zu einem `mp_obj_base_t`- oder `mp_obj_type_t`-pointer gecastet werden. Wenn das MicroPython-Objekt korrekt konfiguriert wurde, ist nun das MicroPython-Objekt-Typ auslesbar. Und das MicroPython-Objekt kann einem Typ zugeordnet werden.  

Eben diese Operation wird durch die getType-Operation bzw. desser Wrapper-Funktion realisiert:  
```cpp
const mp_obj_type_t *MICROPY_WRAP_MP_OBJ_GET_TYPE(mp_obj_get_type)(mp_const_obj_t o_in) {
#if MICROPY_OBJ_IMMEDIATE_OBJS && MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A
    if (mp_obj_is_obj(o_in)) {
        const mp_obj_base_t *o = MP_OBJ_TO_PTR(o_in);
        return o->type;
    } else {
<...>
```
[[MP, 'micropython/py/obj.c', ab Zeile:56]](./../../../../micropython/tree/master/py/obj.c#L56-62) 

Der Typ eines MicroPython-Objektes ist durch einen Typ-Pointer von diesem Definiert. Jede MicroPython-Objekt-Datei definiert diesen für sich. Meist sieht dies wie Folgt aus:

```cpp
//Deffinieren des Typ-Pointers
const mp_obj_type_t motor_type;
//Einstellen des Objekt-Typen
motor_obj_t m = { .base = { .type = &motor_type }, <Weitere_Objekt_Felder> };
```

Es wird einfach ein Konstanter Typ-Pointer erstellt. Es wird die Einmaligkeit einer Speicher-Adresse verwendet, um jeden Objekt-Typen eine eindeutige Zuordnung zu einem Typ zu geben.

Ein solcher Typ-Vergleich könnte wie Folgt aussehen:  
```cpp
//Die oben gezeigt MICROPY_WRAP_MP_OBJ_GET_TYPE wird intern verwendet.
mp_obj_get_type(motor) == &motor_type
```

Bei Konkrete-Typen ist also eher wichtig das MicronObjekte korrekt aufgebaut sind. Wie ein Korrekt aufgebautes MicroPython-Objekt aussieht, wird im Folgenden erklärt. Unter Berücksichtigung der folgenden Einschränkungen wird eine Objelkt-Typ-Zuordung, zu einem einfachen Cast und zugriff auf das Feld einer Struktur.

Durch die Natur der 'getType'-Funktion, ist das zugreifen auf einen Objekt-Typ ein riskanter Cast. Dies funktioniert jedoch aufgrund, des gut durchdachten Speichermanagements der MicroPython-Entwickler. So wird an dieser Stelle vorausgesetzt das 'mp_obj_t'-Pointer auf Objekte Zeigen welche an der Adresse 0 ihrer Struktur, eine 'mp_obj_base_t' haben. Sonst funktioniert dieser Cast nicht. Dies ist eine Besonderheit der 'C-Structs' die von den MicroPython-Entwicklern genutzt wird.

Angenommen wir haben diese 'Struct':
```cpp
typedef struct _button_obj_t {
    mp_obj_base_t base;
    uint8_t id;
} button_obj_t;
``` 
Dann wird an der ersten Stelle des Struct. Also an Adresse 0, die 'mp_orb_base_t'. Solange dies nicht durch Optimierung verändert wurde. Auf dieses folgt, inklusive Alignment, die aneinander gereihten anderen Struct-Variablen. Der Cast funktioniert.

Betrachten wir nun dieses Struct:  
 ```cpp
typedef struct _button_obj_t {
    uint8_t id;
    mp_obj_base_t base;
} button_obj_t;
``` 
Tauschen wir die Reihenfolge der Parameter, so produziert der Typ-Zugriff einen Speicherzugriffsfehler. Da `const mp_obj_base_t *o = MP_OBJ_TO_PTR(o_in)` an dieser Stelle auf das `uint8_t id` zeigt. Wird diese Besonderheit  von MicroPython-Objekten bei der Implementierung beachtet. So ist die Typ-Prüfung ein sicherer vorgang. 

## Problematik bei der Verwendung von Namespaces
Bei dem Versuch MicroPython-Module und Klassen aufzubauen ist ein Problem aufgefallen. Es gibt Limitierungen bei der Verwendung von Namespaces. Auf diese und die daraus entstehende Problematik soll im Folgenden eingegangen werden.

- [Super- & Submodule](#super-&-submodule)
- [Limitierungen des MicroPython-Interpreters](#limitierungen-des-micropython-interpreters)
- [Submodule als QString Alias](#submodule-als-qstring-alias)
- [Kombination von Submodulen, QString und Supermodulen](#kombination-von-submodulen-qstring-und-supermodulen)
- [Schlussfolgerung zu diesen Problemen](#schlussfolgerung-zu-diesen-problemen)

### Super- & Submodule
Dies ist eine kurze Begriffs-Erklärung. Super-Module sind solche Module, welche andere Module als Teil ihres Dictionaries verwalten. Also solche Module über die man auf andere Module zugreifen kann. Ein Submodul ist ein solches Modul, welches von einem Super-Modul eingebunden wurde.

### Limitierungen des MicroPython-Interpreters

Imports von Modulen funktionieren nur eine Ebene tiefer. Hat man z.B. ein Modul `Devices` und registriert darin das Submodul `Sensors`, welches wiederum einen spezifischen Sensor wie `EV3-Lichtsensor` enthält. So könnte man erwarten, dass die Syntax `import Devices.Sensors.EV3-Lichtsensor` oder `from Devices.Sensors import EV3-Lichtsensor` funktioniert. Jedoch findet der MicroPython-Interpreter diese Module nicht.

Für solche Submodule bietet MicroPython ein alternatives Vorgehen an. Das Registrieren von 'Submodule als QString Alias'. Dieses wird im Subpackage-Beispiel des MicroPython-Projektes erklärt (siehe [[MP, 'micropython/examples/usercmodule/subpackage']](./../../../../micropython/tree/master/examples/usercmodule/subpackage)). Diese Methode bringt jedoch andere Probleme mit sich.

### Submodule als QString Alias

Ein alternatives Vorgehen für solche Submodul-Strukturen wäre, das Modul `Devices` gar nicht umzusetzen.

Man würde ein Modul `Sensor` schreiben, registriert dieses jedoch nicht bei einem Supermodul, sondern gibt ihm den Namen `MP_QSTR_devices_dot_sensors`. Anschließend muss man eine QString-Datei erstellen. Diese wird in den MicroPython-Build-Prozess eingebunden. In dieser werden die 'dot-strings' ein weiteres Mal definiert, `Q(devices.sensors)`. Dieser Schritt ist notwendig um "dot" durch "." für den Import zu ersetzen.

Die damit verbundenen Probleme sind Folgende:  
Das Devices-Modul kann man keine eigenen Funktionen verwalten. Da es praktisch gesehen gar nicht existiert. Außerdem kann das Devices-Modul selber nicht importiert werden. Die Anweisung `import devices` führt zu einem Import-Error. Es muss stattdessen `from devices.sensors import EV3-Lichtsensor` für den Import verwendet werden.

### Kombination von Submodulen, QString und Supermodulen

Man könnte auf die Idee kommen, beide Ansätze zu kombinieren. Man erstellt ein Supermodul `devices`, registriert dieses sowie das zugehörige Submodule. Das Submodule selbst wird ebenfalls registriert. Dieses Vorgehen scheint auf den ersten Blick zu funktionieren und löst die oben beschriebenen Probleme, hat jedoch auch unerwartetes Verhalten.

Führt man `import devices.sensors` aus, so scheint der Import korrekt zu funktionieren. Bei näherer Betrachtung wird jedoch nicht das Modul `sensors` importiert. Es wird hier das Modul `sensors`, unter dem Namen `devices` importiert. Führt man dann `print(devices)` aus, so wird der Name des `sensors` Moduls ausgegeben. Das Devices-Modul wurde hier überschrieben. Erst durch den Befehlt `import devices` ist dieses wieder korrekt erreichbar.

### Schlussfolgerung zu diesen Problemen

Ursprünglich war geplant, den Namespace `orb` zu verwenden, um Module zu organisieren. So könnte man Klassen, Module und Funktionen unter einem Namespace zusammenfassen. Wie z.B. `orb.sensors` oder `orb.devices`. Aufgrund der oben genannten Probleme ist dies jedoch nicht möglich. Der `orb`-Prefix muss gestrichen werden, um das von Python erwartete Verhalten zu gewährleisten.

Es gibt daher die Einschränkung, Module nur eine Ebene tief zu gestalten. Funktionen und Klassen auf einer dritten Ebene werden nur registriert, wenn sie ausschließlich im Zusammenhang mit dem Supermodul  Sinn ergeben. Wie zum Beispiel Funktionen einer Klasse, die zuvor instanziiert werden muss.

In der Praxis ist dies zum Beispiel `devices.sensor.get()`. Die Get-Funktion eines Sensors ergibt nur Sinn, falls sie im Zusammenhang mit einem Sensor-Objekt verwendet wird.


## MicroPython Flags

Die MicroPython Flags werden in der Datei `mpconfigport.h` definiert, ein Auszug aus dieser Datei könnte so aussehen.
```cpp
#define MICROPY_CONFIG_ROM_LEVEL                (MICROPY_CONFIG_ROM_LEVEL_MINIMUM)

#define MICROPY_PERSISTENT_CODE_LOAD            (1)
#define MICROPY_ENABLE_COMPILER                 (1)

#define MICROPY_ENABLE_GC                       (1)
#define MICROPY_PY_GC                           (1)
#define MICROPY_FLOAT_IMPL                      (MICROPY_FLOAT_IMPL_FLOAT)
```
Diese Flags werden benutzt, um MicroPython mitzuteilen, welche Module zu dem Port hinzugeladen werden sollen und welche nicht. Es gibt eine ganze Menge an Standard Module, welche für dieses Projekt nicht braucht zu werden. Daher ist als Erstes die Verwendung von
`#define MICROPY_CONFIG_ROM_LEVEL (MICROPY_CONFIG_ROM_LEVEL_MINIMUM)` hervorzuheben.
Dies bewirkt das keine Standard-Module verwendet werden. Es wurde also das absolut minimale Grundgerüst der MP-VM konfiguriert. 
Darauf folgen zwei weitere Wichtige Flags. 'MICROPY_PERSISTENT_CODE_LOAD' fügt den MicroPython-Byte-Interpreter hinzu. Dieser soll am Ende dieses Projektes, der einzige weg sein, MicroPython-Programme auszuführen.
`MICROPY_ENABLE_COMPILER` Hingegen in der String Interpreter, dieser war aus Bequemlichkeit Zwecken zu Beginn des Projektes noch hinzugeschaltet. Dieser konnte für Debug-Zwecke verwendet werden. Im fertigen Projekt wurde diese Flag jedoch de-aktiv sein.

## Reduzierung des MicroPython-Heap-Verbrauchs durch Objektreferenzen

MicroPython hat einen sehr begrenzten Heap-Speicher. Dies stellt einige Probleme dar, die bei der Implementierung einer MicroPython-Firmware sowie beim Arbeiten mit MicroPython selbst berücksichtigt werden müssen. Ein Problem, das in diesem Zusammenhang identifiziert wurde, wird im Folgenden erklärt. Dabei wird dies Beispielhaft für einen Daten-Typ erklärt. Dieses Problem tritt jedoch auch bei anderen Daten-Typen auf.

Betrachteten wir als Beispiel den Float-Daten-Typ. In MicroPython sind Floats MicroPython-Objekte. Die bilden das Verhalten, welches man von Floats erwarten ab. Sie also ein Konkreters-MicroPython-Objekt.
Nehmen wir an, wir haben den folgenden Code:
```python
import gc
a = 0.5
while a < 10000:
    a = a + 0.5
    print(gc.mem_free())
```

Jedes Mal, wenn a = a + 0.5 aufgerufen wird, liest die MP-VM den alten Wert von a und erstellt ein neues Objekt mit dem neuen Wert. Das alte Objekt bleibt jedoch im Speicher. Dieses kleine Programm kann sehr schnell zu einem Speichermangel führen. Grund dafür ist unzureichende Heap-Speicherkapazität. Wir können dieses Überlaufen verfolgen, indem wir den freien Speicher mit `print(gc.mem_free())` ausgeben. Eine Lösung für dieses Problem besteht darin, den integrierten Garbage-Collector zu verwenden.
> [!NOTE]  
> Dies muss nicht zwangsläufig, kann aber zu einem Fehler in der Programm-Ausführung führen. Das Beispiel oben erlaubt der MP-VM zu erkennen, das nicht genügend Heap-Speicher zur Verfügung steht. In diesem Fall wird der 'gc.collect()' Befehlt automatisch durch die MP-VM durchgeführt. Gegebenenfalls, wird die MP-VM ausführung unterbochen. Und unter bestimmten bedingungen kann dies jedoch zu einem Speicher-Zugriffs-Fehler führen. Dieser Fehler wird in [Tests,Spezificationen.md](./Tests,Spezificationen.md) unter [2.2.3. Error-Tests](./Tests,Spezificationen.md#223-error-tests): 'Hard-Fault', provoziert.

```python
import gc
a = 0.5
while a < 10000:
    a = a + 0.5
    gc.collect()
    print(gc.mem_free())
```

Wenn wir nun die Ausgabe von `gc.mem_free()` betrachten, sehen wir, dass der verwendete Speicher nicht unbegrenzt ansteigt. Dies ist eine gute Lösung, aber der Benutzer muss über dieses Problem Bescheid wissen. Und in diesem Zusammenhang über die möglichen Risiken.

Eine mögliche Lösung für dieses Problem ist das Integrieren eines Aufrufs zum Garbage Collector in die Routine zur Erstellung neuer Objekte. Also die gleichen Überprüfungen, welche MicroPython bereits für eigene Daten-Typen integriert hat. Dies könnte eine Lösung sein, wenn das Hinzufügen von möglicherweise überflüssigen Overhead kein Problem darstellt. In den meisten Fällen hat man bei Mikrocontrollern jedoch eine starke Performance-Begrenzung. Daher ist die wohl zumeist keine gute Lösung. In diesem Projekt sind am stärksten, die Klassen für Sensoren, Motoren usw. betroffen. Eben all diese Klassen, welche ein an ein Port verbundenes Gerät abbilden. Daher wurde folgende Lösung gewählt, es wurde eine Liste mit einem Objekt für jeden Port erstellt. Hier für die 2 Servo-Ports:
  
```cpp
servo_obj_t servo_obj_list[2] = {
    { .base = { .type = &servo_type }, .port = 0, .speed = 0, .angle = 0 },
    { .base = { .type = &servo_type }, .port = 1, .speed = 0, .angle = 0 },
};
```

Wann immer ein Benutzer ein neues Objekt erstellt, erhält er eine Referenz auf eines, dieser statisch erstellten Objekte. Betrachten wir nun den folgenden Code:
```python
from devices import servo
a = servo(0)
b = servo(0)
a.set(speed=10, angle=20)
```
Dieser Code-Schnipsel hat folgenden Nebeneffekt. Es wird nicht nur den Zustand von Servo-`a` aktualisieren. Sondern auch Servo-`b` beeinflussen. Da diese ein gemeinsames Objekt verwalten. Auf diese Weise treten keine Speicherprobleme, im Zusammenhang mit dem zuvor erwähnten Problem auf. Zusätzlich hat der Benutzer immer Objekte, die den aktuellen Zustand der realen Geräte oder zumindest der angewendeten Einstellungen repräsentieren.

Betrachten wir nun diesen Python-Code:  
```python
a = servo(0)
b = servo(0)
a.set(speed=20)
b.set(angle=30)
```
Der Servo wird auf `angle(30)` mit `speed(20)` bewegen. Es wird also zusammenfassen ein Konsistentes verhalten zwischen der verschiedenen Devices-Objekten gewährleistet. Und der MicroPython-Heap-Verbrauch minimiert.

## Thread Safety
MicroPython ist nicht Thread-Safe. Dies ist bei der Implementierung der MicroPython-Task zu beachten. MicroPython selber sollte so lange nicht anders möglich seinen Speicher-Bereich vollständig selber verwalten. Die MicroPython-Task sollte vollständig losgelöst von der Restlichen Firmware laufen. Kommunikation zwischen ORB-Firmware und Python-VM sollte nur über Flags geschehen die immer nur von einer der beiden Seiten beschrieben werden können. Dadurch entstehende Race-Conditions sollten bedacht werden.
> MicroPython selber bietet die Möglichkeit an Threads zu verwalten. Dies jedoch nur für ein Thread-Modul welches in der MP-VM, in sich geschlossen arbeitet. Dafür muss man die in der Datei `mpthread.h` definierten Funktionen umsetzen. Da die ORB-Application ab von sich aus keine Threads anbietet und das Umsetzen dieser somit außerhalb des Umfangs dieser Bachelorarbeit liegt, wird dies nicht weiter beachtet.

## Windows-Bug: Falsches Register bei Non-Local Return-Adressierung 
- [Definition und Funktionsweise des Non-Local Return](#definition-und-funktionsweise-des-non-local-return)
- [Beschreibung des Bugs](#beschreibung-des-bugs)
- [Vorgehensweise zur Fehlerbehebung](#vorgehensweise-zur-fehlerbehebung)  

### Definition und Funktionsweise des Non-Local Return

Der Non-Local Return (NLR) wird in der MicroPython-Umgebung verwendet, um bei Ausnahmen oder Fehlern schnell aus tiefer verschachtelten Funktionsaufrufen herauszuspringen. Dabei ermöglicht NLR es, den normalen Programmablauf zu unterbrechen und direkt zu einem vorher definierten Punkt im Code zurückzukehren. Ohne erst den vollständigen Funktionsaufruf nach oben hin wieder abzuwickeln. Z.B. bei dem Auftreten von Exceptions, hier muss der Programmablauf effektiv und vor allem Speicher und Rechen-Leistung effizient unterbrochen werden können.

//TODO richtiges Zitat
### Beschreibung des Bugs

Führt man den MicroPython-Embed-Port unter Windows aus, ohne die Optimierungs-Flag `-fomit-frame-pointer` zusetzen. So kommt es zu einem Speicherzugriffsfehler, nach der Ausführung der Funktion `nlr_push` zu finden in der Datei `micropython/py/nlrx64.c`. Diese Option ist in `-Os` & `-Og` enthalten, wie in der GNU-GCC-Dokumentation beschrieben ist. [[vgl. GCC-cf]](https://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html) Also in den beiden als Sicher angesehenen Optimierungs-Flages für dieses Projekt. Mehr zu den Optimierungs-Flages unter [Compiler Flag Kompatibilität](#compiler-flag-kompatibilität).   
"-fomit-frame-pointer  
<...>    
Enabled by default at -O1 and higher."  
[[GCC-cf]](https://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html)

>[!NOTE]  
>Da `-Os` alle Optimierungen von `-O1` durchführt. Und `-Os` alle von `-O2`.
(Beiden Optimierungen haben Ausnahmen, zu denen `-fomit-frame-pointer` jedoch in beiden Fällen nicht zählt).

Der Grund für diesen Fehler ist, die annahm das MicroPython an folgender Stelle, die `omit-frame-pointer`-Anweisung voraussetzt.
```cpp
#if !MICROPY_NLR_OS_WINDOWS
#if defined(__clang__) || (defined(__GNUC__) && __GNUC__ >= 8)
#define USE_NAKED 1
#else
// On older gcc the equivalent here is to force omit-frame-pointer
__attribute__((optimize("omit-frame-pointer")))
#endif
//Hier müsste Windows eigentlich sein eigenes Handling für omit-frame-pointer haben
#endif
```  
[[MP, 'micropython/py/nlrx64.c', ab Zeile 38]](./../../../../micropython/tree/master/py/nlrx64.c#L38-44)

Ungünstig in an dieser Stelle nur das der Fall für Windows hier gar nicht beachtet wird. Das Windows diese Optimisierungs-Anweisung jedoch erwartet, auch wenn nicht hier angeführt, kann man aus der Funktion jedoch heraus lesen.

Es handelt sich hier um ein Problem das im Rahmen der nlr-Assembler-Codes auftritt, daher ein wenig Kontext zu diesem Problem. Bei diesem Problem spielen Zwei Register eine Rolle, rbp und rbp.

Wenn der Compiler rbp als Frame Pointer verwendet,

> [!NOTE]  
> Dies könnte man als non-omitted bezeichnen
  
sorgt er dafür, dass rbp auf den Anfang des Stack-Rahmens zeigt. Ein Stack-Rahmen speichert die lokalen Variablen, Funktionsargumente und die Rücksprungadresse einer Funktion. Ist rbp als Frame-Pointer verwendet so kann man diese Register nicht mehr frei verwendet. Es ist kein General-Purpose-Register mehr.

> "-fomit-frame-pointer  
> Omit the frame pointer in functions that don’t need one. This avoids the instructions to save, set up and restore the frame  pointer; on many targets it also makes an extra register available."  
[[GCC-cf]](https://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html)

Der Quell-Code der betroffenen Funktion sieht wie Folgt aus:   
```cpp
unsigned int nlr_push(nlr_buf_t *nlr) {
    #if !USE_NAKED
    (void)nlr;
    #endif

    #if MICROPY_NLR_OS_WINDOWS

    __asm volatile (
        "movq   (%rsp), %rax        \n" // load return %rip
        "movq   %rax, 16(%rcx)      \n" // store %rip into nlr_buf
        "movq   %rbp, 24(%rcx)      \n" // store %rbp into nlr_buf
        "movq   %rsp, 32(%rcx)      \n" // store %rsp into nlr_buf
        "movq   %rbx, 40(%rcx)      \n" // store %rbx into nlr_buf
        "movq   %r12, 48(%rcx)      \n" // store %r12 into nlr_buf
        "movq   %r13, 56(%rcx)      \n" // store %r13 into nlr_buf
        "movq   %r14, 64(%rcx)      \n" // store %r14 into nlr_buf
        "movq   %r15, 72(%rcx)      \n" // store %r15 into nlr_buf
        "movq   %rdi, 80(%rcx)      \n" // store %rdr into nlr_buf
        "movq   %rsi, 88(%rcx)      \n" // store %rsi into nlr_buf
        "jmp    nlr_push_tail       \n" // do the rest in C
        );

    #else
```
[[MP, 'micropython/py/nlrx64.c', ab Zeile:59]](./../../../../micropython/tree/master/py/nlrx64.c#L59-81)

Hier ist an folgender Zeile zu erkennen, was Falsch läuft.
```cpp 
`movq   (%rsp), %rax        \n" // load return %rip`  
```
Wie auch im Kommentar steht wird hier die Adresse für den return Sprung geschrieben, diese jedoch nicht nach rbp, sondern nach rsp.
 
Das rsp-Register ist das Stack Pointer Register. Wird rbp 'omitted' so wird das rsp-Register direkt verwendet, um das Stack-Top zu verwalten. Also wo sich die Rücksprung-Adresse befindet.
  
Das heißt, hier ist es notwendig das rsp und nicht rbp von dem Compiler als 'Rücksprung-Zuständiger' erkannt wird. Falls nicht, ist das Verhalten undefiniert und wir bekommen im besten Fall einen Speicher-Zugriffs-Fehler, können somit erkennen das hier etwas schiefgelaufen ist.

### Vorgehensweise zur Fehlerbehebung

Da dieses Problem nur unter Windows eine Rolle spielt und für den STM32F405 Mikrocontroller dies Problem nicht auftritt. Reicht es  mit Compiler-Flags zu Compilieren welche `-fomit-frame-pointer` setzen. Also `-Os` oder auch `-Og`. Das ORB verwendet in seinem Compilier-Prozess `-O0`, jedoch gibt es diesen Bug nicht bei der Verwendung mit diesem Mikrocontroller. Das bedeutet, __attribute__((optimize("omit-frame-pointer"))) wird korrekt gesetzt. Da die für diesen Mikrocontroller zuständigen Dateien dies korrekt implementieren. Im Verlaufe des Projektes wurde die `nlrx64.c`-Datei um Logik für das setzen des Omit-Frame-Pointer-Attributes erweitert. Es muss nun nicht mehr des Flag `-fomit-frame-pointer` für den gesammten Build-Prozess gesetzt werden. Dies bedeutet auch, das Windows-Projekt kann mit `-O0`, genauso wie das Firmware-Projekt compiliert werden.

## Compiler Flag Kompatibilität
> [!Caution]  
>  Compiler-Flags sind ein Stolperstein für alle die einen eigenen MicroPython-Port aufbauen wollen oder wie ich, in ein bereit bestehendes Projekt Integrieren wollen. Ist man bei seinem Ausgangsprojekt (in meinem Fall ORB) an bestimmte Compiler Flags gebunden, so kann es grundsätzlich unmöglich oder sehr umständlich sein MicroPython in sein Projekt zu integrieren. Die Betrachtung der Kompatiblen Compiler-Flags sollte also vor der Implementierung des Ports getestet und gut durchdacht werden.

- [MicroPython Compiler Flags](#micropython-compiler-flags)
- [ORB-Firmware Compiler Flags](#orb-firmware-compiler-flags)
- [Angepassten Compiler Flags](#angepassten-compiler-flags)

### MicroPython Compiler Flags
Möchte man MicroPython Compilieren und auch Debuggen können, so ist es notwendig '-Og' als Flags zu setzen.
Dieses Flag sorgt für eine moderate Optimierung. Für uns ist an dieser Stelle aber wichtig die Debug-Informationen erhalten bleiben. Durch die Verwendung dieser Flag ist es möglich in der Code::Blocks Umgebung MicroPython Code sinnvoll zu debuggen.

> Die Verwendung dieser Flag erfüllt zugleich für uns einen zweiten Zweck. Der Dokumentation der GNU-GCC-Dokumentation ist zu entnehmen, das viele Optimierung-Flags durch das Verwenden von -Og unterdrückt werden.
Wie man im Kapitel 3.11 Options That Control Optimization nachlesen kann: "Most optimizations are completely disabled at -O0 or if an -O level is not set on the command line, even if individual optimization flags are specified. Similarly, -Og suppresses many optimization passes." [[vlg. GCC-cf]](https://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html)

Auch wenn es noch viele weitere gibt hier exemplarisch die `-ftree-pta` Flag:
>```cpp 
>-ftree-pta
>    Perform function-local points-to analysis on trees. 
>    This flag is enabled by default at -O1 and higher, except for -Og.
>```
[[GCC-cf]](https://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html)

Dieser Ansatz minimiert also das Risiko von Problemen, welche durch aggressivere
Optimierungen provoziert werden können. Das dies auch wirklich so ist lässt sich einfach ausprobieren. 
Test weise wurde die `-Og` Flag entfernt und die `-O3` Optimierung verwendet. Dies ist eine sehr aggressive Optimierung. Wird der MicroPython-Port der mit der richtigen Compiler Flag problemlos funktioniert, gebaut. So bekommt man schon vor dem Ausführen der ersten Python-Zeile den Fehler-Code `0xC0000005`. dies ist ein Speicher zugriffs Fehler. 

> [!NOTE]
> Im aktuellen Stand dieses Projektes lässt sich dieser Fehler nicht mehr Produzieren. Es ist jedoch möglich, diesen durch das Verwenden des Embed-Example-Port zu produzieren. 

Aufgrund der Natur von MicroPython, das stark auf das Casten von Zeigern und das dynamische Aufrufen von Funktionen basiert. Und das durch diese Operationen das dynamische aufrufen von Python-Funktionen realisiert. 
Ist es entscheidend, dass Optimierungen die beispielsweise ungenutzte MicroPython-Module entfernt nicht genutzt werden. Daher sollte im besten Fall auf aggressive Optimierungen verzichtet werden, um die Funktionsfähigkeit und Stabilität des Systems zu gewährleisten.

Ist das Debuggen nicht mehr notwendig, so ist es möglich `-Os` zu verwenden, um Speicherplatz zu sparen. Abgesehen von der geringen Programm-Größe, bietet dieses Flag jedoch keinen Vorteil. Wichtig anzumerken ist das dies die einzige aggressive Optimierung-Flag ist, welche hier als sicher im Rahmen der Verwendung mit MicroPython angesehen wird. Da sich diese in einer Vielzahl der MicroPython-Ports finden lässt.

### ORB-Firmware Compiler Flags
Die ORB-Firmware hat als vor Configurierte Optimizierunge `-O0` eingestellt. Zusätzlich sind die Flags: 
`-ffunction-sections` und `-fdata-sections` eingestellt. Diese Optionen Isolieren Daten und Funktionen jeweil in ein eigenes Segment. Aus der GNU GCC Dokumentation ist zu entnehmen das diese Flags nur den generierten Code sortieren. Jedoch nicht ungenutzten Code entfernen. Für das Entfernen hier "garbage collection" von ungenutzten code müsste man die Flag ` -Wl,--gc-sections` setzen. [[vgl. GCC-co]](https://gcc.gnu.org/onlinedocs/gnat_ugn/Compilation-options.html) Daher würde ich diese Flags als eher unproblematisch einstufen. Zusätzliches Testen konnte validieren, das diese Compiler-Flags mit dem MicroPython-Projekt kompatibel  sind.  
Außerdem ist in dem ORB-Firmware-Projekt configuriert, das Floating-Pointer Operationen als **"FPU-specific calling convention"** generiert werden. Für uns bedeutet dies das wir Float-Operationen als Daten-Typ verwenden können. Dabei müssen wir uns keine Gedanken über die genaue verwendung von Floats auf der FPU des Microcontrollers machen. Alle anderen Flags sind für Compiler-Warnings und zusätzliche Debug-Informationen.

### Angepassten Compiler Flags
Da die Compiler Flags der ORB-Firmware ohne hin schon kompatiblem mit den erlaubten Compiler-Flags des MicroPython-Ports sind müssen keine weiteren Änderungen vorgenommen werden. Das Fertige Projekt soll mit `-O0` und den zusätzlichen Flags der ORB-Firmware compiliert werden.  

Wird ein höherer grad an Optimierung vonnöten sein. Zum Beispiel durch unzureichenden Speicherplatz. So sollte im besten fall die Optimierungs-Stufe `-Os` verwendet werden und die Kompatibilität von dieser im Zusammenhang mit der ORB-Firmware getestet werden. Zu diesem Zeitpunkt ist jedoch keine zusätzliche Optimierung notwendig. Dies könnte jedoch bei zusätzlicher Erweiterung der ORB-Firmware durch zum Beispiel zusätzliche Funktionen in der Zukunft erforderlich sein.  

### QStrings

QStrings sind Strings welche gehashed wurden. Sie werden über diesen Hash-Wert verwaltet und zum Beispiel auf Gleichheit geprüft. Im wesentlich sind QString nur eine Zahl, die einen String repräsentieren. Die MicroPython-C-Api hat dabei interne Vorgänge, welche Kollisionen handhabt. Es gibt zwei Arten von QStrings. Solche, die als Dynamisch bezeichnet werden können, diese werden zu Laufzeit berechnet. Und solche die im MicroPython-Port-Pre-Compile-Schritt zugeordnet werden. Letzteres wird für Modul-, Klassen oder allgemein MicroPython-Objekt-Informationen verwendet. Diese sind an dem Präfix 'MP_QSTR' erkennbar. Durch dieses vorgehen, muss ihr Wert nicht zu Laufzeit berechnet werden. Wird jedoch eine QString dieser Form verändert, so muss der MicroPython-Port erneut gebaut werden. In diesem Projekt wird dieser Schritt auch als 'Rebuild-MicroPython' bezeichnet.
