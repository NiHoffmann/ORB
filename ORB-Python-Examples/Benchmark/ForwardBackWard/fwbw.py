from devices import sensor, motor
from time import getTime, wait
import monitor

colorLeft  = NXTLightSensor(sensor.S1) # type: ignore
colorRight = NXTLightSensor(sensor.S2) # type: ignore

#ensure same callibration as c++ code
colorLeft.setCallibration(1600, 2600)
colorRight.setCallibration(1600, 2600)

motorLeft  = motor(motor.M2)
motorRight = motor(motor.M1)
m_speed = 300.0
runFor = 60 * 1000 * 20 #20 Minutes
line_counter = 0
start = getTime()

motorLeft.set(mode=motor.SPEED_MODE, speed=m_speed)
motorRight.set(mode=motor.SPEED_MODE, speed=m_speed)
while  getTime() - start < runFor:
    if colorLeft.get() > 80 or colorRight.get() > 80:
        line_counter += 1
        motorLeft.config(direction = motor.REVERSE)
        motorRight.config(direction = motor.REVERSE)
        wait(250)
    else:
        motorLeft.config(direction = motor.FORWARD)
        motorRight.config(direction = motor.FORWARD)

    monitor.setText(0,"LineCounter" + str(line_counter))
motorLeft.set(mode=motor.SPEED_MODE, speed=0)
motorRight.set(mode=motor.SPEED_MODE, speed=0)
#we wait here to look at the values
while True:
    pass
