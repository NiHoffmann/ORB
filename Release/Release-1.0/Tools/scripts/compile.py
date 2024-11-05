import asyncio
import subprocess
import sys
import tempfile
import pathlib
import os
from typing import List
import argparse
import json

def mpy_cross_compile(file_name: str, file_contents: str ):
       with tempfile.TemporaryDirectory() as tmp_dir:
        tmp_dir = pathlib.Path(tmp_dir)

        with open(tmp_dir / "tmp.py", "w") as in_file:
            in_file.write(file_contents)

        args = ["mpy-cross.exe", in_file.name, "-s", file_name]
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
HEX_LINE_LEN = 16

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

def compileOneFile(_path, main = False):
    with open(os.path.join(os.getcwd(), _path), 'r') as file:
            _file_in = file.read()
    if not main:
        _file_in = "__name__ = '__import__'\n" + _file_in
    else:
        _file_in = "__name__ = '__main__'\n" + _file_in
    status, data = asyncio.run(compile_program(_file_in, os.path.basename(_path)))
    if status == "error":
            raise ValueError('Micropython-Script Compilation Error')
    if status == "error":
        print("Compilation Error")
    return data

def compile(path, dot_build_script, file_name=None, verbose=False):
    flag = (0b00001111).to_bytes(1,'big')

    if not dot_build_script:
        if file_name != None:
            path = path + '\\' + file_name
        
        with open(path, 'r') as file:
            file_in = file.read()
        
        status, data = asyncio.run(compile_program(file_in, os.path.basename(path)))
        bin_path = rreplace(path, '.py' , '.bin' , 1)
        
        if status == "error":
            raise ValueError('Micropython-Script Compilation Error')
    
        mpy_size = len(data).to_bytes(4,'big')
        frame_size = (len(data)+5).to_bytes(4,'big')
        num_modules = 1
        
        print("compilation: " + status)
        if(verbose):
            print("data: " + str(list(data)))
            print("length: " + str(len(data)))
            print("size: "  + str(mpy_size))
            print("flag : " + str(flag))

        with open(bin_path, 'wb') as bin_file:
            bin_file.write(flag)
            bin_file.write(frame_size)
            bin_file.write((num_modules).to_bytes(1,'big'))
            bin_file.write(mpy_size)
            bin_file.write(data)
        
        hex_path = rreplace(path, '.py' , '.hex' , 1)

        convertBinaryToHex(bin_path, hex_path)
        return rreplace(os.path.join(path), '.py' , '' , 1)
    
    else:
        with open(path + '\.build', 'r') as file:
            build_data = json.load(file)
        import_files = build_data.get("import", [])
        main_file = build_data.get("main", "")
        
        #compile import
        data_array = []
        length_array = []
        num_modules = 0
        for import_file in import_files:
            num_modules += 1
            d = compileOneFile(import_file)
            data_array.append(d)
            length_array.append(len(d).to_bytes(4,'big'))

        #compile main
        num_modules += 1
        data_main = compileOneFile(main_file, True)
        data_array.append(data_main)
        length_array.append(len(data_main).to_bytes(4,'big'))
        bin_file_path = rreplace(main_file, '.py' , '.bin' , 1)
        hex_file_path = rreplace(main_file, '.py' , '.hex' , 1)

        #put everything together
        with open(bin_file_path, 'wb') as bin_file:
            bytes = b''.join(data_array)
            bytes_length = b''.join(length_array)
            mpy_size = len(bytes).to_bytes(4,'big')
            frame_size = (len(bytes) + len(bytes_length) + 5).to_bytes(4,'big')
            
            bin_file.write(flag)
            bin_file.write(frame_size)
            bin_file.write((num_modules).to_bytes(1,'big'))
            for b in data_array:
                bin_file.write(len(b).to_bytes(4,'big'))
            bin_file.write(bytes)

        print("compilation: Success")
        if verbose:
            print("Flag: "+str(flag))
            print("Frame Size: " + str(frame_size))
            print("Num Modules: "+ str(num_modules))
            print("Bytes: " + str(bytes))

            
        convertBinaryToHex(bin_file_path, hex_file_path)
    
        return rreplace(os.path.join(path, main_file), '.py' , '' , 1)