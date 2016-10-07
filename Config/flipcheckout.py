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
    
    # download archive
    file = "flip-demo-macos-" + args.commit + ".tar.gz"
    urllib.urlretrieve (flip_url + file, file)
    
    # decompress archive
    mac_dir = os.path.join(os.getcwd(), "flip-demo-mac")
    os.mkdir(mac_dir)
    
    archive = tarfile.open(file, "r:gz")
    archive.extractall(mac_dir)
    archive.close
    
    # Copy include, lib and documentation into flip-demo
    shutil.copytree (os.path.join(mac_dir, "flip-demo", "include"), os.path.join(os.getcwd(), "flip-demo", "include"))
    shutil.copytree (os.path.join(mac_dir, "flip-demo", "documentation"), os.path.join(os.getcwd(), "flip-demo", "documentation"))
    os.makedirs(os.path.join(os.getcwd(), "flip-demo", "lib", "mac"))
    shutil.copyfile (os.path.join(mac_dir, "flip-demo", "lib", "libflip.a"), os.path.join(os.getcwd(), "flip-demo", "lib", "mac", "libflip.a"))
    
    # remove archive
    os.remove(os.path.join(os.getcwd(), file))
    shutil.rmtree(mac_dir)

#==============================================================================
# Name : checkout_windows
#==============================================================================

def checkout_windows(args):
    
    # download archive
    file = "flip-demo-windows-" + args.commit + ".zip"
    urllib.urlretrieve (flip_url + file, file)
    
    # decompress archive
    windows_dir = os.path.join(os.getcwd(), "flip-demo-windows")
    os.mkdir(windows_dir)
    
    archive = zipfile.ZipFile(file, 'r')
    archive.extractall(windows_dir)
    archive.close
    
    # Copy libraries into flip-demo
    os.makedirs(os.path.join(os.getcwd(), "flip-demo", "lib", "windows"))
    shutil.copytree (os.path.join(windows_dir, "lib", "VS2015"), os.path.join(os.getcwd(), "flip-demo", "lib", "windows", "VS2015"))
    
    # remove archive && directory
    shutil.rmtree(windows_dir)
    os.remove(file)

#==============================================================================
# Name : main
#==============================================================================

os.chdir(os.path.join(project_dir, "ThirdParty"))

if  os.path.exists(os.path.join(os.getcwd(), "flip-demo")):
        shutil.rmtree(os.path.join(os.getcwd(), "flip-demo"));
        os.mkdir("flip-demo")

checkout_mac(parse_args())
checkout_windows(parse_args())
checkout_fip_submodule(parse_args())