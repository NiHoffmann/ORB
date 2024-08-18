# devices/sensor.py
    
class sensor:

    def __init__(self, port: int, type: int, mode: int, mem_offset:int):
        return
    
    def config(self, type:int , mode:int, mem_offset: int):
        return

    def get(self):     
        return
    
    def getDigital(self):
        return

    def getValueExt(self, channel: int):
        return
    
    S1 = 0
    S2 = 1
    S3 = 2
    S4 = 3 
    Analog = 0
    T2C = 1
    TOF = 2
    Touch = 3 
    UART = 4

class motor:
    def __init__(self, port: int, direction: int, ticks: int, acc: int, kp: int, ki: int):
        return

    def config(self, direction: int, ticks: int, acc: int, kp: int, ki: int):
        return
    
    def set(self, mode: int, speed: int, position: int):
        return
    
    def get(self):
        return

    M1 = 0
    M2 = 1
    M3 = 2
    M4 = 3
    FORWARD = +1
    REVERSE = -1
    POWER_MODE  = 0
    BRAKE_MODE  = 1
    SPEED_MODE  = 2
    MOVETO_MODE = 3

class servo:

    def __init__(self, port: int, ):
        return
    
    def set(self, speed: int, angle: int):
        return
    
    S1 = 0
    S2 = 1