import monitor
from devices import motor

a = motor(0)
a.set(mode=motor.POWER_MODE, speed = 50)

def testMotor():
    i = 0
    while i < 10000:
        monitor.setText(0, "Hallo" + str(i) + "\n")
        i += 1

testMotor()

a.set(mode = motor.BRAKE_MODE)