from devices import sensor, motor
import time
import monitor
import time

#this value works well for lego motors
motorLeft = motor(port = motor.M2, ticks = 1200)
motorLeft.set(mode = motor.SPEED_MODE)

_speed = -100

motorLeft.set(speed = _speed)
while True:
    monitor.setText(0, "speed set: " + str(_speed))
    monitor.setText(1, "speed cur: " + str(motorLeft.get()["speed"]))
    monitor.setText(2, "power cur: " + str(motorLeft.get()["power"]))
    _speed += 1
    if(_speed >= 100):
        _speed = -100
    time.wait(100)
    motorLeft.set(speed = _speed)
    pass