#!/usr/bin/env python

##### IMPORT #################################################################

import os
import subprocess
import sys
import argparse
import platform

project_dir = os.path.abspath(".");

#==============================================================================
# Name : parse_args
#==============================================================================

def parse_args ():
    arg_parser = argparse.ArgumentParser ()
    
    arg_parser.add_argument('-c', '--configuration', default = 'Release', choices = ['Debug', 'Release'])

    if platform.system() == "Windows":
        arg_parser.add_argument('-p','--platform', default = 'x64', choices = ['x64', 'Win32'])

    return arg_parser.parse_args (sys.argv[1:])

#==============================================================================
# Name : build_osx
#==============================================================================

def build_osx(args):

    if (args.configuration == "Debug"):
        target_xcode = os.path.join(project_dir, "Build", "Debug", "Kiwi.xcodeproj")
    elif(args.configuration == "Release"):
        target_xcode = os.path.join(project_dir, "Build", "Release", "Kiwi.xcodeproj")

    cmd =  "xcodebuild"
    cmd += " -project " + target_xcode
    cmd += " -configuration %s " % args.configuration
    cmd += ' -alltargets '

    return subprocess.check_call(cmd, shell = True)

#==============================================================================
# Name : build_windows
#==============================================================================

def build_windows(args):
    if(args.configuration == "Release"):
        if(args.platform == "x64"):
            cmd = "MSBuild.exe " + "/p:Configuration=Release /p:Platform=x64 " + os.path.join(project_dir, "Build", "Release", "x64", "Kiwi.sln");
        elif(args.platform == "Win32"):
            cmd = "MSBuild.exe " + "/p:Configuration=Release /p:Platform=Win32 " + os.path.join(project_dir, "Build", "Release", "Win32", "Kiwi.sln");
    elif(args.configuration == "Release"):
        if(args.platform == "x64"):
            cmd = "MSBuild.exe " + "/p:Configuration=Debug /p:Platform=x64 " + os.path.join(project_dir, "Build", "Debug", "x64", "Kiwi.sln");
        elif(args.platform == "Win32"):
            cmd = "MSBuild.exe " + "/p:Configuration=Debug /p:Platform=Win32 " + os.path.join(project_dir, "Build", "Debug", "Win32", "Kiwi.sln");

	subprocess.check_call(cmd, shell = True)

#==============================================================================
# Name : main
#==============================================================================

if platform.system() == "Darwin":
    build_osx(parse_args())
elif platform.system() == "Windows":
	build_windows(parse_args());
