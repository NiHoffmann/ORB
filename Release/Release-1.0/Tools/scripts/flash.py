import subprocess
import sys
import os

def flash(fw_name):
	script_directory = os.path.dirname(os.path.abspath(__file__))
	target_path = os.path.join(os.path.dirname(script_directory), 'Firmware', fw_name)

	subprocess.run([ "dfu-util.exe", "-a", "0" ,"-D" ,target_path]) 