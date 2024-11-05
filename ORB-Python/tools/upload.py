import subprocess
import sys
import os

def upload(bin_path):
    address_offset = "0"
    dfuse_address = " 0x080E0000"
    subprocess.run([ "dfu-util.exe", "-a", address_offset , "--dfuse-address", dfuse_address ,"-D" , bin_path]) 

if __name__ == "__main__":
    bin_path = "../program/program.bin";
    upload(bin_path)