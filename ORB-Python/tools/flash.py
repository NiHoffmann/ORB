import subprocess
import sys

if len(sys.argv) > 1:
	fw_name = sys.argv[1]
else:
	fw_name = 'orb-hw0022-fw0100.dfu'

subprocess.run([ ".\dfu-util.exe", "-a", "0" ,"-D" ,"./../../ORB-Firmware/Bin/" + fw_name]) 