import monitor
from devices import sensor, motor
from time import getTime
import math
import gc

interrations = 10000

m = motor(0)
s = sensor(0)

start = getTime()
for _ in range(interrations):
    s.get()
finish = getTime()
monitor.setText(0, f'sensor.get(): {finish - start} ms')

start = getTime()
for _ in range(interrations):
    s.getDigital()
finish = getTime()
monitor.setText(1, f'sensor.getDigital(): {finish - start} ms')

start = getTime()
for _ in range(interrations):
    m.set()
finish = getTime()
monitor.setText(2, f'motor.set(): {finish - start} ms')

start = getTime()
for _ in range(interrations):
    m.get()
finish = getTime()
monitor.setText(3, f'motor.get(): {finish - start} ms')

start = getTime()
for _ in range(10000):
    gc.collect()
finish = getTime()
monitor.setText(0, f'gc.collect(): {finish - start} ms')

while True:
    pass
