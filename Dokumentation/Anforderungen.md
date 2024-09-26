# Anforderungen 

1. Der Micropython Embed Port kann unter Windows kompiliert und debuggt werden.
2. Die Ausführung von Micropython-Skripten soll ausschließlich durch MPY-Bytecode ermöglicht werden, nicht durch Klartext oder eine REPL-Funktion.
3. Es sollen Micropython-Module und -Funktionen bereitgestellt werden, die sich aus den in `orblocal.h` definierten Funktionen ableiten.  
    Diese Funktionen werden von der ORB-Firmware für die Programmierung in C zur Verfügung gestellt:
```cpp
...
virtual void        configMotor      (BYTE id,WORD t,BYTE a,BYTE Kp,BYTE Ki)
virtual void        setMotor         (BYTE id,BYTE m, short s, int p       )
virtual ORB::Motor  getMotor         (BYTE id                              )
virtual void        setModelServo    (BYTE id, BYTE s, BYTE w              ) 
virtual DWORD       getTime          (                                     )
virtual void        wait             (DWORD time                           )
virtual void        setMonitorText   (BYTE line,const char *fmt, va_list va)
virtual BYTE        getMonitorKey    (                                     )
virtual void        clearMemory      (                                     )
virtual void        setMemory        (DWORD addr, BYTE *data, DWORD s      )
virtual void        getMemory        (DWORD addr, BYTE *data, DWORD s      )
virtual void        configSensor     (BYTE id,BYTE t,BYTE m,WORD o         )
virtual ORB::Sensor getSensor        (BYTE id                              ) 
virtual WORD        getSensorValueExt(BYTE id, BYTE ch                     ) 
virtual BYTE        getSensorDigital (BYTE id                              )
...
```
4. Die ORB-Funktionen müssen in ihrer vollständigen Funktionalität implementiert werden.  
Die Verwendung von Micropython darf keinen Funktionalitätsverlust zur Folge haben.  
Die Art und Weise wie mit Komponenten Interargiert wird z.B. durch das abbilden als Objecte ist erlaubt.  
5. Diese Funktionen sollen nicht direkt übernommen, sondern in die Python-Welt übertragen werden. Falls angebracht werden Python-Objekte/Module bereitgestellt , welche die genannten Funktionen möglichst gut und mit geeigneten Name-Spaces abbilden.
6. Den Nutzern sollen ausreichende Informationen zur Verfügung gestellt werden, damit sie diese Funktionen verwenden können. Das heißt, auch ohne Vorkenntnisse über die ORB-Firmware soll es möglich sein, den Micropython-Interpreter zu nutzen.
7. Es soll eine klar definierte Schnittstelle zur Python-VM existieren, die als einzige Verbindung zwischen Micropython und der ORB-Firmware dient.
8. Es soll eine Python-Task implementiert werden, die parallel zu den anderen Aufgaben der ORB-Firmware läuft. Diese Python-Task nutzt die Schnittstelle zur Python-VM.
9. Das Python-Programm soll im Flash-Speicher des ORB abgelegt werden.
10. Das Python-Programm soll vom Nutzer gestartet und gestoppt werden können.
11. Die Übertragung des Python-Programms soll über den von DFU bereitgestellten Prozess erfolgen. Die Firmware soll nicht erneut aufgespielt werden müssen.
12. Es sollen keine standardmäßigen Micropython-Module ge-ported werden. Es werden ausschließlich die Funktionen der ORB-Firmware bereitgestellt.
13. Die Funktionen der ORB-Firmware sollen lediglich erweitert und nicht grundlegend verändert werden.   
Die Python-Task muss im Einklang mit den restlichen Aufgaben der ORB-Firmware ausgeführt werden können.   
Anpassungen wie z.B. die Neubelegung von Tasten oder das Verschieben/Verringen von Nutzer-Speicherblöcken sind erlaubt.  
Solange diese die Funktionalität der ORB-Firmware nicht einschränken.
14. Auch wenn die ORB-Funktionen vollständig umgestzt werden sollen, so reicht für z.B. die Sensor-Klasse also solche Funktionen welche meherere Komponenten geleichzeitig umsetzen, das an dieser Stelle nur mit einem Sensor-Typ getestet wird. Es soll lediglich sicher gestellt werden das alle in Micropython eingestellten Parameter korrekt an die ORB-Firmware übergeben werden und einstellbar sind. Ist dies gegeben gillt eine Funktion als getestet. 