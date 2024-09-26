from devices import servo

def servoTest():
    print("=== Constructor Function ===")
    print("a = servo(servo.S1)")
    a = servo(servo.S1)
    print("a = servo(servo.S2)")
    b = servo(servo.S2)
    print("\n=== Set Function ===")
    print("a.set(speed = 10, angle = 20)")
    a.set(speed = 10, angle = 20)
    print("a.set(angle = 120)")
    a.set(angle = 120)
    print("a.set(speed = 210)")
    a.set(speed = 210)
    print("b.set(speed = 10)")
    b.set(speed = 10)
    print("\n=== Servo Parameter ===")
    print(f"servo S1:       {servo.S1}")
    print(f"servo S2:       {servo.S2}")
    print("=======================")

servoTest()

