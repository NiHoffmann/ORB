import asyncio
import subprocess
import sys
import tempfile
import pathlib
import os
from typing import List

def mpy_cross_compile(file_name: str, file_contents: str ):
       with tempfile.TemporaryDirectory() as tmp_dir:
        tmp_dir = pathlib.Path(tmp_dir)

        with open(tmp_dir / "tmp.py", "w") as in_file:
            in_file.write(file_contents)

        args = ["mpy-cross", in_file.name, "-s", file_name]
        process = subprocess.run(args, capture_output=True)

        try:
            with open(tmp_dir / "tmp.mpy", "rb") as out_file:
                data = out_file.read()
        except OSError:
            data = None

        return process, data

async def compile_program(program: str, name: str):
    proc, mpy = mpy_cross_compile(name, program)

    if proc.returncode == 0:
        return "success", mpy
    else:
        return "error", proc.stderr.decode("utf-8")

def rreplace(s, old, new, occurence = 1):
        if occurence == 0:
            return s
        left, found, right = s.rpartition(old)
        if found == "":
            return right
        else:
            return rreplace(left, old, new, occurence - 1) + new + right


#Code joinked from https://gist.github.com/pavel-a/89d71b3aba9d7a9e6f8a61d728b08a8e
#Bin-to-hex.py (intel-hex)
HEX_LINE_LEN = 32

def ConstructRecord(recordType, address, data) -> bytes:

    assert 0 <= address < 0x10000
    assert 0 <= recordType <= 255
    record = []

    recordType = recordType & 0xFF
    address = (address >> 8) & 0xFF, address & 0xFF
    numBytes = len(data) & 0xFF
    
    record.append(numBytes)
    record += address
    record.append(recordType)
    record += data
    
    checksum = 0
    for x in record:
        checksum += x
    checksum &= 0xFF
    
    # Two's complement
    checksum = ~checksum + 1
    checksum &= 0xFF
    
    record.append(checksum)

    recordStr = ':'
    for byte in record:
        recordStr += '{:02X}'.format(byte)
    recordStr += '\n'
    
    return recordStr.encode('ascii', errors='strict')

def convertBinaryToHex(binaryPath, hexPath, start_addr = 0, noEndRecord = False, ignoreErasedRecords = True):
    
    address = start_addr & 0xFFFF  # initial offset, low part
    addr_high = (start_addr >> 16) & 0xFFFF

    hexFile = open(hexPath, 'wb')
    
    if address != 0 :
        hexFile.write(ConstructRecord(0x04, 0x0000, addr_high.to_bytes(2, 'big')))

    with open(binaryPath, 'rb') as binaryFile:
        byte = binaryFile.read(1)
        data = []
        while byte != b'':
            if address == 0 :
                hexFile.write(ConstructRecord(0x04, 0x0000, addr_high.to_bytes(2, 'big')))

            byte = int.from_bytes(byte, byteorder='big') & 0xFF
            data.append(byte)
            address += 1
            
            if len(data) >= HEX_LINE_LEN:
                for val in data:
                    if val != 0xFF or not ignoreErasedRecords:
                        hexFile.write(ConstructRecord(0x00, address - len(data), data))
                        break
                data = []
                if address > 0xFFFF:
                    addr_high += 1
                    address   -= 0x10000
                    assert address == 0, "start addr must be aligned on HEX_LINE_LEN else revise!"
                
            byte = binaryFile.read(1)

        # last incomplete row
        if len(data) :
            for val in data:
                if val != 0xFF or not ignoreErasedRecords:
                    hexFile.write(ConstructRecord(0x00, address - len(data), data))
                    break
        
    if not noEndRecord:
        hexFile.write(ConstructRecord(0x01, 0x0000, []))
    hexFile.close()


if __name__ == "__main__":
    if len(sys.argv) > 2 and sys.argv[2] == "True" :
        path = os.path.join(os.path.dirname(os.path.dirname(os.path.abspath(sys.argv[0]))), sys.argv[1])
    else:
        path = os.path.join(os.getcwd(), sys.argv[1])

    with open(path, 'r') as file:
        file_in = file.read()
    
    status, data = asyncio.run(compile_program(file_in, os.path.basename(path)))
    bin_path = rreplace(path, '.py' , '.bin' , 1)
    
    if status == "error":
         raise ValueError('Micropython-Script Compilation Error')

    print("compilation: " + status)
    print("data: " + str(list(data)))
    print("length: " + str(len(data)))
    flag = (0b00001111).to_bytes(1,'big')
    mpy_size = len(data).to_bytes(4,'big')
    print("size: "  + str(mpy_size))
    print("flag : " + str(flag))

    with open(bin_path, 'wb') as bin_file:
        bin_file.write(flag)
        bin_file.write(mpy_size)
        bin_file.write(data)
    
    hex_path = rreplace(path, '.py' , '.hex' , 1)

    convertBinaryToHex(bin_path, hex_path)


