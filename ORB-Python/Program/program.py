from devices import sensor, motor, button, servo, EV3Ultrasonic
import monitor
from monitor import keys
import time

def testMotor():
    a = motor(port = motor.M1, direction =  motor.REVERSE)
    a.config(direction = motor.REVERSE)
    a.set(mode = motor.POWER_MODE, speed = 245)
    print(a.get())

def testSensor():
    b = sensor(sensor.S1)
    b.getValueExt(0)
    b.getDigital()
    b.config(mode=sensor.TOF)
    print(b.get())

def testButton():
    c = button(id = 1)
    d = button(id = 0)
    print(c.get())
    print(d.get())

def testMonitor():
    if(keys.A1 == monitor.getKey()):
        print(keys.A2)
    else:
        print(keys.A3)
    monitor.setText("hallo")
    monitor.setText(" wie gehts\n")
    print(monitor.getKey())

def testServo():
    f = servo(servo.S1)
    f.set(10,20)
    print(f)
    

def testTime():
    print("Time: " + str(time.getTime()))
    time.wait(10)
    print("")

def sens():
    print(EV3Ultrasonic)
    e = EV3Ultrasonic(3)
    print(e)
    print(e.get())

testMotor()
testSensor()
testButton()
testMonitor()
testTime()
testServo()
#sens()