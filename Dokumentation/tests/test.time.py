import time
from time import getTime, wait

def testTime():
    print("=== Print Module ===")
    print(time)
    print("=== getTime Function ===")
    print("a = getTime()")
    a = getTime()
    print(a)
    print("=== wait Function ===")
    print("wait(20)")
    a = wait(20)
    print("")
    print(a)
    print("=======================")

testTime()
    