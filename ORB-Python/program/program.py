import gc
from devices import motor

a = motor(0)
report = a.get()
print(report)
print(report.speed)
report.speed = 123.2 + 2 
print(report.position)
print(report.speed)
report.power = "hallo"
