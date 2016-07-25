#!/usr/bin/env python

##### IMPORT #################################################################

import argparse
import glob
import os
import platform
import subprocess
import sys
import distutils

project_dir = os.path.abspath(".");

test_list = ["test_atom", "test_dsp"]

#==============================================================================
#Name : get_test_file
#==============================================================================

def get_test_file(test_name):
	test_file = test_name;
	if platform.system() == "Windows":
		test_file = test + ".exe"
	return test_file

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
#Name : main
#==============================================================================

args = parse_args()

config_path = ""

if (args.configuration == "Release"):
    config_path = os.path.join(config_path, "Release")
elif(args.configuration == "Debug"):
    config_path = os.path.join(config_path, "Debug")

platform_path = config_path

if platform.system() == "Windows":
    if (args.platform == "x64"):
        platform_path = os.path.join(platform_path, "x64")
    elif(args.platform == "Win32"):
        platform_path = os.path.join(platform_path, "Win32")

test_dir = os.path.join(project_dir, "Build", platform_path, "KiwiBuild", "Test", config_path)

for test in test_list:
	subprocess.check_call(os.path.join(test_dir, get_test_file(test)), shell= True);
