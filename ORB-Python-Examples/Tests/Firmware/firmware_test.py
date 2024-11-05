from devices import sensor, motor, servo
import time
import monitor
from monitor import keys
import time
import memory
import math

class KeyA8Exception(Exception):
    pass

def waitAndGetKey():
    key = keys.NO_KEY

    while monitor.getKey() != keys.NO_KEY:
        pass
    
    while (key := monitor.getKey()) == keys.NO_KEY:
        pass

    #wait for key release
    while monitor.getKey() != keys.NO_KEY:
        pass

    if key == keys.A8:
        raise KeyA8Exception()

    return key

def clearMonitor():
    monitor.setText(0, " ")
    monitor.setText(1, " ")
    monitor.setText(2, " ")
    monitor.setText(3, " ")

def printPMOption():
    clearMonitor()
    monitor.setText(0, "change: +/-")
    monitor.setText(1, "confirm:#")

class VM_State_Test:
    def printVMStateMenu():
        monitor.setText(0, "1. Hard-Fault")
        monitor.setText(1, "2. Memory-Error")
        monitor.setText(2, "3. User-Exception")
        monitor.setText(3, "4. User-Interrupt")

    def executeTest(key):
        clearMonitor()

        #finally figured out how to force a hardfault (access violation inside micropython interpreter)
        if key == keys.B1:
            a = [1,2,3,4,5,6,7,8,9,10]
            while True:
                a.append(a)
                print(a)

        if key == keys.B2:
            a = [1]
            while True:
                a.append([1,2,3,4,5,6])
                monitor.setText(0, a)

        if key == keys.B3:
            raise Exception("This is a User-Exception raised by the Python-Script.")

        if key == keys.B4:
            monitor.setText(0,"Press Red Stop Button")
            while True:
                pass

    def select():
        while True:
            VM_State_Test.printVMStateMenu()
            VM_State_Test.executeTest(waitAndGetKey())

class Time_Test:
    def printMenu():
        clearMonitor()
        monitor.setText(0,"1. Get Time")
        monitor.setText(1,"2. Wait")

    def selectTest(key):
        if key == keys.B1:
            monitor.setText(2, "Time: " + str(time.getTime()))
            time.wait(2000)

        if key == keys.B2:
            monitor.setText(2, "Waiting 2 seconds...")
            time.wait(2000)
            monitor.setText(2, "Finished")
            time.wait(1000)

    def select():
        while True:
            Time_Test.printMenu()
            Time_Test.selectTest(waitAndGetKey())

class Memory_Test:
    def printMenu():
        monitor.setText(0, "1. Get Memory")
        monitor.setText(1, "2. Clear Memory")
        monitor.setText(2, "'Write Mem'-Programm")
        monitor.setText(3, "Is used to Write Memory")

    def selectTest(key):
        if key == keys.B1:
            clearMonitor()
            value = memory.getMemory(0, 1)
            monitor.setText(0,"Reading One Byte at Address 0")
            monitor.setText(1,"Bytes-Value: " + str(value))
            monitor.setText(2,"As List: " + str(list(value)))
            time.wait(3000)

        if key == keys.B2:
            clearMonitor()
            memory.clearMemory()
            monitor.setText(0, "Cleared Memory(to xff)")
            time.wait(2000)
   
    def select():
        while True:
            Memory_Test.printMenu()
            Memory_Test.selectTest(waitAndGetKey())

class Motor_Test:
    port = 0
    direction = 1
    acc = 0
    speed = 0
    mode = 0
    position = 0

    def printMenu():
        clearMonitor()
        monitor.setText(0, "1. config")
        monitor.setText(1, "2. set")
        monitor.setText(2, "3. get")

    def selectTest(key):
        if key == keys.B1:  
            clearMonitor()
            monitor.setText(0, "Confirm Settings with '#'")
            monitor.setText(1, "press any key......")
            first = True
            while (key := waitAndGetKey()) != keys.B10:
                if first:
                    key = keys.NO_KEY
                    first = False
                if key == keys.B1:
                    Motor_Test.port = (Motor_Test.port + 1)%4
                if key == keys.B2:
                    Motor_Test.direction *= -1
                if key == keys.B3:
                    Motor_Test.acc = (Motor_Test.acc + 10)
                    if Motor_Test.acc > 100:
                        Motor_Test.acc = -100

                if Motor_Test.direction == 1:
                    d_str = "FORWARD"
                else:
                    d_str = "BACKWARD"
                monitor.setText(0, "1. Port (M" + str(Motor_Test.port + 1) + ")")
                monitor.setText(1, "2. Direction (" + d_str +")")
                monitor.setText(2, "3. Acc (" + str(Motor_Test.acc)  + ")")

        if key == keys.B2:
            clearMonitor()
            monitor.setText(0, "Go back with '#'")
            monitor.setText(1, "press any key......")
            first = True
            while (key := waitAndGetKey()) != keys.B10:
                if first:
                    key = keys.NO_KEY
                    first = False
                if key == keys.B1:
                    Motor_Test.mode = (Motor_Test.mode + 1) % 4
                if key == keys.B2:
                    Motor_Test.speed = (Motor_Test.speed + 100)
                    if Motor_Test.speed > 1000:
                        Motor_Test.speed = 0
                if key == keys.B3:
                    Motor_Test.position = (Motor_Test.position + 36)
                    if Motor_Test.position > 360:
                        Motor_Test.position = 0

                if Motor_Test.mode == 0:
                    mode_str = "POWER_MODE"
                if Motor_Test.mode == 1:
                    mode_str = "BRAKE_MODE"
                if Motor_Test.mode == 2:
                    mode_str = "SPEED_MODE"
                if Motor_Test.mode == 3:
                    mode_str = "MOVETO_MODE"

                monitor.setText(0, "1. Mode(" + mode_str + ")")
                monitor.setText(1, "2. Speed(" + str(Motor_Test.speed) + ")")
                monitor.setText(2, "3. position(" + str(Motor_Test.position) + ")")
                monitor.setText(3, "")
                m = motor(port = Motor_Test.port, direction = Motor_Test.direction, acc = Motor_Test.acc)
                m.set(speed = Motor_Test.speed, position = Motor_Test.position, mode = Motor_Test.mode)

        if key == keys.B3:
            clearMonitor()
            m = motor(Motor_Test.port)
            print(m.get())
            time.wait(2000)

    def select():
        while True:
            Motor_Test.printMenu()
            Motor_Test.selectTest(waitAndGetKey())

class Sensors_Test:
    port = 0
    type = 0
    mode = 0
    option = 0

    def printMenu():
        clearMonitor()
        monitor.setText(0, "1. config")
        monitor.setText(1, "2. get")
        monitor.setText(2, "3. pre-configured")

    def selectTest(key):
        if key == keys.B1:
            clearMonitor()
            monitor.setText(0, "Go back with '#'")
            monitor.setText(1, "press any key......")
            first = True
            while (key := waitAndGetKey()) != keys.B10:
                if first:
                    key = keys.NO_KEY
                    first = False
                if key == keys.B1:
                    Sensors_Test.port = (Sensors_Test.port + 1) % 4
                if key == keys.B2:
                    Sensors_Test.type = (Sensors_Test.type + 1) % 5
                if key == keys.B3:
                    Sensors_Test.option = 0
                    monitor.setText(2, "Type in value|Confirm:'*'")
                    time.wait(2000)
                    monitor.setText(2, "3. Option_Value: "+str(Sensors_Test.option))

                    while (key := waitAndGetKey()) != keys.B12: 
                        if key == keys.B1:
                            Sensors_Test.option = (Sensors_Test.option*10) + 1
                        if key == keys.B2:
                            Sensors_Test.option = (Sensors_Test.option*10) + 2
                        if key == keys.B3:
                            Sensors_Test.option = (Sensors_Test.option*10) + 3
                        if key == keys.B4:
                            Sensors_Test.option = (Sensors_Test.option*10) + 4
                        if key == keys.B5:
                            Sensors_Test.option = (Sensors_Test.option*10) + 5
                        if key == keys.B6:
                            Sensors_Test.option = (Sensors_Test.option*10) + 6
                        if key == keys.B7:
                            Sensors_Test.option = (Sensors_Test.option*10) + 7
                        if key == keys.B8:
                            Sensors_Test.option = (Sensors_Test.option*10) + 8
                        if key == keys.B9:
                            Sensors_Test.option = (Sensors_Test.option*10) + 9
                        if key == keys.B11:
                            Sensors_Test.option = (Sensors_Test.option*10)

                        monitor.setText(2, "3. Option_Value: "+str(Sensors_Test.option))

                if key == keys.B4:
                    Sensors_Test.mode = 0
                    monitor.setText(3, "Type in value|Confirm:'*'")
                    time.wait(2000)
                    monitor.setText(3, "4. Mode_Value: "+str(Sensors_Test.mode))

                    while (key := waitAndGetKey()) != keys.B12: 
                        if key == keys.B1:
                            Sensors_Test.mode = (Sensors_Test.mode*10) + 1
                        if key == keys.B2:
                            Sensors_Test.mode = (Sensors_Test.mode*10) + 2
                        if key == keys.B3:
                            Sensors_Test.mode = (Sensors_Test.mode*10) + 3
                        if key == keys.B4:
                            Sensors_Test.mode = (Sensors_Test.mode*10) + 4
                        if key == keys.B5:
                            Sensors_Test.mode = (Sensors_Test.mode*10) + 5
                        if key == keys.B6:
                            Sensors_Test.mode = (Sensors_Test.mode*10) + 6
                        if key == keys.B7:
                            Sensors_Test.mode = (Sensors_Test.mode*10) + 7
                        if key == keys.B8:
                            Sensors_Test.mode = (Sensors_Test.mode*10) + 8
                        if key == keys.B9:
                            Sensors_Test.mode = (Sensors_Test.mode*10) + 9
                        if key == keys.B11:
                            Sensors_Test.mode = (Sensors_Test.mode*10)

                        monitor.setText(3, "4. Mode_Value: "+str(Sensors_Test.mode))

                if Sensors_Test.type == 0:
                    type_str = "Analog"
                if Sensors_Test.type == 1:
                    type_str = "I2C"
                if Sensors_Test.type == 2:
                    type_str = "TOF"
                if Sensors_Test.type == 3:
                    type_str = "Touch"
                if Sensors_Test.type == 4:
                    type_str = "UART"

                clearMonitor()
                monitor.setText(0, "1. Port(S" + str(Sensors_Test.port + 1) + ")")
                monitor.setText(1, "2. Type(" + type_str + ")")
                monitor.setText(2, "3. Option("+ str(Sensors_Test.option)  +")")
                monitor.setText(3, "4. Mode("+ str(Sensors_Test.mode)  +")")

        if key == keys.B2:
            clearMonitor()
            s = sensor(port = Sensors_Test.port, type = Sensors_Test.type, mode = Sensors_Test.mode, option = Sensors_Test.option)

            monitor.setText(0, "get...")
            time.wait(1000)
            clearMonitor()
            val = s.get()
            monitor.setText(0, "Val:"+str(val["values"]))
            monitor.setText(1, "Type:"+str(val["type"]))
            monitor.setText(2, "option:"+str(val["option"]))
            monitor.setText(3, "lenExp:"+str(val["lenExp"]))
            time.wait(4000)

            clearMonitor()
            monitor.setText(0, "getDigital...")
            monitor.setText(1, "Value: " + str(s.getDigital()))
            time.wait(2000)

            clearMonitor()
            monitor.setText(0, "getValueExt...")
            monitor.setText(1,"Ch1:" + str(s.getValueExt(0)))
            monitor.setText(2,"Ch2:" + str(s.getValueExt(1)))
            time.wait(4000)

        if key == keys.B3:
            clearMonitor()
            monitor.setText(0, "Go back with '#'")
            monitor.setText(1, "press any key......")
            first = True
            while (key := waitAndGetKey()) != keys.B10:
                if first:
                    key = keys.NO_KEY
                    first = False
                monitor.setText(0, "Sensor as Port(S1)")
                monitor.setText(1, "1. NXT-Light-Sensor")
                if key == keys.B1:
                    clearMonitor()
                    s = sensor(port = 0, type = sensor.Analog, mode = 0, option = (0x08|0x07)<<8)
                    time.wait(2000)

                    monitor.setText(0,"Light-On...")
                    s.config(option = (0x08|0x07)<<8)
                    time.wait(3000)

                    monitor.setText(0,"Light-Off....")
                    s.config(option = (0x00|0x07)<<8)
                    time.wait(3000)

                    monitor.setText(0,"Light-On...")
                    s.config(option = (0x08|0x07)<<8)
                    time.wait(3000)

                    was = time.getTime()
                    ist = was
                    while (ist - was) < 8000:
                        ist = time.getTime()
                        monitor.setText(0,"Raw-Value:" + str(s.get()["values"][0] & 0xFFF))
                        monitor.setText(3,"Exit in: " + str(8 - int((ist-was)/1000.0)) + "Seconds")
                        
                    break

    def select():
        while True:
            Sensors_Test.printMenu()
            Sensors_Test.selectTest(waitAndGetKey())

class Servo_Test:
    port = 0
    speed = 0
    angle = 0

    def printMenu():
        clearMonitor()
        monitor.setText(0, "1. Port(S" + str(Servo_Test.port + 1)  + ")")
        monitor.setText(1, "2. speed(" + str(Servo_Test.speed) + ")")
        monitor.setText(2, "3. angle(" + str(Servo_Test.angle) + ")")
        monitor.setText(3, "4. apply")

    def selectTest(key):
        if key == keys.B1:
            printPMOption()
            monitor.setText(2, "port(S" + str(Servo_Test.port + 1)  + ")")
            while (key := waitAndGetKey()) != keys.B10:
                if key == keys.A1:
                    Servo_Test.port = min(Servo_Test.port + 1 , 1)
                if key == keys.A2:
                    Servo_Test.port = max(Servo_Test.port - 1, 0)
                monitor.setText(2, "port(S" + str(Servo_Test.port + 1)  + ")")

        if key == keys.B2:
            printPMOption()
            monitor.setText(2, "Speed(" + str(Servo_Test.speed)  + ")")
            while (key := waitAndGetKey()) != keys.B10:
                if key == keys.A1:
                    Servo_Test.speed += 50
                if key == keys.A2:
                    Servo_Test.speed -= 50
                monitor.setText(2, "Speed(" + str(Servo_Test.speed)  + ")")
            
        if key == keys.B3:
            printPMOption()
            monitor.setText(2, "Angle(" + str(Servo_Test.angle)  + ")")
            while (key := waitAndGetKey()) != keys.B10:
                if key == keys.A1:
                    Servo_Test.angle += 10
                if key == keys.A2:
                    Servo_Test.angle -= 10
                monitor.setText(2, "Angle(" + str(Servo_Test.angle)  + ")")

        if key == keys.B4:
            clearMonitor()
            monitor.setText(0, "applying....")
            s = servo(Servo_Test.port)
            s.set(speed = Servo_Test.speed, angle = Servo_Test.angle)
            time.wait(2000)

    def select():
        while True:
            Servo_Test.printMenu()
            Servo_Test.selectTest(waitAndGetKey())

class Devices_Teset:
    def printMenu():
        clearMonitor()
        monitor.setText(0, "1. Servo Module")
        monitor.setText(1, "2. Motor Module")
        monitor.setText(2, "3. Sensor Module")

    def selectModule(key):
        if key == keys.B1:
            Servo_Test.select()
            return
        if key == keys.B2:
            Motor_Test.select()
            return
        if key == keys.B3:
            Sensors_Test.select()
            return
        
    def select():
        while True:
            Devices_Teset.printMenu()
            Devices_Teset.selectModule(waitAndGetKey())

        
class Modules_Test_Top_Level:
    def printMenu():
        clearMonitor()
        monitor.setText(0, "1. Devices")
        monitor.setText(1, "2. Memory")
        monitor.setText(2, "3. Time")

    def selectTest(key):
        if key == keys.B1:
            Devices_Teset.select()
        if key == keys.B2:
            Memory_Test.select()
        if key == keys.B3:
            Time_Test.select()

    def select():
        while True:
            Modules_Test_Top_Level.printMenu()
            Modules_Test_Top_Level.selectTest(waitAndGetKey())

class Build_In_Test:
    def printMenu():
        clearMonitor()
        monitor.setText(0, str(math) + " available")
        monitor.setText(1, "1. Exit")
        monitor.setText(2, "2. Min/Max")
        monitor.setText(3, "3. getArg")

    def selectTest(key):
        if key == keys.B1:
            clearMonitor()
            monitor.setText(0, "Exit in 2 Seconds...")
            time.wait(2000)
            exit()
            while True:
                pass
        if key == keys.B2:
            clearMonitor()
            monitor.setText(0, "Press Keys for Values")
            a =  waitAndGetKey()
            monitor.setText(1, "Value1: " + str(a))
            b = waitAndGetKey()
            monitor.setText(2, "Value2: " + str(b))
            monitor.setText(3, "Min " + str(min(a,b)) + " Max "+ str(max(a,b)))
            time.wait(2000)
        if key == keys.B3:
            clearMonitor()
            monitor.setText(0, "Arg: " + str(getArg()))
            time.wait(2000)

    def select():
        while True:
            Build_In_Test.printMenu()
            Build_In_Test.selectTest(waitAndGetKey())

class Top_Level_Tests:

    def printMenu():
        monitor.setText(0, "Please Select a Test:")
        monitor.setText(1, "1. Modules")
        monitor.setText(2, "2. Builtin Functions")
        monitor.setText(3, "3. Error-Test")

    def callSubTest(key):
        if key == keys.B1:
            Modules_Test_Top_Level.select()
            return
        if key == keys.B2:
            Build_In_Test.select()
            return
        if key == keys.B3:
            VM_State_Test.select()
            return
        
    def select():
        while True:
            Top_Level_Tests.printMenu()
            Top_Level_Tests.callSubTest(waitAndGetKey())

if getArg() == 0:
    while True:
        try:
            Top_Level_Tests.select()
        except KeyA8Exception:
            pass
else:
    clearMonitor()
    monitor.setText(0,"Add values with number Buttons")
    monitor.setText(1, "Confirm with '#'")
    i = 0
    while True:
        key = waitAndGetKey()
        if key == keys.B10:
            memory.setMemory(0, [i])
            exit()
        i += key
        if i > 255:
                i = 255
        monitor.setText(2, "i: " + str(i))
