import argparse
import os
import subprocess
from compile import compile
from upload import upload
from flash import flash

def main():
    parser = argparse.ArgumentParser()

    parser.add_argument('execution_path', type=str, help='The execution path')
    parser.add_argument('-b', action='store_true', help='Build-Script flag')
    parser.add_argument('-c', action='store_true', help='Only compile flag')
    parser.add_argument('-f', type=str, help='Specific file name')
    parser.add_argument('-k', type=str, help='ORB-Keys File (default:ORB-Monitor-Tasten.txt)')
    parser.add_argument('-t', choices=['win', 'orb', 'dfu'], default='orb', help='Target platform (default: orb)')
    parser.add_argument('--flash', choices=['0.22','0.33','1.0'], help='Flash the firmware')
    parser.add_argument('--clean', action='store_true', help='Clean Build Files')
    parser.add_argument('--verbose', action='store_true', help='Add Compilation Information')

    args = parser.parse_args()

    execution_path = args.execution_path
    build_flag = args.b
    compile_only_flag = args.c
    specific_file = args.f
    keys_path = args.k
    target_platform = args.t
    flash_version = args.flash
    clean_project = args.clean
    print_verbose = args.verbose

    script_directory = os.path.dirname(os.path.abspath(__file__))
    orb_monitor_path = os.path.join(script_directory, "ORB-Monitor.exe")
    release_path = os.path.join(os.path.dirname(script_directory), 'build', 'bin', 'Release', "ORB-Python.exe")

    if clean_project:
        for root, dirs, files in os.walk(execution_path):
            for file in files:
                if file.endswith('.bin') or file.endswith('.hex'):
                    file_path = os.path.join(root, file)
                    try:
                        os.remove(file_path)
                        print(f"Removed: {file_path}")
                    except Exception as e:
                        print(f"Error removing {file_path}: {e}")
        return

    if flash_version != None:
        if flash_version == '0.22':
            flash('orb-hw0022-fw0100.dfu')
        if flash_version == '0.33':
            flash('orb-hw0033-fw0100.dfu')
        if flash_version == '1.0':
            flash('orb-hw0100-fw0100.dfu')
        return

    if build_flag:
        specific_file = None
        if not os.path.exists(execution_path+"/.build"):
            print("Could not find .build file.")
            return

    if specific_file == None and build_flag == False:
        parser.print_help()
        return

    out_file_path_no_extension = os.path.join(compile(execution_path, build_flag, specific_file, print_verbose))

    if not compile_only_flag:
        if target_platform == 'win':
            try:
                subprocess.run([release_path, out_file_path_no_extension+".bin"])
            except KeyboardInterrupt:
                pass
        if target_platform == 'orb':
            if keys_path == None:
                keys_path = "ORB-Monitor-Tasten.txt"  
            subprocess.Popen(f"{orb_monitor_path} -prog {out_file_path_no_extension}.hex -k {os.path.join(execution_path, keys_path)}", shell = True)
        if target_platform == 'dfu':
            upload(out_file_path_no_extension+".bin")

if __name__ == "__main__":
    main()
