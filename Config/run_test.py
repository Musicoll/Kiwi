#!/usr/bin/env python

##### IMPORT #################################################################

import argparse
import os
import platform
import subprocess
import sys
import distutils

project_dir = os.path.abspath(".");

test_list = ["test_atom"]

#==============================================================================
#Name : parse_args
#==============================================================================

def parse_args ():
    arg_parser = argparse.ArgumentParser ()
    
    arg_parser.add_argument('-c', '--configuration', default = 'Release', choices = ['Release', 'Debug'])

    if platform.system() == "Windows":
        arg_parser.add_argument('-p', '--platform', default = 'x64', choices = ['x64', 'Win32'])

    return arg_parser.parse_args (sys.argv[1:])

#==============================================================================
#Name : run_recurse
#==============================================================================

def run_test (dir):
	subfiles = os.listdir(dir)

	for filename in subfiles:
		filepath = os.path.join(dir, filename);
		print(filepath)

		if os.path.isdir(filepath):
			run_recurse(filepath)
		else:
			exe_file = distutils.spawn.find_executable(filename)
			if (exe_file):
				subprocess.check_call(exe_file, shell= True);

	return arg_parser.parse_args (sys.argv[1:])

#==============================================================================
#Name : main
#==============================================================================

args = parse_args()

config_path = ""

if (args.configuration == "Release"):
    config_path = os.path.join(config_path, "Release")
elif(args.configuration == "Debug"):
    config_path = os.path.join(config_path, "Debug")
    
if platform.system() == "Windows":
    if (args.platform == "x64"):
        config_path = os.path.join(config_path, "x64")
    elif(args.configuration == "Win32"):
        config_path = os.path.join(config_path, "Win32")

test_dir = os.path.join(project_dir, "Build", config_path, "KiwiBuild", "Test", config_path)

for test in test_list:
	subprocess.check_call(os.path.join(test_dir, test), shell= True);
