import monitor
from monitor import getKey, setText, keys

def monitorTest():
    print("=== getKey Test ===")
    print("call key = monitor.getKey()")
    key = monitor.getKey()
    print(key)
    print("=== setText Test ===")
    print("call setText(1, \"hallo\")")
    setText(1, "hallo")
    print("")
    print("=== Test keys ===")
    print(f"NO_KEY: {keys.NO_KEY}")
    print(f"A1: {keys.A1}")
    print(f"B1: {keys.B1}")
    print(f"C1: {keys.C1}")
    print("=======================")

monitorTest()