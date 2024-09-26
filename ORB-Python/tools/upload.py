import subprocess
import sys
import os

address_offset = "0"
dfuse_address = " 0x080E0000"

bin_path = "../program/program.bin";

subprocess.run([ ".\dfu-util.exe", "-a", address_offset , "--dfuse-address", dfuse_address ,"-D" , bin_path]) 
