import memory
from memory import setMemory,getMemory,clearMemory

def testMemory():
    print("=== Print Module ===")
    print(memory)
    print("=== setMemory Function ===")
    print("call setMemory(0, [1,2,3,4,5])")
    setMemory(0, [1,2,3,4,5])
    print("call memory.setMemory(0x0112, [3,4,5])")
    memory.setMemory(0x0112, [3,4,5])
    print("=== getMemory Function ===")
    print("call a = getMemory()")
    a = getMemory(0x800020, 128)
    print("call print(a)")
    print(a)
    print("=== Iterrate Byte Array ===")
    for i in range(5):
        print(a[20+ i])
    print("=== clearMemory Function ===")
    print("call clearMemory()")
    clearMemory()
    print("=======================")
    
testMemory()
