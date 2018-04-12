#!/usr/bin/env python

##### IMPORT #################################################################

import os
import sys
import platform
import subprocess
import shutil
import argparse

##### VERSION #################################################################

if sys.version_info[0] != 2 or sys.version_info[1] < 7:
    print("This script requires version python 2, 2.7 or higher")
    sys.exit(1)

project_dir = os.path.abspath(".");

cmd = "cmake";

#==============================================================================
# Name : parse_args
#==============================================================================

def parse_args ():
    arg_parser = argparse.ArgumentParser ()

    arg_parser.add_argument('-c', '--configuration', default = 'All', choices = ['Debug', 'Release' ,'All'],
                            help="the chosen configuration  to initialize either Release or Debug")

    if platform.system () == 'Linux':
        arg_parser.add_argument('-coverage', required=False, action='store_true', help='activate coverage')

    if platform.system() == "Windows":
        arg_parser.add_argument('-p','--platform', default = 'All', choices = ['x64', 'Win32', 'All'],
                                help="the chosen platform for which you want to later build kiwi")

    return arg_parser.parse_args (sys.argv[1:])

#==============================================================================
# Name : update_flip
#==============================================================================
def update_flip():
    subprocess.check_call("python " + os.path.join(project_dir, "Scripts", "flip.py"), shell= True);

#==============================================================================
# Name : create_dir
#==============================================================================

def create_dir(dir):
	if not os.path.exists(dir):
	   os.makedirs(dir);

#==============================================================================
# Name : mac_release
#==============================================================================

def mac_release():
    cmd_loc = cmd;
    build_dir = os.path.join(project_dir, "Build", "Release");
    create_dir(build_dir);
    os.chdir(build_dir);
    cmd_loc+= " -DCMAKE_BUILD_TYPE:STRING=Release";
    cmd_loc+= " " + project_dir;
    subprocess.check_call(cmd_loc, shell= True);
    return


#==============================================================================
# Name : mac_debug
#==============================================================================

def mac_debug():
    cmd_loc = cmd;
    build_dir = os.path.join(project_dir, "Build", "Debug");
    create_dir(build_dir);
    os.chdir(build_dir);
    cmd_loc += " -DCMAKE_BUILD_TYPE:STRING=Debug"
    cmd_loc += " " + project_dir;
    subprocess.check_call(cmd_loc, shell= True);
    return

#==============================================================================
# Name : configure_mac
#==============================================================================

def configure_mac(args):
    global cmd;
    cmd += " -G Xcode";

    if (args.configuration == "Debug" or args.configuration == "All"):
        mac_debug();

    if (args.configuration == "Release" or args.configuration == "All"):
        mac_release();

    return

#==============================================================================
# Name : win_debug_32
#==============================================================================

def win_debug_32():
    cmd_loc = cmd;
    build_dir = os.path.join(project_dir, "Build", "Debug", "Win32");
    create_dir(build_dir);
    os.chdir(build_dir);
    cmd_loc += "\""
    cmd_loc += " -DCMAKE_BUILD_TYPE:STRING=Debug"
    cmd_loc += " " + project_dir
    subprocess.check_call(cmd_loc, shell= True)
    return

#==============================================================================
# Name : win_debug_64
#==============================================================================

def win_debug_64():
    cmd_loc = cmd;
    build_dir = os.path.join(project_dir, "Build", "Debug", "x64");
    create_dir(build_dir);
    os.chdir(build_dir);
    cmd_loc += " Win64\""
    cmd_loc += " -DCMAKE_BUILD_TYPE:STRING=Debug"
    cmd_loc += " " + project_dir
    subprocess.check_call(cmd_loc, shell= True)
    return

#==============================================================================
# Name : win_release_32
#==============================================================================

def win_release_32():
    cmd_loc = cmd;
    build_dir = os.path.join(project_dir, "Build", "Release", "Win32");
    create_dir(build_dir);
    os.chdir(build_dir);
    cmd_loc += "\""
    cmd_loc += " -DCMAKE_BUILD_TYPE:STRING=Release"
    cmd_loc += " " + project_dir
    subprocess.check_call(cmd_loc, shell= True)
    return

#==============================================================================
# Name : win_release_64
#==============================================================================


def win_release_64():
    cmd_loc = cmd;
    build_dir = os.path.join(project_dir, "Build", "Release", "x64");
    create_dir(build_dir);
    os.chdir(build_dir);
    cmd_loc += " Win64\""
    cmd_loc += " -DCMAKE_BUILD_TYPE:STRING=Release"
    cmd_loc += " " + project_dir
    subprocess.check_call(cmd_loc, shell= True)
    return

#==============================================================================
# Name : configure_windows
#==============================================================================

def configure_windows(args):
    global cmd

    cmd += " -G \"Visual Studio 14 2015"

    if (args.platform == "x64" or args.configuration == "All"):
        if (args.configuration == "Debug" or args.configuration == "All"):
            win_debug_64();
        if(args.configuration == "Release" or args.configuration == "All"):
            win_release_64();

    if(args.platform == "Win32" or args.platform == "All"):
        if (args.configuration == "Debug" or args.configuration == "All"):
            win_debug_32();
        if (args.configuration == "Release" or args.configuration == "All"):
            win_release_32();

    return

#==============================================================================
# Name : linux_debug
#==============================================================================

def linux_debug():
    cmd_loc = cmd;
    build_dir = os.path.join(project_dir, "Build", "Debug");
    create_dir(build_dir);
    os.chdir(build_dir);
    cmd_loc += " -DCMAKE_BUILD_TYPE:STRING=Debug"
    cmd_loc += " " + project_dir
    subprocess.check_call(cmd_loc, shell= True)
    return

#==============================================================================
# Name : linux_release
#==============================================================================

def linux_release():
    cmd_loc = cmd;
    build_dir = os.path.join(project_dir, "Build", "Release");
    create_dir(build_dir);
    os.chdir(build_dir);
    cmd_loc += " -DCMAKE_BUILD_TYPE:STRING=Release"
    cmd_loc += " " + project_dir
    subprocess.check_call(cmd_loc, shell= True)
    return


#==============================================================================
# Name : configure_linux
#==============================================================================

def configure_linux(args):
    global cmd;
    if (args.coverage):
        cmd += " -DGCOV_SUPPORT=On"

    if (args.configuration == "Debug" or args.configuration == "All"):
        linux_debug()

    if (args.configuration == "Release" or args.configuration == "All"):
        linux_release()

    return

#==============================================================================
# Name : main
#==============================================================================

update_flip()

root_build_dir = os.path.join(project_dir, "Build")

create_dir(root_build_dir)

if platform.system()=="Darwin":
    configure_mac(parse_args());
elif platform.system() == "Windows":
	configure_windows(parse_args())
elif platform.system() == "Linux":
	configure_linux(parse_args())
