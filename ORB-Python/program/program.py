from devices import motor

def testMotor():
    a = motor(port = motor.M1, direction =  motor.REVERSE,ticks=100,acc=10,kp=5,ki=15)
    a.set(10, 20, 24)
    a.config(1)

testMotor()