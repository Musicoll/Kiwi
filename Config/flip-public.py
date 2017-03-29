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
import platform


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
        fo = open(os.path.join(os.getcwd(), "flip-public", "config"), "r")
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
# Name : copy include and documentation
#==============================================================================

def copy_includes_and_doc(archive_dir):

    print "-- copying includes and documentation"

    # copy include, lib and documentation into flip-demo
    shutil.copytree (os.path.join(archive_dir, "include"),
                     os.path.join(os.getcwd(), "flip-public", "include"))

    shutil.copytree (os.path.join(archive_dir, "documentation"),
                     os.path.join(os.getcwd(), "flip-public", "documentation"))


#==============================================================================
# Name : checkout_mac
#==============================================================================

def checkout_mac(commit):

    # download archive
    file = "flip-demo-macos-" + commit + ".tar.gz"

    print "-- downloading: " + flip_url + file + " file"
    urllib.urlretrieve (flip_url + file, file)

    # decompress archive
    archive_dir = os.path.join(os.getcwd(), "flip-demo-macos")
    os.mkdir(archive_dir)

    print "-- extracting archive in: " + archive_dir
    archive = tarfile.open(file, "r:gz")
    archive.extractall(archive_dir)
    archive.close()

    # copy include, lib and documentation into flip-demo
    copy_includes_and_doc(os.path.join(archive_dir, "flip-demo"))

    print "-- copying libs"
    os.makedirs(os.path.join(os.getcwd(), "flip-public", "lib", "mac"))

    shutil.copyfile (os.path.join(archive_dir, "flip-demo", "lib", "libflip.a"),
                     os.path.join(os.getcwd(), "flip-public", "lib", "mac", "libflip.a"))

    # remove archive
    os.remove(os.path.join(os.getcwd(), file))
    shutil.rmtree(archive_dir)

#==============================================================================
# Name : checkout_windows
#==============================================================================

def checkout_windows(commit):

    # download archive
    file = "flip-demo-windows-" + commit + ".zip"

    print "-- downloading: " + flip_url + file + " file"
    urllib.urlretrieve (flip_url + file, file)

    # decompress archive
    archive_dir = os.path.join(os.getcwd(), "flip-demo-windows")
    os.mkdir(archive_dir)

    print "-- extracting archive in: " + archive_dir

    archive = zipfile.ZipFile(file, 'r')
    archive.extractall(archive_dir)
    archive.close()

    # copy include, lib and documentation into flip-demo
    copy_includes_and_doc(archive_dir)

    # copy libraries into flip-demo
    print "-- copying libs"
    os.makedirs(os.path.join(os.getcwd(), "flip-public", "lib", "windows"))

    shutil.copytree (os.path.join(archive_dir, "lib", "VS2015"),
                     os.path.join(os.getcwd(), "flip-public", "lib", "windows", "VS2015"))

    # remove archive && directory
    shutil.rmtree(archive_dir)
    os.remove(file)

#==============================================================================
# Name : checkout_linux
#==============================================================================

def checkout_linux(commit):

    # download archive
    file = "flip-demo-linux-" + commit + ".tar.gz"

    print "-- downloading: " + flip_url + file + " file"
    urllib.urlretrieve (flip_url + file, file)

    # decompress archive
    archive_dir = os.path.join(os.getcwd(), "flip-demo-linux")
    os.mkdir(archive_dir)

    print "-- extracting archive in: " + archive_dir

    archive = tarfile.open(file, "r:gz")
    archive.extractall(archive_dir)
    archive.close()

    # copy include, lib and documentation into flip-demo
    copy_includes_and_doc(os.path.join(archive_dir, "flip-demo"))

    print "-- copying libs"
    os.makedirs(os.path.join(os.getcwd(), "flip-public", "lib", "linux"))

    shutil.copytree (os.path.join(archive_dir, "flip-demo", "lib", "gcc"),
                     os.path.join(os.getcwd(), "flip-public", "lib", "linux", "gcc"))

    # remove archive
    os.remove(os.path.join(os.getcwd(), file))
    shutil.rmtree(archive_dir)

#==============================================================================
# Name : checkout
#==============================================================================

def checkout(commit):

    if (platform.system() == "Darwin"):
        checkout_mac(commit)
    elif (platform.system() == "Windows"):
    	checkout_windows(commit)
    elif platform.system() == "Linux":
    	checkout_linux(commit)


#==============================================================================
# Name : main
#==============================================================================

os.chdir(os.path.join(project_dir, "ThirdParty"))

private_commit = get_private_commit()
public_commit = get_public_commit()

if private_commit != public_commit:

    print "-- flip-public not up-to-date with flip :"

    if os.path.exists(os.path.join(os.getcwd(), "flip-public")):
        shutil.rmtree(os.path.join(os.getcwd(), "flip-public"))

    os.mkdir("flip-public")

    checkout(private_commit)

    set_public_commit(private_commit)

# flip flip-public version should be up-to-date with flip
print "-- flip-public version up-to-date with flip"
