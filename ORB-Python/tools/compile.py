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
    mpy_size = len(data).to_bytes(4,'big')
    print(mpy_size)

    with open(bin_path, 'wb') as bin_file:
        bin_file.write(mpy_size)
        bin_file.write(data)
