#!/usr/bin/env python

##### IMPORT #################################################################

import os
import sys
import argparse
import subprocess
import platform
import urllib2
import ssl
import zipfile
import tarfile
import shutil

##### VERSION #################################################################

if sys.version_info[0] != 2 or sys.version_info[1] < 7:
    print("This script requires version python 2, 2.7 or higher")
    sys.exit(1)

root_dir = os.path.split(os.path.dirname(os.path.realpath(__file__)))[0]
boost_dir = os.path.join(root_dir, "ThirdParty", "boost")
boost_version = "1.63.0"
boost_url = "https://sourceforge.net/projects/boost/files/boost/"

#==============================================================================
# Name : get_public_commit
#==============================================================================

def read_version():

    version = ""

    config_file = os.path.join(boost_dir, "config")

    if  os.path.exists(config_file):
        fo = open(config_file, "r")
        version = fo.readline()
        fo.close()

    return version[0:10]

#==============================================================================
# Name : set_public_commit
#==============================================================================

def write_version(version):

    config_file = os.path.join(boost_dir, "config")
    fo = open(config_file, "w")
    fo.write(version)
    fo.close()


#==============================================================================
# Name : init_boost_macos
#==============================================================================

def init_boost_macos():

    subprocess.check_call("./bootstrap.sh toolset=clang macosx-version-min=10.8 architecture=combined link=static", shell= True)

    subprocess.check_call("./b2 address-model=32_64 --with-system stage", shell= True)

#==============================================================================
# Name : download_flip_windows
#==============================================================================

def init_boost_windows():

    subprocess.check_call("bootstrap.bat", shell= True)

    subprocess.check_call("b2 --toolset=msvc-14.0 -j4 --with-system --stagedir=stage64 variant=release architecture=x86 address-model=64 link=static", shell= True)

    subprocess.check_call("b2 --toolset=msvc-14.0 -j4 --with-system --stagedir=stage32 variant=release architecture=x86 address-model=32 link=static", shell= True)

#==============================================================================
# Name : download_flip_linux
#==============================================================================

def init_boost_linux():

    subprocess.check_call("./bootstrap.sh toolset=gcc link=static", shell= True);

    subprocess.check_call("./b2 --with-system stage", shell= True);

#==============================================================================
# Name : init_boost
#==============================================================================

def init_boost():

    os.chdir(os.path.join(root_dir, "ThirdParty", "boost"));

    if (platform.system() == "Darwin"):
        init_boost_macos()
    elif (platform.system() == "Windows"):
    	init_boost_windows()
    elif platform.system() == "Linux":
    	init_boost_linux()

    os.chdir(root_dir);

#==============================================================================
# Name : download_boost_unix
#==============================================================================

def download_boost_unix():

    boost_archive_file = "boost_" + boost_version.replace(".", "_") + ".tar.gz"

    # download archive
    print "-- downloading archive"

    endpoint = boost_url + boost_version + "/" + boost_archive_file

    f = open(os.path.join(root_dir, "ThirdParty", boost_archive_file), 'wb')
    f.write(urllib2.urlopen(endpoint).read())
    f.close()

    print "-- extracting archive"

    archive = tarfile.open(os.path.join(root_dir, "ThirdParty", boost_archive_file), "r:gz")
    archive.extractall(os.path.join(root_dir, "ThirdParty"))
    archive.close()

    os.rename(os.path.join(root_dir, "ThirdParty", "boost_" + boost_version.replace(".", "_")), boost_dir)

    # remove archive
    os.remove(os.path.join(root_dir, "ThirdParty", boost_archive_file))

#==============================================================================
# Name : download_boost_windows
#==============================================================================

def download_boost_windows():

    boost_archive_file = "boost_" + boost_version.replace(".", "_") + ".zip"

    # download archive
    print "-- downloading archive"

    endpoint = boost_url + boost_version + "/" + boost_archive_file

    ctx = ssl.create_default_context()
    ctx.check_hostname = False
    ctx.verify_mode = ssl.CERT_NONE

    f = open(os.path.join(root_dir, "ThirdParty", boost_archive_file), 'wb')
    f.write(urllib2.urlopen(boost_url + boost_version + "/" + boost_archive_file, context=ctx).read())
    f.close()

    print "-- extracting archive"

    archive = zipfile.ZipFile(os.path.join(root_dir, "ThirdParty", boost_archive_file), "r")
    archive.extractall(os.path.join(root_dir, "ThirdParty"))
    archive.close()

    os.rename(os.path.join(root_dir, "ThirdParty", "boost_" + boost_version.replace(".", "_")), boost_dir)

    # remove archive
    os.remove(os.path.join(root_dir, "ThirdParty", boost_archive_file))

#==============================================================================
# Name : download_boost
#==============================================================================

def download_boost():

    if (platform.system() == "Darwin"):
        download_boost_unix()
    elif (platform.system() == "Windows"):
    	download_boost_windows()
    elif platform.system() == "Linux":
    	download_boost_unix()

#==============================================================================
# Name : main
#==============================================================================

current_version = read_version()

if current_version != boost_version:

    print "-- boost is not up-to-date :"

    if os.path.exists(boost_dir):
        shutil.rmtree(boost_dir)

    download_boost()

    init_boost()

    write_version(boost_version)

    print "-- boost successfully updated"

else:

    print "-- boost already up to date"
