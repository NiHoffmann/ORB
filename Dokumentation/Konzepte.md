# Inhaltsverzeichnis

- [Warum den MicroPython Embed Port verwenden?](#warum-den-micropython-embed-port-verwenden)
- [Micropython-Types](#micropython-types)  
    - [Micropython-Object-Type](#micropython-object-type)
    - [Konkrete-Typen](#konkrete-typen)
        - [Module-Type](#module-type)  
        - [Klassen-Type](#klassen-type)//hier noch die unter-titel eintragen wenn fertig.
            - [Selbst Deffinierte Klassen](#selbst-deffinierte-klassen)
            - [Bereit gestellte Funktionen und Konzepte](#bereit-gestellte-funktionen-und-konzepte)
- [Micropython Typ Zuordnung](#micropython-typ-zuordnung)
    - [Worauf Stützt sich die Typ-Zuordnung](#worauf-stützt-sich-die-typ-zuordnung)
    - [Wie Funktioniert die Typ-Zuordnung (Non-Concrete-Types)](#wie-funktioniert-die-typ-zuordnung-non-concrete-types)
    - [Wie Funktioniert die Typ-Zuordnung (Concrete-Types)](#wie-funktioniert-die-typ-zuordnung-concrete-types)
- [Problematik bei der Verwendung von Namespaces](#problematik-bei-der-verwendung-von-namespaces) 
    - [Super- & Submodule](#super-&-submodule)
    - [Dictionaries und QStrings](#dictionaries-und-qstrings)
    - [Submodule als QString Alias](#submodule-als-qstring-alias)
    - [Kombination von Submodulen, QString und Supermodulen](#kombination-von-submodulen-qstring-und-supermodulen)
    - [Schlussfolgerung zu diesen Problemen](#schlussfolgerung-zu-diesen-problemen)
    - [Limitierungen des MicroPython-Interpreters](#limitierungen-des-micropython-interpreters)//noch nicht sortiert
- [Micropython Flags](#micropython-flags)
- [Reduzierung des Micropython-Heap-Verbrauchs durch Objektreferenzen](#reduzierung-des-micropython-heap-verbrauchs-durch-objektreferenzen)
- [Thread Safety](#thread-safety)
- [Windows-Bug: Falsches Register bei Non-Local Return-Adressierung](#windows-bug-falsches-register-bei-non-local-return-adressierung)
    - [Definition und Funktionsweise des Non-Local Return](#definition-und-funktionsweise-des-non-local-return)
    - [Beschreibung des Bugs](#beschreibung-des-bugs)
    - [Vorgehensweise zur Fehlerbehebung](#vorgehensweise-zur-fehlerbehebung)   
- [Compiler Flag Kompatibilität](#compiler-flag-kompatibilität)
    - [Micropython Compiler Flags](#micropython-compiler-flags)
    - [ORB-Firmware Compiler Flags](#orb-firmware-compiler-flags)
    - [Angepassten Compiler Flags](#angepassten-compiler-flags)
- [Argument Parsing](#argument-parsing)  
//TODO vorschriften/diagramme zuende machen und ins dokument bringen.  
//das hier ist noch nicht nummeriert da sich noch einiges ändern kann 
- [System Boundaries]
- [Design Flow] 
- [Argument Parsing] 

//Das hier kommt aufjedenfall noch rein

## Warum den MicroPython Embed Port verwenden?

Im Rahmen meiner Bachelorarbeit habe ich zwei mögliche Ansätze zur Integration des MicroPython-Interpreters in die ORB-Firmware identifiziert. Der erste Ansatz basiert auf der Verwendung eines der MicroPython STM-Ports (Wie z.B. der [ADAFRUIT_F405_EXPRESS-Port](https://github.com/micropython/micropython/tree/master/ports/stm32/boards/ADAFRUIT_F405_EXPRESS)). Dieser Ansatz würde die MicroPython-Umgebung als Grundlage nutzen. Somit müsste die ORB-Firmware als erweiterung des Micropython-Port eingebunden werden. Allerdings sehe ich diesen Weg als eher unpraktisch. Da er Komplikationen im Build-Prozess mit sich bringen könnte. Möglicherweise wären auch umfangreiche Anpassungen an dem Micropython-Port vorzunehmen. Grade durch die Natur der ORB-Firmware als C++-Projekt.

Da die ORB-Firmware idealerweise nur an den notwendigsten Stellen angepasst werden solltn, ist dies kein guter Ansatz. Wie bereits im Exposé beschrieben, ist mein Ziel, den MicroPython-Interpreter als eigenständige Komponente einzubinden. Somit ist dies nicht das erwünschte Vorgehen.

Der zweite Ansatz ist die Verwendung des MicroPython Embed Ports. Dieser Ansatz bietet einen klaren Vorteil. Der MicroPython-Interpreter kann eigenständige Komponente in die ORB-Firmware eingebunden werden kann. Und dies ohne dass die Firmware selbst stark verändert werden muss. Es müssen lediglich sinnvolle Schnittstellen definiert werden. Sowohl C++-Klassen um die Micropython-VM bereit zustellen. Als auch Schnittstellen der ORB-Firmware, welche die C++- auf C-Funktionen abbilden sollen und dadurch verwendbar im Micropython-Code werden.

Gleichzeitig hat dieser Ansatz den Vorteil das ich den Embed-Port auch unter Windows compiliert werden kann. Hier kann ich mithilfe von Code::Blocks oder einer anderen Entwicklungs-Umgebung eine Debug-Umgebung geschafft werden. Mit hilfe von dieser sollte der Prozess zur Entwicklung für Micropython bedeutend erleichtert werden. 

Aufgrund diese Vorteile habe ich mich für den zweiten Ansatz entschieden.

## Micropython-Types
1. [Micropython-Object-Type](#micropython-object-type)  
2. [Module-Type](#module-type)
3. [Klassen-Typ](#klassen-type)  

Für die Umsetzung meines Projektes sind 3 Arten von Types die mit Micropython abgebildet werden können wichtig. Die Objekt-, Modul- und Typ-Klassen. Im Folgenden werden diese Genauer erklärt.

#### Micropython-Object-Type
Der Micropython-Object-Typ (mp_obj_t) ist ein abstrakter Zeiger.  
Dieser wird genutzt wird, um verschiedene Objekttypen generisch zu behandeln.  
`mp_obj_t` ist wie folgt definiert:  
(obj.h zeile:40)
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
Da der MICROPY_OBJ_REPR-Modus in unserem Projekt durch die Standardkonfiguration vorgegeben ist, verwenden wir MICROPY_OBJ_REPR_A. 
(aus mpconfig.h des embed-ports Zeilt:153)  
```cpp
#ifndef MICROPY_OBJ_REPR
#define MICROPY_OBJ_REPR (MICROPY_OBJ_REPR_A)
#endif
```
Damit betrachten wir `mp_obj_t` als Alias für einen `void-Pointer`, also einen typfreien Zeiger.  
  
Aus dem Kommentar zu MICROPY_OBJ_REPR_A lässt sich eine weitere Erkenntnis ableiten.
```cpp
// A MicroPython object is a machine word having the following form:
//  - xxxx...xxx1 : a small int, bits 1 and above are the value
//  - xxxx...x010 : a qstr, bits 3 and above are the value
//  - xxxx...x110 : an immediate object, bits 3 and above are the value
//  - xxxx...xx00 : a pointer to an mp_obj_base_t (unless a fake object)
#define MICROPY_OBJ_REPR_A (0)
```
`mp_obj_t` wird nicht ausschließlich als void-Pointer genutzt. Sondern enthält auch Informationen über das gespeicherte Objekt. In einigen Fällen, wie bei small int, wird der variable Wert im Pointer gespeichert. Dafür werden die hier beschriebenen gesetzten Bits später für Typ-Zuordnung verwendet.
> [NOTE!]
> Mehr dazu unter [Micropython-Typ-Zuordnung](#micropython-typ-zuordnung).

Da jetzt geklärt ist was der Micropython Object Type ist. Kann nun geklärt werden welche Rolle er spielt, wo und wie er verwendet wird.

Funktionen, die in MicroPython geschrieben werden und auf Python-Objekten arbeiten , erwarten dies stets als mp_obj_t. Dazu gehört auch self, wie in folgendem Beispiel (`button.get()`):
```cpp
static mp_obj_t get_button(mp_obj_t self_id) {
    button_obj_t *self = MP_OBJ_TO_PTR(self_id);
    bool ret = getSensorDigital(self->id);
    return mp_obj_new_bool(ret);
}
```
Auch der Rückgabewert einer Methode ist immer vom Typ mp_obj_t. Selbst spezielle Rückgabewerte wie None sind als solche Objekte gekapselt.  Beispielsweise mp_const_none, der das `None`-"Objekt":  
(aus obj.h)  
```cpp
#define mp_const_none (MP_OBJ_FROM_PTR(&mp_const_none_obj))
```
> [NOTE!]  
> Das Makro `MP_OBJ_FROM_PTR` wandelt einen Zeiger in ein `mp_obj_t` um.  

Primitive-Typen wie z.b. Integer werden als `mp_obj_t` von Methoden zurück gegeben. Bzw. falls diese argument sind werden diese als solches übergeben. Hier ist zu beachten das Small Int wie oben beschrieben eine sonderrolle haben. "Vollwertige" Integer werden als Objecte verwaltet und haben ein mp_orb_int_t struct.  
Der unterschied im Zugriff lässt sich an der Folgenden Methode gut erkennen:  
(objint_longlong.c Zeil: 284)  
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

Diese Daten-Typen verhalten sich wie somit Konzeptionell wie jedes andere Micropython-Object. Arbeitet man mit selbst implementierten oder bereit gestellten Objekten, so sieht in zugriff in der Regel wie folgt aus:
```cpp
<...>
static void mp_funktion(..., mp_obj_t obj_input, ...){
button_obj_t *button = MP_OBJ_TO_PTR(obj_input);
<...>
```
Man hat ein Struct welches das Object beschreibt in diesem Fall `button_obj_t`. Mithilfe des Macros `MP_OBJ_TO_PTR` wird aus dem `mp_obj_t`
ein Pointer zu der eigentlichen Richtigen Structur. 
> [!NOTE]  
>Wobei diese anweisung ein einfacher cast ist, und eher Syntaktischer Zucker 
>als eine komplexe Operation:  
>```cpp
>#define MP_OBJ_TO_PTR(o) ((void *)(o))
>```
> Angesichts der Komplexität des Micropython Projektes ist eine solche
> Vereinfachung eine große Hilfe und erleichtert vorallem die Lesbarkeit von
> Micropython C-API Code.

Es ist wichtig, den Cast nur vorzunehmen, wenn Typ-Sicherheit gegeben ist. 
MicroPython bietet jedoch auch eine Möglichkeit für Typprüfungen an.

Ein solcher vergleich könnte so aussehen:  
```cpp
mp_obj_get_type(self_id) == &button_type
```
Die für diese Operation Relevante Stelle sieht wie folgt aus:
```cpp
    if (mp_obj_is_small_int(o_in)) {
        return &mp_type_int;
    } else if (mp_obj_is_qstr(o_in)) {
        return &mp_type_str;
        #if MICROPY_PY_BUILTINS_FLOAT && ( \
            MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D)
    } else if (mp_obj_is_float(o_in)) {
        return &mp_type_float;
        #endif
    #if MICROPY_OBJ_IMMEDIATE_OBJS
    } else if (mp_obj_is_immediate_obj(o_in)) {
        static const mp_obj_type_t *const types[2] = {&mp_type_NoneType, &mp_type_bool};
        return types[MP_OBJ_IMMEDIATE_OBJ_VALUE(o_in) & 1];
    #endif
    } else {
        const mp_obj_base_t *o = MP_OBJ_TO_PTR(o_in);
        return o->type;
    }
```
MicroPython versucht, bekannte Typen aufzulösen. Findet es keinen passenden "besonderen" Typ, wird angenommen, dass es sich um eine Struktur mit mp_obj_base_t als Teil der Struktur handelt: 
```cpp
    const mp_obj_base_t *o = MP_OBJ_TO_PTR(o_in);
    return o->type;
``` 
Dies stellt an dieser Stelle einen riskanten Cast dar. Er funktioniert jedoch aufgrund des gut durchdachten Speichermanagements der MicroPython-Entwickler. 
So wird an dieser Stelle voraus gesetzt das `mp_obj_t`-pointer auf Objecte Zeigen welche an der Addresse 0 ihres Structs eine `mp_obj_base_t` haben. Sonst funktioniert dieser Cast nicht. Dies ist eine besonderheit der C-Structs die von der Micropython-Entwicklern genutzt wird.

Angenommen wir haben diese Strukt:
```cpp
typedef struct _button_obj_t {
    mp_obj_base_t base;
    uint8_t id;
} button_obj_t;
``` 
Dann wird an der ersten Stelle des Struct. Also an Addresse(0) die `mp_orb_base_t`. C-Structs, solange diese nicht durch Optimierung verändert werden sind einfach nur (inclusive Alignment) die aneinander gereihten Datentypen. 
Somit funktioniert der Cast Funktioniert.   
Betrachten wir nun dieses Struct:   
 ```cpp
typedef struct _button_obj_t {
    uint8_t id;
    mp_obj_base_t base;
} button_obj_t;
``` 
Tauschen wir die Reihenfolge der Parameter so bekommen wir bei diesem Check einen Speicherzugriffs fehler. Da `const mp_obj_base_t *o = MP_OBJ_TO_PTR(o_in)` an dieser Stelle auf das `uint8_t id` zeigt.  

### Konkrete-Typen
Zu den Konkreten-Typen gehören Module, Klassen & Objekt-Instanzen. Dies sind objecte welche eine `mp_obj_base_t` an der Stelle ihres `mp_orb_t`-pointers erwarten. 
> (obj.h zeile 52):   
"// Anything that wants to be a concrete MicroPython object must have mp_obj_base_t  
// as its first member (small ints, qstr objs and inline floats are not concrete)."

Diese Base und der dazu gehörige Typ werden verwendet um das Micropython-Objekt zu verwalten. Mehr dazu im Folgenden und unter [Micropython Typ Zuordnung](#micropython-typ-zuordnung).

### Module-Type
Module werden mit Hilfe des Daten-Typ `mp_obj_module_t` abgebildet. Im einfachsten Fall setzen sie sich aus zwei Komponenten bzw. Definitionsschritten zusammen.
1. Erstellung eines Dictionaries, das zur Verwaltung von Modul-Metainformationen verwendet wird. Beispielsweise werden Modulname und die Init-Funktion, die beim Import des Moduls aufgerufen wird, verwaltet.
    ```cpp
    static const mp_rom_map_elem_t devices_globals_table[] = {
        { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_devices) },
        { MP_ROM_QSTR(MP_QSTR___init__), MP_ROM_PTR(&devices___init___obj) },
        { MP_ROM_QSTR(MP_QSTR_motor),  MP_ROM_PTR(&motor_type) },
    };
    static MP_DEFINE_CONST_DICT(devices_globals, devices_globals_table);
    ```
    Solche Dictionaries bestehen idr. aus einem paar aus `MP_ROM_QSTR` hier wird der Name der Funktionalität deffiniert. Hier ist `___name___` eine in Micropython eingabaute Funktion.
    Dem Modul wird der Name 'devices' zugewiesen. Dies ist nicht der Importname, sondern der interne Name, den das Modul für sich selbst verwaltet. 
Es ist wichtig, solche Namen stets als QString mithilfe des MicroPython-Makros zu generieren. Die Zeile `{ MP_ROM_QSTR(MP_QSTR_motor), MP_ROM_PTR(&motor_type) },` dient zur Registrierung eines MicroPython-Class-Types im Modul. Diese ist dann später über das Modul erreichbar. Zu letzt muss man das `mp_rom_map_elem_t` array zu einem Dictionary umwandeln. Dafür verwende ich das makro `MP_DEFINE_CONST_DICT`.   

2. Als nächstes muss das Dictionary unter einem Module registriert werden. Hierfür gibt es den vorher erwähnte typ `mp_obj_module_t`. Dieser hat für uns an dieser Stelle 2 wichtige attribute.
    ```cpp
       const mp_obj_module_t devices_module = {
        .base = { &mp_type_module },
        .globals = (mp_obj_dict_t *)&devices_globals,
        };
        MP_REGISTER_MODULE(MP_QSTR_devices, devices_module);
    ```
    Die base, eine Pointer welcher, wie in [Micropython-Object-Type](#micropython-object-type) beschrieben, die Typ zuweisung für den Micropython-Interpreter ist. Die Modul base ist hier `&mp_type_module` jedes modul hat diesen pointer als Base. Und erhält somit 
    alle Funktionalitäten und Eigenschaften die man von einem Modul erwarten würde. Das globals-Attribut umfasst die durch das Modul zugänglichen Klassen, Funktionen usw., kurz gesagt das oben deffinierte Dictionary. `MP_REGISTER_MODULE` registriert das Module, hier wird ein QString angegeben werden welcher den Name des Moduls vorgibt. Dieser Name wird auch für den import verwendet.
      
    Aus der Datei `makemodulesdef.py` ist zu entnehmen, das es 3 arten gibt Module zu registrieren.
    >  `makemodulesdef.py` ist ein Python-Script weches im Micropython-Build-Prozess verwendet wird
    
    **MP_REGISTER_MODULE** Ein Modul als *Builtin*-Modul deklarieren.
    **MP_REGISTER_EXTENSIBLE_MODULE** Dieses Modul soll vom Dateisystem aus erweitert werden können.
    > Da wir kein Dateisystem implementieren, ist dieser Punkt uninteressant.

    **MP_REGISTER_MODULE_DELEGATION** Wird verwendet, um die Registrierung oder Initialisierung eines Moduls an eine externe Funktion zu delegieren.
    > Da die umzusetzenden Funktionen klar deffiniert sind spiel diese Modul registrierung keine Rolle.

### Klassen-Type
1. [Selbst Deffinierte Klassen](#selbst-deffinierte-klassen)
2. [Bereit gestellte Funktionen und Konzepte](#bereit-gestellte-funktionen-und-konzepte)

Die Besonderheit von Klasen-Typen im gegensatz zu Modul-Typen, ist das sie ihren Typ-Pointer selber verwalten. 
Ein Klassen-Typ hat immer eine Deffinition für den Typ-Pointer in dieser Form:
```cpp
    const mp_obj_type_t <typ_name>;
```
Die Addresse dieses Pointers ist nach Initialisierung eindeutig und einmahlig. Solange nicht fälschlicherweise auf ihn zugegriffen wird. Dieser Pointer wird verwendet um Objekte diesem Typ zuzuordnen. Währen Module den `mp_type_module`
Pointer verwenden. Hier wird die erste Parallel in der Verwaltung von Objekte und Modulen klar. 

Zusätzlich zu diesem Typ-Pointer kann ein selbst Deffiniertes Struct erstellt werden.
In der Form:
```cpp
typedef struct _<struct> {
    mp_obj_base_t base;
    <
    Frei wählbare Felder. 
    >
} <struct>_t;
```
Dieses Struct ist später an Objekt Instanzen gebunden. In diesem Struct können Informationen die man später für die Verwendung eines Objektes braucht verwaltet werden. Ein Python-Object kann als referenz auf diesen Daten-Typ angesehen werden.

Wichtig ist das diese Struct immer über eine base verfügen. (siehe [Micropython-Object-Type](#micropython-object-type)) Alle anderen Felder sind frei definierbar.  

Für Micropython-Objekte wie z.B. Sensoren erstelle ich zusätzlich ein Array, das die zu verwaltenden Objekte einmalig anlegt.
> mehr dazu unter [Reduzierung des Micropython-Heap-Verbrauchs durch Objektreferenzen](#reduzierung-des-micropython-heap-verbrauchs-durch-objektreferenzen).

#### Selbst Deffinierte Klassen

Wie oben beschrieben verwaltet das Struct der Objektes Informationen über eine Instanz. Zum Beispiel den Port mit dem eine Sensor-Instanz initialisiert wurde. 
Vergleichbar ist dies mit dem `self`-Konzept aus der Python-Programmierung.
Hat man also eine Funktion einer Klasse:  
```cpp
static mp_obj_t get_button(mp_obj_t arg) {
<...>
}
```
so ist das erste Argument welches der Funktion mit gegeben wird das `self`. Also ein `mp_obj_t`-pointer der auf das korrekte Struct gecasted werden kann. 

//TODO diesen teil noch zuende schreiben  
Um diese C-Funktion in die Funktions-Tabelle der Micropython-VM eintragen zu lassen, werden Macros bereit gestelle:  
MP_DEFINE_CONST_FUN_OBJ_1/.....  

#### Bereit gestellte Funktionen und Konzepte

Micropython bietet von sich aus auch verscheidene Objekt-Typen an, wie z.B. Listen oder Dictionaries diese sind Konzeptionell das selbe wie eine Objekt-Instanz einer selbst deffinierten klasse. Dafür stellt Micropython vordeffinierte Modul-Klassen und Typ-Strukturen bereit. Und setzen die Micropython-Objekte auf gleiche art und weise um wie selbst deffinierte Klassen.
(Beispiel aus `objlist.h`)
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
Dadruch ist es möglich Micropython-Objekte oder Module auf gleiche art und weise zu erweitern wie man es bei einem Selbst-Deffinierten-Datentyp machen würde. Z.B. könnte man das Listen-Object um eine Get-Index-Funktion Erweitern.

Es gibt auch eine Vielzahl an anderen Konzepten die Micropython-Objekte umsetzen.
Auß dem oben angegeben Code-Schnippsel lassen sich zwei dieser Konzepte heraus lesen.
Der `new` operator, welcher immer den typen des gewünschten Elements hällt. Das is der in [Klassen-Type](#klassen-type) beschriebene Pointer. Und eine Variable Liste an Argumenten angegeben durch `size_t n_args, size_t n_kw, const mp_obj_t *args`.
> Mehr zu solchen Argument-Listen [Argument-Parsing](#argument-parsing)  
//TODO Argument-Parsing text schreiben

Diese besonderen Methoden werden in dem `MP_DEFINE_CONST_OBJ_TYPE`-Macro angegeben und somit an Micropython angebunden.  
(aus `objlist.c`)
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
Das Listen-Obejkt bildet eine Vielzahl an diesen Eingebauten Konzepten an.  
An erster Stelle ist immer die `mp_obj_type_t` Referenz, an dieser Stelle bindet Micropython den Typ-Pointer and die hier deffinierten Funktionen. 
| MP-Konzept          |       Bedeutung     | Verwendung |  
| ------------------- | -------------   |  ------- |
| MP_QSTR_`name`      | Der Objekt Name | Dieser wird z.b. von dem get-type python operator verwendet um den Objekt-Name auszulesen | 
| make_new Object | Der `new` Operator  | Die Funktion die bei der erstellung einer neuen Objekt-Instanz aufgerufen wird. Hier wird das Micropython-Objekt als return-wert wieder gegeben. |
| print      | Die zum Objekt gehörende print Funktion     | Das Binding für die Print Funktion, `print(objekt)`. |
| unary_op | Unary Operation | Dies Sind Operationen auf dem Objekt selber z.B. der boolsche vergleich|
| binary_op | Binäre Operationen | Dies sind Operationen mit anderen Objekten z.B. der `+`- oder `-`-Operator|
| locals_dict | Das Objekt-Dictionary | Im Objekt-Dictionary werden weitere Funktionen des Objektes festgehalten und Deffiniert. Für Klasse-Typen sind diese Instanz gebunden. |
| MP_TYPE_FLAG_ITER_IS_GETITER | Der Objekt Iterrier Typ  | Zuweisung der Iterator Implementation, also welche der verschiedenen Micropython-Iteratier verfahren verwendet werden soll.  |
| subscr | Subscription | Dieses Konzept ist meistens als `Indexing` bekannt z.B. `a[1]` |
| iter | Der Iterator | Dieses Objekt kann z.B. in einer for-each loop verwendet werden. |

## Micropython Typ Zuordnung
Micropython stellt eine möglichkeit bereit um verschieden Micropython-Object-Typen zu unterscheiden und unterscheidlich zu verarbeiten. Im Folgenden möchte ich ein paar Fragen im zusammenhang dazu klären:

1. [Worauf Stützt sich die Typ-Zuordnung](#worauf-stützt-sich-die-typ-zuordnung)
2. [Wie Funktioniert die Typ-Zuordnung (Non-Concrete-Types)](#wie-funktioniert-die-typ-zuordnung-non-concrete-types)
2. [Wie Funktioniert die Typ-Zuordnung (Concrete-Types)](#wie-funktioniert-die-typ-zuordnung-concrete-types)

### Worauf Stützt sich die Typ-Zuordnung
Wie in [Micropython-Object-Type](#micropython-object-type) beschrieben wird der `mp_obj_t` Pointer verwendet um zusätzliche Informationen über das Micropython-Object zu verwalten.  
Die für uns Bereit gestellten Object-Klassifizierungen sehen wie folgt aus.

| Typ                 |       Bit     |
| ------------------- | ------------- |
| Small Int           | xxxx...xxx1   |
| QStrings            | xxxx...x010   |
| Intermediate Object | xxxx...x110   |
| MP Object Base      | xxxx...xx00   |

Intermedia Objects sind, wie in [Intermedia Objects Type]() beschrieben, durch Micropython bereit gestellte Objekte wie Listen und Integer.  
QStrings sind sind, wie in [QStrings-Type]() beschrieben, eine spezielle Repräsentation von Strings.     
MP-Object-Base sind wie in in [Micropython-Object-Type](#micropython-object-type) beschrieben, im wesentlichen entweder Module- oder Klassen-Objecte. Eben solche Objecte welche eine Micropython Object Base besitzen. 
> [!NOTE]
> Diese Objecte könnte ich noch gut genauer erkären, eigene kapitel dazu

Wichtig ist hier anzumerken, wie dieser Prozess der Speicherung von Typinformationen funktioniert.

MicroPython verfügt nicht über einen speziellen Mechanismus, um sicherzustellen, dass MP Object Base Pointer immer mit -xxx...xx00 enden. Stattdessen verlässt sich MicroPython hier auf Pointer-Alignment. Diese Annahme basiert darauf, dass der GCC-Compiler verwendet wird. Da der GCC-Compiler dem C-Standard folgt, geht er immer von ausgerichteten Zeigern aus. Definiert wird dies in dem C-Standart:

(Seite 36 ISO/IEC 9899:202x)  
"6.2.8(.1)  
Complete object types have alignment requirements which place restrictions on the addresses at
which objects of that type may be allocated. An alignment is an implementation-defined integer
value representing the number of bytes between successive addresses at which a given object can be
allocated. An object type imposes an alignment requirement on every object of that type: stricter
alignment can be requested using the _Alignas keyword.
"  

Da MicroPython bis zu 3 Bits verwendet, um Typinformationen zu speichern. (siehe Intermediate Object) Nehmen die Micropython-Entwickler hier an, dass die Kompilierung für das Zielsystem mindestens ein 3-Bit-Alignment aufweist.

Der STM32F405 Prozessor hat nach angaben eines STM32 Mitarbeiters eine 4-Byte Grenze für Pointer Alignment. D.h. Pointer auf ein Struct sind immer ein vielfaches von 2^32. (https://community.st.com/t5/stm32-mcus-products/pointer-optimization/td-p/417229)

Somit ist die von dem Micropython-Entwicklern gemacht annahme für den von uns gewählten Microcontroller zulässig. 

### Wie Funktioniert die Typ-Zuordnung (Non-Concrete-Types)

Die oben angegebenen Daten-Typen sind im Sinne der Typ-Zuordnung ähnlich implementiert, daher hier small int als Beispiel:   
```cpp
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A

static inline bool mp_obj_is_small_int(mp_const_obj_t o) {
    return (((mp_int_t)(o)) & 1) != 0;
}
#define MP_OBJ_SMALL_INT_VALUE(o) (((mp_int_t)(o)) >> 1)
#define MP_OBJ_NEW_SMALL_INT(small_int) ((mp_obj_t)((((mp_uint_t)(small_int)) << 1) | 1))
```

Im Gegensatz zu `mp_obj_base_t`-Pointer wird hier der `mp_obj_t`-Pointer nicht als tatsächlicher Zeiger, sondern als Nutzdaten-Träger für den Wert des Small Int verwendet. Und als Informatins-Träger, für die Typ-Zuordnung. Dies funktioniert, da Small Int weniger Bits benötigt als der void-Pointer und somit Platz für die zusätzliche Information des Datentyps bietet. Hier ein Code-Snippet, welches die Funktionsweise verdeutlicht: 
```cpp
mp_obj_t b = MP_OBJ_NEW_SMALL_INT(0x1); //b soll den wert 1 haben
printf("Ausgabe(%d)", b); //Ausgabe(3) d.h. 0x..0011
                          //also 0x00000..x1 wir haben den typ small int
                          //mit dem Nutzdaten-Wert von 1
```
> [!NOTE]  
> //TODO diese note überarbeiten
> Hier ist etwas wichtiges zu beachten. Arbeitet man mit mp_obj_t und erhält fälschlicherweise einen einfachen Datentyp als Parameter seiner Methoden. So ist es wichtig, Folgendes zu beachten: Das diese besonderen Datentype anders behandelt. Anders als mp_obj_t, welches direkt auf ein Struct referenzieren. Vor dem Casten auf ein tatsächlichhes Micropython-Object, solle man prüfen ob das eingabe object vom richtigen typ ist.  
Glücklicherweise bietet MicroPython eine Infrastruktur für diese Typ-Prüfungen.   
//TODO an dieser stelle später auf die abschnitte verlinken

### Wie Funktioniert die Typ-Zuordnung (Concrete-Types)
//TODO hier noch einen Teil der Typ-Zuordnung für mp_obj_base_t typen also zu ordnung über den typ pointer, am anfang von type zu ordnung noch ergänze warum ich das hier konkret /nicht konkret nenne (kommentar auf obj.h)

## Problematik bei der Verwendung von Namespaces
//TOOD das hier alles noch nach bearbeiten.
### Super- & Submodule

Hier möchte ich kurz erklären, was ich mit Super- und Submodulen meine. Ein Modul kann in diesem weiten Sinne auch synonym mit Unterklassen oder registrierten Funktionen verwendet werden. Möglicherweise wird hier ein anderer Name gebraucht.

### Dictionaries und QStrings

Dictionaries halten die Informationen über QStrings (Namen von Modulen, Funktionen, Typen, etc.).

### Limitierungen des MicroPython-Interpreters

Imports von Modulen funktionieren nur eine Ebene tiefer. Hat man z.B. ein Modul `Devices` und registriert darin das Submodul `Sensors`, welches wiederum einen spezifischen Sensor wie `EV3-Lichtsensor` enthält, so könnte man erwarten, dass die Syntax `import Devices.Sensors.EV3-Lichtsensor` oder `from Devices.Sensors import EV3-Lichtsensor` funktioniert. Jedoch findet der MicroPython-Interpreter diese Module nicht.

Für solche Submodule bietet MicroPython ein alternatives Vorgehen an, wie es auch in dessen Submodule-Beispiel erklärt wird. Diese Methode bringt jedoch andere Probleme mit sich.

### Submodule als QString Alias

Ein alternatives Vorgehen für solche Submodul-Strukturen wäre, das Modul `Devices` gar nicht umzusetzen. Dadurch kann man diesem jedoch keine eigenen Funktionen zuweisen (erste Einschränkung). Zudem ist man beim Importieren der Module in die andere Richtung begrenzt.

Man würde ein Modul `Sensor` schreiben, registriert dieses jedoch nicht bei einem Supermodul, sondern gibt ihm den Namen `MP_QSTR_devices_dot_sensors`. Anschließend muss man eine QString-Datei erstellen (welche in den MicroPython-Build-Prozess eingebunden wird) und darin die Strings ohne Punkt bereitstellen, z.B. `Q(devices.sensors)`. Erst dadurch erhält das Modul den Namen mit ".".

Jetzt kann man jedoch das Modul `devices` selbst nicht importieren, sondern muss stattdessen `from devices.sensors import EV3-Lichtsensor` verwenden.
<!-- TODO: Kann man auch `import devices.sensors.EV3-Lichtsensor` schreiben? -->

### Kombination von Submodulen, QString und Supermodulen

Man könnte auf die Idee kommen, beide Ansätze zu kombinieren. Man erstellt ein Supermodul `devices`, registriert dieses sowie das zugehörige Untermodule. Das Untermodule selbst wird ebenfalls registriert. Dieses Vorgehen scheint auf den ersten Blick zu funktionieren und löst die oben beschriebenen Probleme, hat jedoch auch unerwartetes Verhalten.

Führt man `import devices.sensors` aus, so scheint der Import korrekt zu funktionieren. Bei näherer Betrachtung wird jedoch nicht das Modul `sensors` importiert, sondern es wird das Modul `sensors` unter dem Namen `devices` importiert. Führt man dann `print(devices)` aus, so wird der Name des `sensors` Moduls ausgegeben und überschreibt das `devices` Modul, zumindest bis `import devices` ausgeführt wurde.

### Schlussfolgerung zu diesen Problemen

Ich hatte ursprünglich geplant, den Namespace `orb` zu verwenden, um Module zu organisieren, die Klassen und Funktionen zusammenfassen, wie z.B. `orb.sensors`, `orb.devices`, usw. Aufgrund der oben genannten Probleme ist dies jedoch nicht möglich. Der `orb`-Prefix muss gestrichen werden, um das von Python erwartete Verhalten zu gewährleisten.

Es gibt daher die Einschränkung, Module nur eine Ebene tief zu gestalten. Funktionen und Klassen auf einer dritten Ebene registriere ich nur, wenn sie ausschließlich im Zusammenhang mit dem Supermodul Sinn machen, z.B. Funktionen einer Klasse, die zuvor instanziiert werden muss.

## Micropython Flags
//TODO diesen Teil vielleicht auch in ImReport nur für meine Flags erstellen, dann hierhin verweisen und erkläten wie flags im Rahmen von Micropython verwendet werden. (stichwort "Konifigurierbarkeit/Modularität")  

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


## Reduzierung des Micropython-Heap-Verbrauchs durch Objektreferenzen

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

## Thread Safety
Micropython ist nicht Thread-Safe. Dies ist bei der Implementierung der Micropython-Task zu beachten. Micropython selber sollte so lange nicht anders möglich seinen Speicher-Bereich vollständig selber verwalten. Die Micropython-Task sollte vollständig losgelöst von der Restlichen Firmware laufen. Kommunikation zwischen ORB-Firmware und Python-VM sollte nur über Flags geschehen die immer nur von einer der beiden Seiten beschrieben werden können. Dadurch enstehende Race-Conditions sollten bedacht werden. 
>Micropython selber bietet die möglichkeit an Threads (in der VM geschlossen) zu verwalten. Dafür muss man die in der Datei `mpthread.h` deffinierten Funktionen umsetzen. Da die ORB-Application ab von sich aus keine Threads anbietet und das Umsetzen dieser somit außerhalb des Umfangs meiner Bachelorarbeiet liegt wird dies nicht weiter beachtet.

## Windows-Bug: Falsches Register bei Non-Local Return-Adressierung 

### Definition und Funktionsweise des Non-Local Return
Der Non-Local Return (NLR) wird in der MicroPython-Umgebung verwendet, um bei Ausnahmen oder Fehlern schnell aus tiefer verschachtelten Funktionsaufrufen herauszuspringen. Dabei ermöglicht NLR es, den normalen Programmablauf zu unterbrechen und direkt zu einem vorher definierten Punkt im Code zurückzukehren. Ohne erst den vollständigen Funktions aufruf nach oben hin wieder abzuwickeln. Z.B. bei dem Auftreten von Exceptions, hier muss der Programm ablauf Effektiv und vorallem Speicher und Rechen-Leistungs Effizient unterbrochen werden können.

### Beschreibung des Bugs
Führt man den Micropython-Embed-Port unter Windows aus, ohne die Optimierungs-Flag `-fomit-frame-pointer` Kommt es zu einen Speicher-Zugriffs fehler nach der ausführung der Funktion `nlr_push` zu finden in der Datei `py/nlrx64.c`. (diese ist in `-Os` & `-Og` enthalten wie in [GNU Optimizations Docs](https://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html) beschrieben.  
"-fomit-frame-pointer  
<...>    
Enabled by default at -O1 and higher."  
>[!NOTE]  
>Da `-Os` alle Optimierungen von `-O1` durchführt. Und `-Os` alle von `-O2`.(Beiden Optimierungen haben Ausnahmen, zu denen -fomit-frame-pointer jedoch in beiden Fällen nicht zählt).

Der Grund für diesen fehler ist die annahme das Micropython an diser stelle die `omit-frame-pointer`-Anweisung vorraussetzt.  
(Aus nlrx64.c Zeile 38)
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
Ungünstig in an dieser Stelle nur das der Fall für Windows hier garnicht beachtet wird. Das Windows diese Optimisierungs Anweisung jedoch erwartet auch wenn nicht hier angeführt, kann man aus der Funktion jedoch heraus lesen.

Es handelt sich hier um ein Problem das im Rahmen der nlr-ansembler codes auftritt, daher ein wenig Kontext zu diesem Problem. Bei diesem Problem spielen Zwei Register eine Rolle, rbp und rbp.  

Wenn der Compiler rbp als Frame Pointer verwendet,  
> [!NOTE]  
> Dies könnte man als non-omitted bezeichnen
  
sorgt er dafür, dass rbp auf den Anfang des Stack-Rahmens zeigt. Ein Stack-Rahmen speichert die lokalen Variablen, Funktionsargumente und die Rücksprungadresse einer Funktion. Ist rbp als Frame Poiunter verwendet so kann man diese Register nicht mehr frei verwendet. (kein general-purpose register)  
//TODO mit quellen belegen  
Der Quell-Code der nlrx64.c Funktion sieht aber wie Folgt aus:  
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
Hier ist an Folgender Zeile zu erkennen was Falsch läuft.   
`movq   (%rsp), %rax        \n" // load return %rip`  
Wie auch im Kommentar steht wird hier die Addresse für den return Sprung geschrieben, diese jedoch nicht nach rbp sondern nach rsp.  

Das rsp-Register ist das Stack Pointer Register. Wird rbp omitted so wird das rsp register direkt verwendet um den Stack-Top zu verwalten. Also wo sich die Rücksprung/Return Addresse befindet.  
  
Das heißt hier ist es Notwendig das rsp und nicht rbp von dem Compiler als "Rücksprung-Zuständiger" erkannt wird. Falls nicht ist das Verhalten undefiniert und wir bekommen im besten Fall einen Speicher-Zugriffs-Fehler, können somit erkennen das hier etwas schief gelaufen ist.

### Vorgehensweise zur Fehlerbehebung

Da dieses Problem nur unter Windows eine Rolle-Spielt und für den STM32F405 Microcontroller dies Problem nicht auftritt. Reicht es für mich immer mit Compiler-Flags zu Compilieren welche `-fomit-frame-pointer` setzen. Also `-Os` oder auch `-Og`. Das ORB verwendet in seinem Compilier-Prozess `-O0`, jedoch gibt es diesen Bug nicht bei der Verwendung mit diesem Microcontroller. D.h. __attribute__((optimize("omit-frame-pointer"))) wird korrekt gesetzt bzw. erreicht. 

## Compiler Flag Kompatibilität
> [!Caution]
>  Compiler-Flags sind ein Stolperstein für alle die einen eigenen Micropython-Port aufbauen wollen oder wie ich, in ein bereit bestehendes Projekt Integrieren wollen. Ist man bei seinem Ausgangsprojekt (in meinem Fall ORB) an Bestimmte Compiler Flags gebunden, so kann es grundsätzlich unmöglich oder sehr umständlich sein Micropython in sein Projekt zu integrieren. Die betrachtung der Compatibelen Compiler-Flags sollte also vor der Implementierung des Ports getestet und gut durch dacht werden.

### Micropython Compiler Flags
Möchte man Micropython Compilieren und auch Debuggen können, so ist es notwendig  `-Og` als Flags zu setzen.
Dieses Flag sorgt für eine moderate Optimierung. Für uns ist an dieser Stelle aber wichtig das Debugg Informationen erhalten bleiben. Durch die verwendung dieser flag ist es möglich in der Code::Blocks Umgebung Micropython Code sinnvoll zu debuggung. 
    
> Die verwendung dieser Flag erfüllt zugelich für uns einen zweiten zweck. Der Dokumentation der [gcc compiler flags](https://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html) ist zu entnehmen, das viele optimierungs flags durch das verwenden von -Og unterdrückt werden.  
Wie man im Kapitep 3.11 Options That Control Optimization nachlesen kann: "Most optimizations are completely disabled at -O0 or if an -O level is not set on the command line, even if individual optimization flags are specified. Similarly, -Og suppresses many optimization passes."   
Auch wenn es noch viele weitere gibt hier exemplarisch die -ftree-pta flag:   
>```cpp 
>-ftree-pta
>    Perform function-local points-to analysis on trees. 
>    This flag is enabled by default at -O1 and higher, except for -Og.
>```
Dieser Ansatz minimiert also das Risiko von Problemen durch aggressivere
Optimierungen. Das dies auch wirklich so ist lässt sich einfach ausprobieren. 
Test weise habe die `-Og` Flag entfernt und die `-O3` optimierung verwendet. Dies ist eine sehr aggresive Optimierung. Baue ich nur meinen Micropython-Port der mit der Richtigen Compiler Flag problemlos funktioniert so bekomme ich schon vor dem ausführen der ersten Python-Zeile den Fehler-Code `0xC0000005`. dies ist ein Speicher zugriffs Fehler. 
Aufgrund der Natur von MicroPython, das stark auf das Casten von Zeigern und das dynamische Aufrufen von Funktionen basiert. Und das durch diese Operationen das dynamische aufrufen von Python-Funktionen realisiert. 
Ist es entscheidend, dass Optimierungen die beispielsweise ungenutzte MicroPython-Module entfernt nicht genutzt werden. Daher sollte im besten Fall auf aggressive Optimierungen verzichtet werden, um die Funktionsfähigkeit und Stabilität des Systems zu gewährleisten.

Ist das Deguggen nicht mehr notwendig, so ist es möglich `-Os` zu verwenden um Speicherplatz zu sparen. Abgesehen von der geringen Programm-Größe bietet dieses Flag jedoch keinen vorstellt. Wichtig anzumerken ist das dies die einzige optimierungs Flag ist welche ist als sicher im Rahmen der verwendung mit Micropython ansehe. Da dies die einzige Flag ist welche man in der vielzahl der Micropython-Ports findet.

### ORB-Firmware Compiler Flags
Die ORB-Firmware hat als vor Configurierte Optimizierunge `-O0` eingestellt. Zusätzlich sind die Flags: 
`-ffunction-sections` und `-fdata-sections` eingestellt. Diese Optionen Isolieren Daten und Funktionen jeweil in ein eigenes Segment. Aus der [GNU GCC Dokumentation](https://gcc.gnu.org/onlinedocs/gnat_ugn/Compilation-options.html) ist zu entnehmen das diese Flags nur den generierten Code sortieren. Jedoch nicht ungenutzten Code entfernen. Für das entfernen hier "garbage collection" von ungenutzten code müsste man die Flag ` -Wl,--gc-sections` setzen. Daher würde ich diese Flags als eher unproblematisch einstufen. Zusätzliches Testen konnte vallidieren das diese Compiler-Flags mit dem Micropython-Projekt compatibel sind.  
Außerdem ist in dem ORB-Firmware-Projekt configuriert, das Floating-Pointer operationen als **"FPU-specific calling convention"** generitert werden. Für uns bedeutet dies das wir Float-Operationen als Daten-Typ verwenden können und uns keine gedanken über die genaue verwendung von Floats auf der FPU des Microcontrollers machen müssen. Alle anderen Flags sind für Compiler-Warnings und zusätzliche Debug-Informationen.

### Angepassten Compiler Flags
Da die Compiler Flags der ORB-Firmware ohne hin schon kompatiblem mit den erlaubten Compiler-Flags des Micropython-Ports sind müssen keine weiteren änderungen vorgenommen werden. Das Fertige Projekt soll mit `-O0` und den zusätzlichen Flags der ORB-Firmware compiliert werden.  

Wird ein höherer grad an Optimierung von nöten sein, z.B. durch unzureichenden Speicherplatz. So sollte im besten fall die Optimierungs-Stufe `-Os` verwendet werden und die Kompatibilität von dieser im zusammenhang mit der ORB-Firmware getestet werden. Zu diesem Zeitpunkt ist jedoch keine zusätzliche Optimierung notwendig. Dies könnte jedoch bei zusätzlicher erweiterung der ORB-Firmware durch z.B. zusätzliche Funktionen in der Zukunft erforderlich sein.  

## Argument Parsing
