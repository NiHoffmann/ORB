import monitor
from time import getTime
import math

def sine_approx(x):
    #we need to calculate with radiants to have smaller numbers
    x = x * (math.pi / 180)  
    #limit range to [0,2pi] - we need to do this overwise we get a overflow error becaues of too high numbers
    x = x % (2 * math.pi)
    i = 1
    cur = x
    acc = 1
    fact = 1
    pow = x

    while abs(acc) > 1e-8 and i < 5:
        fact *= (2 * i) * (2 * i + 1)
        pow *= -1 * x * x 
        acc = pow / fact
        cur += acc
        i += 1

    return cur

def squared():
    square = 0
    for i in range(10000):
        square = i*i

def sqrt():
    for i in range(1000):
        sqrt_newton(i)

def sqrt_newton(number):
    if number == 0: return 0

    estimate = number 
    tolerance = 1e-10

    while True:
        next_estimate = (estimate + number / estimate) / 2 
        if abs(next_estimate - estimate) < tolerance:
            break
        estimate = next_estimate

    return estimate

i = 0
start = getTime()
while i <= 360 :
    #uncomment for debugging
    #monitor.setText(0, "sin(" + str(i) + ") = " + str(sine_approx(i)))
    i += 0.025
finish = getTime()
monitor.setText(0,"Sin(0-360): " + str(finish-start) + "ms")

start = getTime()
squared()
finish = getTime()
monitor.setText(1,"squares: " + str(finish-start) + "ms")

start = getTime()
sqrt()
finish = getTime()
monitor.setText(2,"sqrt_newton: " + str(finish-start)+"ms")

while True:
    pass
