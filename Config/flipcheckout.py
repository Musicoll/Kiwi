#!/usr/bin/env python

##### IMPORT #################################################################

import os
import sys
import argparse
import urllib
import tarfile
import zipfile
import shutil
import subprocess


project_dir = os.path.abspath(".");
flip_url = "http://developer.irisate.com.s3-website-us-east-1.amazonaws.com/files/"

#==============================================================================
# Name : parse_args
#==============================================================================

def parse_args ():
    arg_parser = argparse.ArgumentParser ()
    arg_parser.add_argument('-c', '--commit')

    return arg_parser.parse_args (sys.argv[1:])

#==============================================================================
# Name : checkout_flip_submodule
#==============================================================================

def checkout_fip_submodule(args):
    subprocess.check_call("git submodule update --init flip", shell= True)
    
    os.chdir(os.path.join(os.getcwd(), "flip"))
    subprocess.check_call("git fetch", shell= True)
    subprocess.check_call("git checkout master ", shell= True)
    subprocess.check_call("git checkout " + args.commit , shell= True)
    os.chdir("../")
    

#==============================================================================
# Name : checkout_mac
#==============================================================================

def checkout_mac(args):
    
    # downaload archive
    file = "flip-demo-macos-" + args.commit + ".tar.gz"
    urllib.urlretrieve (flip_url + file, file)
    
    # remove previous flip-demo
    if  os.path.exists(os.path.join(os.getcwd(), "flip-demo")):
        shutil.rmtree(os.path.join(os.getcwd(), "flip-demo"));
    
    # decompress archive
    archive = tarfile.open(file, "r:gz")
    archive.extractall()
    archive.close
    
    # remove unecessary directories
    shutil.rmtree(os.path.join(os.getcwd(), "flip-demo", "project"));
    shutil.rmtree(os.path.join(os.getcwd(), "flip-demo", "samples"));
    shutil.rmtree(os.path.join(os.getcwd(), "flip-demo", "test"));
    shutil.rmtree(os.path.join(os.getcwd(), "flip-demo", "build"));
    
    # create mac lib dir
    os.mkdir(os.path.join(os.getcwd(), "flip-demo", "lib", "mac"))
    shutil.move(os.path.join(os.getcwd(), "flip-demo", "lib", "libflip.a"),
                os.path.join(os.getcwd(), "flip-demo", "lib", "mac", "libflip.a"))
    
    # remove archive
    os.remove(os.path.join(os.getcwd(), file))

#==============================================================================
# Name : checkout_windows
#==============================================================================

def checkout_windows(args):
    
    # downaload archive
    file = "flip-demo-windows-" + args.commit + ".zip"
    urllib.urlretrieve (flip_url + file, file)
    
    # decompress archive
    os.mkdir("flip-demo-windows")
    windows_dir = os.path.join(os.getcwd(), "flip-demo-windows")
    
    archive = zipfile.ZipFile(file, 'r')
    archive.extractall(windows_dir)
    archive.close
    
    # Copy libraries into flip-demo
    lib_dir = os.path.join(os.getcwd(), "flip-demo", "lib", "windows") 
    shutil.copytree (os.path.join(os.getcwd(), windows_dir, "lib", "VS2015"), os.path.join(lib_dir, "VS2015"))
    
    # remove archive && directory
    shutil.rmtree(windows_dir)
    os.remove(file)

#==============================================================================
# Name : main
#==============================================================================

os.chdir(os.path.join(project_dir, "ThirdParty"))

checkout_mac(parse_args())
checkout_windows(parse_args())
checkout_fip_submodule(parse_args())