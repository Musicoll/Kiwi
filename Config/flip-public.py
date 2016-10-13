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
# Name : get_private_commit
#==============================================================================

def get_private_commit():
    commit  = subprocess.check_output(["git", "submodule", "status", "flip"])
    return commit[1:11]

#==============================================================================
# Name : get_public_commit
#==============================================================================

def get_public_commit():
    
    commit = ""
    
    if  os.path.exists(os.path.join(os.getcwd(), "flip-public", "config")):
        fo = open(os.path.join(os.getcwd(), "flip-public", "config"), "rw+")
        commit = fo.readline()
        fo.close()   

    return commit[0:10]

#==============================================================================
# Name : set_public_commit
#==============================================================================

def set_public_commit(commit):
    
    fo = open(os.path.join(os.getcwd(), "flip-public", "config"), "w")
    commit = fo.write(commit)
    fo.close()

#==============================================================================
# Name : checkout_mac
#==============================================================================

def checkout_mac(commit):
    
    # download archive
    file = "flip-demo-macos-" + commit + ".tar.gz"
    urllib.urlretrieve (flip_url + file, file)
    
    # decompress archive
    mac_dir = os.path.join(os.getcwd(), "flip-demo-mac")
    os.mkdir(mac_dir)
    
    archive = tarfile.open(file, "r:gz")
    archive.extractall(mac_dir)
    archive.close()
    
    # copy include, lib and documentation into flip-demo
    shutil.copytree (os.path.join(mac_dir, "flip-demo", "include"), 
                     os.path.join(os.getcwd(), "flip-public", "include"))
    
    shutil.copytree (os.path.join(mac_dir, "flip-demo", "documentation"), 
                     os.path.join(os.getcwd(), "flip-public", "documentation"))
    
    os.makedirs(os.path.join(os.getcwd(), "flip-public", "lib", "mac"))
    
    shutil.copyfile (os.path.join(mac_dir, "flip-demo", "lib", "libflip.a"), 
                     os.path.join(os.getcwd(), "flip-public", "lib", "mac", "libflip.a"))
    
    # remove archive
    os.remove(os.path.join(os.getcwd(), file))
    shutil.rmtree(mac_dir)

#==============================================================================
# Name : checkout_windows
#==============================================================================

def checkout_windows(commit):
    
    # download archive
    file = "flip-demo-windows-" + commit + ".zip"
    urllib.urlretrieve (flip_url + file, file)
    
    # decompress archive
    windows_dir = os.path.join(os.getcwd(), "flip-demo-windows")
    os.mkdir(windows_dir)
    
    archive = zipfile.ZipFile(file, 'r')
    archive.extractall(windows_dir)
    archive.close()
    
    # copy libraries into flip-demo
    os.makedirs(os.path.join(os.getcwd(), "flip-public", "lib", "windows"))
    
    shutil.copytree (os.path.join(windows_dir, "lib", "VS2015"), 
                     os.path.join(os.getcwd(), "flip-public", "lib", "windows", "VS2015"))
    
    # remove archive && directory
    shutil.rmtree(windows_dir)
    os.remove(file)
    
#==============================================================================
# Name : checkout_linux
#==============================================================================

def checkout_linux(commit):
    
    # download archive
    file = "flip-demo-linux-" + commit + ".tar.gz"
    urllib.urlretrieve (flip_url + file, file)
    
    # decompress archive
    linux_dir = os.path.join(os.getcwd(), "flip-demo-linux")
    os.mkdir(linux_dir)
    
    archive = tarfile.open(file, "r:gz")
    archive.extractall(linux_dir)
    archive.close()
    
    # copy include, lib and documentation into flip-demo
    os.makedirs(os.path.join(os.getcwd(), "flip-public", "lib", "linux"))
    
    shutil.copytree (os.path.join(linux_dir, "flip-demo", "lib", "gcc"), 
                     os.path.join(os.getcwd(), "flip-public", "lib", "linux", "gcc"))
    
    # remove archive
    os.remove(os.path.join(os.getcwd(), file))
    shutil.rmtree(linux_dir)

#==============================================================================
# Name : main
#==============================================================================

os.chdir(os.path.join(project_dir, "ThirdParty"))

private_commit = get_private_commit()
public_commit = get_public_commit()

if  private_commit  != public_commit:
    
        if os.path.exists(os.path.join(os.getcwd(), "flip-public")):
            shutil.rmtree(os.path.join(os.getcwd(), "flip-public"))
        os.mkdir("flip-public")
        
        checkout_mac(private_commit)
        checkout_windows(private_commit)
        checkout_linux(private_commit)
        
        set_public_commit(private_commit)