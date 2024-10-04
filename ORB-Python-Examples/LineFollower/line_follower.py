from devices import sensor, motor
import time
import monitor
import time

class NXTLightSensor:
    class Mode:
        LIGHT_OFF = 0
        LIGHT_ON  = 1  

    def __init__(self, _port, _kallib0, _kallib1, _spike, _dipp, _coeffitient, _iterations = 50):   
        self.me = sensor(port = _port, type = sensor.Analog, mode = 0, option = (0x08|0x07)<<8)
        self.kalib0 = _kallib0
        self.kalib1 = _kallib1
        self.spike = _spike
        self.dipp = _dipp
        self.coeffitient = _coeffitient
        self.gain   = 100.0 /(self.kalib1 - self.kalib0)
        self.offset = self.kalib0
        self.iterations = _iterations

    def setMode(self, mode):
        if mode == NXTLightSensor.Mode.LIGHT_OFF:
            self.me.config(option = (0x00|0x07)<<8)
        if mode == NXTLightSensor.Mode.LIGHT_ON:
            self.me.config(option = (0x08|0x07)<<8)

    def get(self):
        it = self.iterations
        val = self.kalib0 - self.kalib0
        filter_coeffitient = self.coeffitient
        filtered_val = 0

        for i in range(self.iterations):
            current_val = self.me.get()["values"][0] & 0xFFFF
            if current_val > self.spike or current_val < self.dipp:
                it -= 1
                continue

            filtered_val = filter_coeffitient * current_val + (1 - filter_coeffitient) * filtered_val
            val += filtered_val

        if it == 0:
            return self.get()

        ret = min(100, max(0, self.gain * ((val / float(it)) - self.offset)))

        return ret

colorRight = NXTLightSensor(sensor.S1, 10000.0, 6000.0, 19000.0, 4000.0, 0.075)
colorLeft  = NXTLightSensor(sensor.S2, 1100.0, 600.0,  10000.0, 2000.0, 0.015)
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
    monitor.setText(0, "Time: "+str((now - was)/1000.0))
    monitor.setText(1, "Left isblack : "  + str(colorLeft.get()))
    monitor.setText(2, "Right isblack : " + str(colorRight.get()))
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