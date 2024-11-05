from devices import sensor, motor
import monitor
from time import getTime

a = sensor(port = sensor.S1, type = sensor.Analog, option = 0x2222)
motorLeft  = motor(port = motor.M1, direction = motor.REVERSE, ticks = 144, acc = 50, kp = 50, ki = 30)
motorRight = motor(port = motor.M2, direction = motor.FORWARD, ticks = 144, acc = 50, kp = 50, ki = 30)

#this works
s = 2750.0

while True:
    lineRight  = not (a.getValueExt(2))
    lineLeft = not (a.getValueExt(3))
    
    motorLeft.set(mode = motor.SPEED_MODE, speed = s * lineRight)
    motorRight.set(mode = motor.SPEED_MODE, speed = s * lineLeft)
