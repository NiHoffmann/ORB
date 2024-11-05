import gc
print("bytes: "+ str(gc.mem_alloc()))

from devices import sensor, motor, button, servo
import monitor
from monitor import keys
import time
import memory

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
    monitor.setText(0, "hallo")
    monitor.setText(1, " wie gehts\n")
    print(monitor.getKey())

def testServo():
    f = servo(servo.S1)
    f.set(10,20)
    print(f)
    

def testTime():
    print("Time: " + str(time.getTime()))
    time.wait(10)
    print("")

def testMemory():
    memory.clearMemory()
    memory.setMemory(0, [0,1,2,3])
    print(memory.getMemory(0, 10))

def testExit():
    try:
        while True:
            try:
                #this way we can check if nested exceptions work
                raise Exception("hi")
            except:
                exit()
                #here we wait for the interrupt
            pass    
    except:
        print("looks like nested exceptions didnt really work")

def testInterrupt():
    try:
        while True:
            pass    
    except:
        print("looks like the user interrupt was caught this should not happen")

def test():
    print("bytes: "+ str(gc.mem_alloc()))
    test()

test()
#a = motor(port = motor.M1, direction =  motor.REVERSE)
#a.config(direction = motor.REVERSE)
#a.set(mode = motor.POWER_MODE, speed = 245)
print("hi\n")
monitor.setText(2, "hallo wie gehts ?")
print("test\n\nhi")
time.wait(2500)
testMotor()
print("bytes: "+ str(gc.mem_alloc()))
testSensor()
print("bytes: "+ str(gc.mem_alloc()))
testButton()
print("bytes: "+ str(gc.mem_alloc()))
testMonitor()
print("bytes: "+ str(gc.mem_alloc()))
testTime()
print("bytes: "+ str(gc.mem_alloc()))
testServo()
print("bytes: "+ str(gc.mem_alloc()))
testMemory()
#print("bytes: "+ str(gc.mem_alloc()))
#testInterrupt()
print("bytes: "+ str(gc.mem_alloc()))
gc.collect
while True:
    pass

testExit()