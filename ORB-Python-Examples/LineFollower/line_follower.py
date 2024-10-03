from devices import sensor, motor
import time
import monitor
import time

class NXTLightSensor:
    NONE  = 0
    BLACK = 1
    WHITE = 2

    class Mode:
        LIGHT_OFF = 0
        LIGHT_ON  = 1  

    #all of this can be callibarated by tracking the lowest/higheset values and removing them with 
    #after that determin kallib0 and kallib1
    kalib0 = 10000.0
    kalib1 = 6000.0
    spike = 19000.0
    dipp = 4000.0

    gain   = 100.0 /(kalib1- kalib0)
    offset = kalib0

    def __init__(self, _port):   
        self.me = sensor(port = _port, type = sensor.Analog, mode = 0, option = (0x08|0x07)<<8)

    def setMode(self, mode):
        if mode == NXTLightSensor.Mode.LIGHT_OFF:
            self.me.config(option = (0x00|0x07)<<8)
        if mode == NXTLightSensor.Mode.LIGHT_ON:
            self.me.config(option = (0x08|0x07)<<8)

    def get(self, iterations = 50, coeffitient = 0.075):
        it = iterations
        val = self.kalib0 - self.kalib0
        filter_coeffitient = coeffitient
        filtered_val = 0

        for i in range(iterations):
            current_val = self.me.get()["values"][0] & 0xFFFF
            if current_val > self.spike or current_val < self.dipp:
                it -= 1
                continue
            filtered_val = filter_coeffitient * current_val + (1 - filter_coeffitient) * filtered_val
            val += filtered_val

        monitor.setText(2, "filter val: "+ str(filtered_val))
        monitor.setText(0,"value: " + str(val/float(it)))
        ret = min(100, max(0, self.gain * ((val / float(it)) - self.offset)))
        #monitor.setText(1, "value(%) : " + str(ret))

        return ret

colorRight = NXTLightSensor(sensor.S1)
"""
colorLeft = NXTLightSensor(sensor.S2)

motorRight = motor(port = motor.M1, direction = motor.REVERSE)
motorLeft = motor(port = motor.M2, direction = motor.REVERSE)
motorLeft.set(mode = motor.POWER_MODE)
motorRight.set(mode = motor.POWER_MODE)

speedLeft =  25.0
speedRight = 25.0

last = time.getTime()
current = last
deltaTime = 0
gain = 0.1
"""

now = time.getTime()
was = now
while True:
    now = time.getTime()
    monitor.setText(2, "Time: "+str((now - was)/1000.0))
    monitor.setText(3, "isblack : " + str(colorRight.get()))
    colorRight.get()
    was = now
    """
    current = time.getTime()
    deltaTime =  current - last
    last = current

    if colorRight.get() < 25:
        speedLeft = min(speedLeft + (gain * deltaTime), 150)
    else:
        speedLeft = max(speedLeft - (gain * deltaTime), 80)

    if colorLeft.get() < 25:
        speedRight = min(speedRight + (gain * deltaTime), 150)
    else:
        speedRight = max(speedRight - (gain * deltaTime), 80)

    monitor.setText(0,"Left: " + str(speedLeft))
    monitor.setText(1,"Right: " + str(speedRight))

    motorLeft.set(speed = int(speedLeft))
    motorRight.set(speed = int(speedRight))
    """