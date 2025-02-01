import gc
from devices import motor, sensor 
'''
a = motor(0)
report = a.get()
print(report)
print(report.speed)
print(report.position)
print(report.speed)
report.power = 1
'''
a = sensor(0)
report = a.get()
print(report)
print(report.values)
print(report.type)
print(report.option)
print(report.lenExp)
report.lenExp = 1