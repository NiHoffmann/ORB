from devices import sensor, motor
import monitor
from time import getTime

a = sensor(port = sensor.S1, type = sensor.Analog, option = 0x2222)
motorLeft  = motor(port = motor.M1, direction = motor.REVERSE, ticks = 144, acc = 50, kp = 50, ki = 30)
motorRight = motor(port = motor.M2, direction = motor.FORWARD, ticks = 144, acc = 50, kp = 50, ki = 30)

s = 800.0 * (2/3)

#0-100 in 90ms
smothing = 1.0 / 90.0
percentLeft  = 1.0
percentRight = 1.0

was = getTime()
ist = was
while True:
    ist = getTime()

    lineRight  = not (a.getValueExt(2))
    lineLeft = not (a.getValueExt(3))

    if lineRight:
        percentLeft = 1.0
    else:
        percentLeft  = min(1.0, max(0, percentLeft -  (smothing * (ist-was))))

    if lineLeft:
        percentRight = 1.0
    else:
        percentRight = min(1.0, max(0, percentRight - (smothing * (ist-was))))

    motorLeft.set(mode = motor.SPEED_MODE,  speed = max(0, (percentLeft  + (1/3 * percentRight)))  * s)
    motorRight.set(mode = motor.SPEED_MODE, speed = max(0, (percentRight + (1/3 * percentLeft))) * s)

    was = ist
