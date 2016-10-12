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

test_list = ["test_atom"]

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


if (platform.system() == "Darwin"):
    test_dir = os.path.join(project_dir, "Build", args.configuration, "KiwiBuild", "Test", args.configuration)
elif(platform.system() == "Windows"):
    test_dir = os.path.join(project_dir, "Build", args.configuration, args.platform, "KiwiBuild", "Test", args.configuration)
elif(platform.system() == "Linux"):
    test_dir = os.path.join(project_dir, "Build", args.configuration, "KiwiBuild", "Test")
    

for test in test_list:
	subprocess.check_call(os.path.join(test_dir, get_test_file(test)), shell= True);
