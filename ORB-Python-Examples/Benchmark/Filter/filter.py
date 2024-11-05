from devices import sensor, motor
from time import getTime, wait
import monitor

colorLeft  = sensor(port = sensor.S1, type = sensor.Analog, mode = 0, option = (0x08|0x07)<<8)

#arbitrary filter reading smoothing + clipping 
coeffitient = 0.005
spike = 20000
dipp = 2000
interrations = 1000
def readSensorWithFilter():
    filtered_val = 0.0
    val = 0.0
    actual_interations = interrations

    for i in range(interrations):
        current_val = colorLeft.get()["values"][0] & 0xFFF
        if current_val > spike or current_val < dipp:
                actual_interations -= 1
                continue
        filtered_val = coeffitient * current_val + (1 - coeffitient) * filtered_val
        val += filtered_val

    if actual_interations == 0:
        return 0
    
    average_value = val / float(actual_interations)
    return average_value

start = getTime()
ist = start
war = ist

sum = 0
count = 0
while True:
    ist = getTime()
    val = readSensorWithFilter()
    deltaTime = ist - war
    sum += deltaTime
    count += 1
    monitor.setText(0, "FilteredValue: " + str(val))
    monitor.setText(1, "DeltaTime: " + str(deltaTime))
    monitor.setText(2, "AvgTime: " + str(sum/count))
    war = ist
