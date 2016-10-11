#!/usr/bin/env python

##### IMPORT #################################################################

import os
import sys
import platform
import subprocess
import shutil
import argparse

project_dir = os.path.abspath(".");

cmd = "cmake";

#==============================================================================
# Name : parse_args
#==============================================================================

def parse_args ():
    arg_parser = argparse.ArgumentParser ()
    
    if platform.system () == 'Darwin':
        arg_parser.add_argument('-flip', default = 'public', choices = ['public', 'private'])

    return arg_parser.parse_args (sys.argv[1:])

#==============================================================================
# Name : create_dir
#==============================================================================

def create_dir(dir):
	if not os.path.exists(dir):
	   os.makedirs(dir);
    
#==============================================================================
# Name : flip_private
#==============================================================================

def build_flip_private():
    flip_path = os.path.join(project_dir, "ThirdParty", "flip")
    os.chdir(flip_path);
    configure_path = os.path.join(flip_path, "configure.py")
    build_path = os.path.join(flip_path, "build.py")
    subprocess.check_call("python " + configure_path, shell= True);
    subprocess.check_call("python " + build_path + " -c Debug -t flip", shell= True);
    subprocess.check_call("python " + build_path + " -c Release -t flip", shell= True);

#==============================================================================
# Name : retrieve_flip_public
#==============================================================================

def retrieve_flip_public():
    subprocess.check_call("python " + os.path.join(project_dir, "Config", "flip-public.py"), shell= True);

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
    if (args.flip == "private"):
        build_flip_private()
        cmd += " -DFLIP_PRIVATE=1"
    mac_debug();
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
    win_debug_32();
    win_debug_64();
    win_release_32();
    win_release_64();
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
    linux_debug()
    linux_release()
    return

#==============================================================================
# Name : main
#==============================================================================

retrieve_flip_public();

root_build_dir = os.path.join(project_dir, "Build")

create_dir(root_build_dir)

if platform.system()=="Darwin":
    configure_mac(parse_args());
elif platform.system() == "Windows":
	configure_windows(parse_args())
elif platform.system() == "Linux":
	configure_linux(parse_args())
