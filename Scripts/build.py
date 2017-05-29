#!/usr/bin/env python

##### IMPORT #################################################################

import os
import subprocess
import sys
import argparse
import platform

##### VERSION #################################################################

if sys.version_info[0] != 2 or sys.version_info[1] < 7:
    print("This script requires version python 2, 2.7 or higher")
    sys.exit(1)

project_dir = os.path.abspath(".");

#==============================================================================
# Name : parse_args
#==============================================================================

def parse_args ():
    arg_parser = argparse.ArgumentParser ()

    arg_parser.add_argument('-c', '--configuration', default = 'Release', choices = ['Debug', 'Release'],
                            help="the configuration that you wish to build either Release or Debug")

    arg_parser.add_argument('-t', '--target', default = 'alltargets',
                            help="specify the target to build. By default all targets are built. ex: build.py -t Tests")

    if platform.system() == "Windows":
        arg_parser.add_argument('-p','--platform', default = 'x64', choices = ['x64', 'Win32'],
                                help="the target platform")

    return arg_parser.parse_args (sys.argv[1:])

#==============================================================================
# Name : build_osx
#==============================================================================

def build_osx(args):

    if (args.configuration == "Debug"):
        target_xcode = os.path.join(project_dir, "Build", "Debug", "Kiwi.xcodeproj")
    elif(args.configuration == "Release"):
        target_xcode = os.path.join(project_dir, "Build", "Release", "Kiwi.xcodeproj")

    cmd =  "xcodebuild "
    cmd += "-project %s " % target_xcode
    cmd += "-configuration %s " % args.configuration

    if(args.target != "alltargets"):
        cmd += "-target %s " % args.target

    return subprocess.check_call(cmd, shell = True)

#==============================================================================
# Name : build_windows
#==============================================================================

def build_windows(args):

    cmd = "MSBuild.exe "

    if(args.target != "alltargets"):
        target = "%s.vcxproj" % args.target
    else:
        target = "Kiwi.sln"

    cmd += "/verbosity:minimal "

    if(args.configuration == "Release"):
        if(args.platform == "x64"):
            cmd += "/p:Configuration=Release /p:Platform=x64 " + os.path.join(project_dir, "Build", "Release", "x64", target);
        elif(args.platform == "Win32"):
            cmd += "/p:Configuration=Release /p:Platform=Win32 " + os.path.join(project_dir, "Build", "Release", "Win32", target);
    elif(args.configuration == "Debug"):
        if(args.platform == "x64"):
            cmd += "/p:Configuration=Debug /p:Platform=x64 " + os.path.join(project_dir, "Build", "Debug", "x64", target);
        elif(args.platform == "Win32"):
            cmd += "/p:Configuration=Debug /p:Platform=Win32 " + os.path.join(project_dir, "Build", "Debug", "Win32", target);


    subprocess.check_call(cmd, shell = True)

#==============================================================================
# Name : build_linux
#==============================================================================

def build_linux(args):

    cmd = "make "

    if(args.target != "alltargets"):
        cmd += "%s" % args.target

    if(args.configuration == "Release"):
        os.chdir(os.path.join(project_dir, "Build", "Release"))
        subprocess.check_call(cmd, shell = True)
    elif(args.configuration == "Debug"):
        os.chdir(os.path.join(project_dir, "Build", "Debug"))
        subprocess.check_call(cmd, shell = True)
os.chdir(project_dir)

#==============================================================================
# Name : main
#==============================================================================

if platform.system() == "Darwin":
    build_osx(parse_args())
elif platform.system() == "Windows":
	build_windows(parse_args());
elif platform.system() == "Linux":
	build_linux(parse_args());
