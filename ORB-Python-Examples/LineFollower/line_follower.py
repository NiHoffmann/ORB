from devices import sensor, motor
import time

class Mode():
    LIGHT = 0
    AMBIENT = 1
    COLOR = 2      

class Color():
    NONE = 0
    BLACK = 1
    BLUE = 2
    GREEN = 3
    YELLOW = 4
    RED = 5
    WHITE = 6
    BROWN = 7

class EV3ColorSensor:
    def __init__(self, _port):   
        self.me = sensor(port = _port, type = sensor.UART, mode = Mode.COLOR)    

    def get(self):
        return self.me.get()["values"][0] & 0xFFFF

colorLeft = EV3ColorSensor(sensor.S1)
colorRight = EV3ColorSensor(sensor.S2)
motorLeft = motor(port = motor.M1)
motorRight = motor(port = motor.M2)
motorLeft.set(mode = motor.SPEED_MODE)
motorRight.set(mode = motor.SPEED_MODE)

speedLeft =  25.0
speedRight = 25.0

last = time.getTime()
current = last
deltaTime = 0
gain = 0.00001

while True:
    current = time.getTime()
    deltaTime = last - current
    last = current

    if colorLeft.get() == Color.BLACK:
        speedLeft = min(speedLeft + (gain * deltaTime), 100)
    else:
        speedLeft = max(speedLeft - (gain * deltaTime), 0)

    if colorRight.get() == Color.BLACK:
        speedRight = min(speedRight + (gain * deltaTime), 100)
    else:
        speedRight = max(speedRight - (gain * deltaTime), 0)

    motorLeft.set(speed = int(speedLeft))
    motorRight.set(speed = int(speedRight))
