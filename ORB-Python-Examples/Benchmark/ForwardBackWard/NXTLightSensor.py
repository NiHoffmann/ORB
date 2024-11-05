from devices import sensor
from memory import setMemory, getMemory

class NXTLightSensor:
    kallib0 = 0
    kallib1 = 0
    gain = 0
    offset = 0

    class Mode:
        LIGHT_OFF = 0
        LIGHT_ON  = 1  

    def __init__(self, _port):   
        self.me = sensor(port = _port, type = sensor.Analog, mode = 0, option = (0x08|0x07)<<8)
        self.port = _port
        self.loadCallibration()

    def setMode(self, mode):
        if mode == NXTLightSensor.Mode.LIGHT_OFF:
            self.me.config(option = (0x00|0x07)<<8)
        if mode == NXTLightSensor.Mode.LIGHT_ON:
            self.me.config(option = (0x08|0x07)<<8)

    def saveCallibration(self):
        setMemory(self.port * 4, [self.kallib0 >> 4, self.kallib0, self.kallib1 >> 4 , self.kallib1])

    def loadCallibration(self):
        data = getMemory(self.port * 4, 4)
        self.setCallibration((data[0] << 4) + data[1], (data[2] << 4) + data[3])
        
    def updateCallibration(self):
        if (self.kallib0 - self.kallib1) != 0:
            self.gain   = 100.0 /(self.kallib1 - self.kallib0)
            self.offset = self.kallib0

    def setCallibration(self, k0, k1):
        self.kallib0 = k0
        self.kallib1 = k1
        self.updateCallibration()
        
    def callibrate0(self):
        self.kallib0 = self.getRaw()
        self.updateCallibration()

    def callibrate1(self):
        self.kallib1 = self.getRaw()
        self.updateCallibration()

    def getRaw(self):
        return self.me.get()["values"][0] & 0xFFF

    def get(self):
        normalied_value = self.gain * (self.getRaw() - self.offset)
        return min(100, max(0, normalied_value))
    