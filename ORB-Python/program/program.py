from devices import sensor, motor
import time
import monitor
import time

class NXTLightSensor:
    class Mode:
        LIGHT_OFF = 0
        LIGHT_ON  = 1  

    def __init__(self, _port, _kallib0, _kallib1, _spike, _dipp, _coeffitient, _iterations = 30):   
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

    def getRaw(self):
        return self.me.get()["values"][0] & 0xFFFF

    def get(self):
        actual_interations = self.iterations
        val = 0
        filtered_val = 0

        for i in range(self.iterations):
            current_val = self.me.get()["values"][0] & 0xFFFF
            if current_val > self.spike or current_val < self.dipp:
                actual_interations -= 1
                continue

            filtered_val = self.coeffitient * current_val + (1 - self.coeffitient) * filtered_val
            val += filtered_val

        if actual_interations == 0:
            return self.get()
        
        average_value = val / float(actual_interations)
        normalied_value = self.gain * (average_value - self.offset)

        return min(100, max(0, normalied_value) )

colorRight = NXTLightSensor(sensor.S1, 8500.0, 4500.0, 19000.0, 4000.0, 0.085)
colorLeft  = NXTLightSensor(sensor.S2, 1300.0, 400.0,  10000.0, 2000.0, 0.025)

motorRight = motor(port = motor.M1, direction = motor.REVERSE)
motorLeft = motor(port = motor.M2, direction = motor.REVERSE)
motorLeft.set(mode = motor.SPEED_MODE)
motorRight.set(mode = motor.POWER_MODE)


motorLeft.set(speed = 100)

while True:
    pass