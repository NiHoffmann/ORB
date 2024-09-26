from devices import motor
from time import wait

def testMotor():
    a = motor(port = motor.M1, direction =  motor.REVERSE)
    a.config(direction = motor.REVERSE)
    a.set(mode = motor.POWER_MODE, speed = 245)
    print(a.get())
    wait(2000)
    a.set(mode = motor.BRAKE_MODE)

testMotor()