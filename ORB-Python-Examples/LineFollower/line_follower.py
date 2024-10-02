from devices import sensor, motor
import time
import monitor
import time

class NXTLightSensor:
    class Mode:
        LIGHT_OFF = 0
        LIGHT_ON  = 1  

    kalib0 = 19500.0
    kalib1 = 20000.0

    gain   = 100.0 /(kalib1- kalib0)
    offset = kalib0

    def __init__(self, _port):   
        self.me = sensor(port = _port, type = sensor.Analog, mode = 0, option = (0x08|0x07)<<8)

    def setMode(self, mode):
        if mode == NXTLightSensor.Mode.LIGHT_OFF:
            self.me.config(option = (0x00|0x07)<<8)
        if mode == NXTLightSensor.Mode.LIGHT_ON:
            self.me.config(option = (0x08|0x07)<<8)

    def get(self):
        val = 0
        for i in range(250):
            val += self.me.get()["values"][0] & 0xFFFF
        return min(100, max(0, NXTLightSensor.gain * ((val/250.0) - NXTLightSensor.offset)))

colorLeft = NXTLightSensor(sensor.S1)

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
    monitor.setText(0, "Left: " + str(colorLeft.get()))
    """
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
    """