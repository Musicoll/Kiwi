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

##### VERSION #################################################################

if sys.version_info[0] != 2 or sys.version_info[1] < 7:
    print("This script requires version python 2, 2.7 or higher")
    sys.exit(1)

root_dir = os.path.split(os.path.dirname(os.path.realpath(__file__)))[0]
flip_dir = os.path.join(root_dir, "ThirdParty", "flip")
flip_commit = "c47e41da05"
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

def read_commit():

    commit = ""

    config_file = os.path.join(flip_dir, "config")

    if  os.path.exists(config_file):
        fo = open(config_file, "r")
        commit = fo.readline()
        fo.close()

    return commit[0:10]

#==============================================================================
# Name : set_public_commit
#==============================================================================

def write_commit(commit):

    config_file = os.path.join(flip_dir, "config")
    fo = open(config_file, "w")
    fo.write(commit)
    fo.close()


#==============================================================================
# Name : download_flip_mac
#==============================================================================

def download_flip_mac(commit):

    archive_file = "flip-demo-macos-" + commit + ".tar.gz"

    # download archive
    print "-- downloading archive"
    urllib.urlretrieve (flip_url + archive_file, os.path.join(root_dir, "ThirdParty", archive_file))

    # ectract archive files
    print "-- extracting archive"
    archive = tarfile.open(os.path.join(root_dir, "ThirdParty", archive_file), "r:gz")
    archive.extractall(os.path.join(root_dir, "ThirdParty"))
    archive.close()

    os.rename(os.path.join(root_dir, "ThirdParty", "flip-demo"), flip_dir)

    # remove archive
    os.remove(os.path.join(root_dir, "ThirdParty", archive_file))

#==============================================================================
# Name : download_flip_windows
#==============================================================================

def download_flip_windows(commit):

    archive_file = "flip-demo-windows-" + commit + ".zip"

    # download archive
    print "-- downloading flip archive"
    urllib.urlretrieve (flip_url + archive_file, os.path.join(root_dir, "ThirdParty", archive_file))

    # extract archive
    print "-- extracting archive"

    os.mkdir(flip_dir)
    archive = zipfile.ZipFile(os.path.join(root_dir, "ThirdParty", archive_file), 'r')
    archive.extractall(flip_dir)
    archive.close()

    os.remove(os.path.join(root_dir, "ThirdParty", archive_file))

#==============================================================================
# Name : download_flip_linux
#==============================================================================

def download_flip_linux(commit):

    archive_file = "flip-demo-linux-" + commit + ".tar.gz"

    # download archive
    print "-- downloading archive"
    urllib.urlretrieve (flip_url + archive_file, os.path.join(root_dir, "ThirdParty", archive_file))

    print "-- extracting archive"

    archive = tarfile.open(os.path.join(root_dir, "ThirdParty", archive_file), "r:gz")
    archive.extractall(os.path.join(root_dir, "ThirdParty"))
    archive.close()

    os.rename(os.path.join(root_dir, "ThirdParty", "flip-demo"), flip_dir)

    # remove archive
    os.remove(os.path.join(root_dir, "ThirdParty", archive_file))

#==============================================================================
# Name : checkout
#==============================================================================

def download_flip(commit):

    if (platform.system() == "Darwin"):
        download_flip_mac(commit)
    elif (platform.system() == "Windows"):
    	download_flip_windows(commit)
    elif platform.system() == "Linux":
    	download_flip_linux(commit)


#==============================================================================
# Name : main
#==============================================================================

current_commit = read_commit()

if current_commit != flip_commit:

    print "-- flip is not up-to-date :"

    if os.path.exists(flip_dir):
        shutil.rmtree(flip_dir)

    download_flip(flip_commit)

    write_commit(flip_commit)

    print "-- flip successfully updated"

else:

    print "-- flip already up to date"
