import subprocess
import sys
import os

def flash(fw_name):
	script_directory = os.path.dirname(os.path.abspath(__file__))
	target_path = os.path.join(os.path.dirname((os.path.dirname(script_directory))), 'ORB-Firmware', 'Bin', fw_name)

	subprocess.run([ "dfu-util.exe", "-a", "0" ,"-D" ,target_path]) 

if __name__ == "__main__":
	if len(sys.argv) > 1:
		fw_name = sys.argv[1]
	else:
		fw_name = 'orb-hw0022-fw0100.dfu'
	flash(fw_name)