## Kiwi

[![Build Status](https://travis-ci.org/Musicoll/Kiwi.svg?branch=master)](https://travis-ci.org/Musicoll/Kiwi)
[![Build status](https://ci.appveyor.com/api/projects/status/github/Musicoll/Kiwi?branch=master&svg=true)](https://ci.appveyor.com/project/CICM/kiwi)
[![Coverage Status](https://coveralls.io/repos/github/Musicoll/Kiwi/badge.svg?branch=master)](https://coveralls.io/github/Musicoll/Kiwi?branch=master)

[![Documentation](https://img.shields.io/badge/docs-doxygen-blue.svg)](http://musicoll.github.io/Kiwi/)  
[![Release](https://img.shields.io/github/release/Musicoll/Kiwi.svg)](https://github.com/Musicoll/Kiwi/releases)  
[![license](https://img.shields.io/github/license/Musicoll/Kiwi.svg?maxAge=2592000)](https://github.com/Musicoll/Kiwi/blob/master/LICENSE)  
[![Website](https://img.shields.io/website/http/shields.io.svg?maxAge=2592000)](http://musicoll.mshparisnord.org)


### Cloning the project
```shell
$ git clone --recursive https://github.com/Musicoll/Kiwi.git
```

### Install dependencies

Kiwi scripts and build system uses python (version 2.7 or higher) and cmake (version 2.8.7 or higher).

#### Mac

Mac build uses xcode projects. AppleClang tested version is 7.3.

Install libcurl 7.52.1 without ssl support. Kiwi build system requires curl libraries being installed on mac. You can install libcurl in a desired location and refere to it using environment variables. We recommend using our precompiled <a href="https://github.com/Musicoll/Kiwi/issues/73">libcurl static library</a> attached to our github account.

```shell
# Download precompiled curl 7.52.1 from and extract the archive to desired location [curl-dir].
$ export KIWI_CURL_LIB=[curl-dir]/lib
$ export KIWI_CURL_INCLUDE=[curl-dir]/include

# To permanently set environment variable edit ~/.bash_profile file.
$ echo export KIWI_CURL_LIB=[curl-dir]/lib >> ~/.bash_profile
$ echo export KIWI_CURL_INCLUDE=[curl-dir]/include >>~/.bash_profile
```

Kiwi also requires collaborative library flip. Same process is used to retrieve flip precompiled libraries and point to them using environment variables. To download current flip-public version for mac please follow this<a href=http://developer.irisate.com.s3-website-us-east-1.amazonaws.com/files/flip-demo-macos-c47e41da05.tar.gz> link </a>.

```shell
# Download precomiled libraries from iristate website.
# Extract the archive to desired location [flip-dir].
$ export KIWI_FLIP_LIB=[flip-dir]/lib
$ export KIWI_FLIP_INCLUDE=[flip-dir]/include

# To permanently set environment variables, edit ~/.bash_profile file.
$ echo export KIWI_FLIP_LIB=[flip-dir]/lib >> ~/.brash_profile
$ echo export KIWI_FLIP_INCLUDE=[flip-dir]/include >>~/.bash_profile
```

#### Linux

Build system uses gcc 4.9 or higher (Tested version 4.9).

```shell
$ sudo apt-get install g++-4.9
```

Install libcurl 7.52.1 without ssl support. Kiwi build system requires curl libraries being installed. You can install libcurl in a desired location and refere to it using environment variables. We recommend using our precompiled <a href="https://github.com/Musicoll/Kiwi/issues/73">libcurl static library</a> attached to our github account.

```shell
# Download precompile curl 7.52.1 from github and extract the archive to desired location [curl-dir].
$ export KIWI_CURL_LIB=[curl-dir]/lib
$ export KIWI_CURL_INCLUDE=[curl-dir]/include

# To permanently set environment variable edit ~/.bashrc file.
$ echo export KIWI_CURL_LIB=[curl-dir]/lib >> ~/.bashrc
$ echo export KIWI_CURL_INCLUDE=[curl-dir]/include >>~/.bashrc
```

Kiwi also requires collborative library flip. Same process is used to retrieve flip precompiled libraries and points to them using environment variables. To download current flip-public version for linux please follow this<a href=http://developer.irisate.com.s3-website-us-east-1.amazonaws.com/files/flip-demo-linux-c47e41da05.tar.gz> link </a>.

```shell
# Download precomiled libraries from irisate website.
# Extract the archive to desire location [flip-dir].
$ export KIWI_FLIP_LIB=[flip-dir]/lib
$ export KIWI_FLIP_INCLUDE=[flip-dir]/include

# To permanently set environment variables edit ~/.bashrc file.
$ echo export KIWI_FLIP_LIB=[flip-dir]/lib/gcc >> ~/.bashrc
$ echo export KIWI_FLIP_INCLUDE=[flip-dir]/include >> ~/.bashrc >>~/.bash_profile
```

Linux build also relies on other external libraries being installed.

```shell
sudo add-apt-repository ppa:webkit-team/ppa -y
sudo add-apt-repository ppa:ubuntu-toolchain-r-test -y
sudo apt-get update
sudo apt-get install libfreetype6-dev
sudo apt-get install libx11-dev
sudo apt-get install libxrandr-dev
sudo apt-get install libxinerama1
sudo apt-get install libxinerama-dev
sudo apt-get install libxcursor-dev
sudo apt-get install libasound2-dev
sudo apt-get install libgtk-3-dev
sudo apt-get install libwebkit2gtk-4.0-dev
```

#### Windows

Windows build uses Visual Studio 2015 as a project manager. Tested MSVC version is 19.0.

Windows will require the windows sdk being installed. Please go to <a href="https://www.microsoft.com/en-us/download/details.aspx?id=8279">
microsoft download page</a> to retrieve the sdk. Tested version on CI is v7.1A.

Install libcurl 7.52.1 without ssl support. Kiwi build system requires curl libraries being installed. You can install libcurl in a desired location and refere to it using environment variables. We recommend using our precompiled <a href="https://github.com/Musicoll/Kiwi/issues/73">curl static library</a> attached to our github account.

```shell
# Download precompiled curl 7.52.1 and extract the archives to desired locations [curl-dir-win32] and [curl-dir-x64].
# Set environment variable to set variable in local scope or use system configuration. Path to 64bit version and
# Win32 version are required.
$ set KIWI_CURL_LIB_WIN32=[curl-dir-win32]/lib/Release
$ set KIWI_CURL_INCLUDE_WIN32=[curl-dir-win32]/include
$ set KIWI_CURL_LIB_x64=[curl-dir-x64]/lib/Release
$ set KIWI_CURL_INCLUDE_x64=[curl-dir-x64]/include
```

Install flip. Kiwi build system requires flip libraries being installed. You can install flip in a desired location and refere to it using environment variables. We recommend using precompiled flip precompiled libraries. To download current flip-public version for windows please follow this<a href=http://developer.irisate.com.s3-website-us-east-1.amazonaws.com/files/flip-demo-windows-c47e41da05.zip> link </a>.

```shell
# Download precomiled libraries from irisate website.
# Exctract the archives to desired locations [flip-dir-win32] and[flip-dir-x64] and set variable before compiling.
$ set KIWI_FLIP_LIB_WIN32=[flip-dir-win32]/lib/VS2015/Win32/Release
$ set KIWI_FLIP_INCLUDE_WIN32=[curl-dir-win32]/include
$ set KIWI_FLIP_LIB_x64=[curl-dir-x64]/lib/Release/lib/VS2015/x64/Release
$ set KIWI_FLIP_INCLUDE_x64=[curl-dir-x64]/include
```

### Compilation

Before compiling Kiwi, configuration is needed using a python script. It uses cmake generator
to generate IDE project files. Windows uses visual studio generator, Mac uses xcode generator and
linux configure generates a makefile.

```shell
# Configure all platforms (for windows only) and configurations
$ python ./Scripts/configure.py
$ python ./Scripts/configure.py --help
usage: configure.py [-h] [-c {Debug,Release,All}]
optional arguments:
  -h, --help              show this help message and exit
  -c {Debug,Release,All}, --configuration {Debug,Release,All}
                          the chosen configuration to initialize either Release
                          or Debug
```

Once configured you can either open project in IDE or directly build the application using script build.py.
You can restrict the targets being built using -t argument, platform (windows only) using -p argument and build configuration
using -c argument.

```shell
$ python ./Scripts/build.py -c Release
$ python
usage: build.py [-h] [-c {Debug,Release}] [-t TARGET]
optional arguments:
  -h, --help            show this help message and exit
  -c {Debug,Release}, --configuration {Debug,Release}
                        the configuration that you wish to build either
                        Release or Debug
  -t TARGET, --target TARGET
                        specify the target to build. By default all targets
                        are built. ex: build.py -t Tests
```

Tests are executed as a post build command of aggregated target Tests. To execute tests one can
build Tests target using build.py or use IDE to build Tests.

```shell
$ python ./Scripts/build.py -c Release -t Tests
```

### Other scripts

The documentation can be generated using doxygen. It is generated in directory /docs in html format.

```shell
# Execute doxygen from root directory.
$ Doxygen ./Doc/Doxyfile
```

You can generate binary data for ressources with the `Scripts/ressource.py` script.
This script takes 2 optional arguments:
 - `-i / --input` : The ressources input directory ("./Ressources/BinaryRes" by default).
 - `-o / --output` : The output directory in which the binary data files will be generated ("./Client/Source/KiwiApp_Ressources" by default).

 ex:

 ```shell
 $ cd {kiwi_directory_path}
 # with default paths:
 $ python ./Scripts/ressource.py
 # or with custom paths
 $ python ./Scripts/ressource.py -i input/ressources/path -o output/binary/path
 ```

### Run the Application

The Kiwi client application needs running API and document session servers to work.

#### Run the Kiwi API server

To run the Node.js server, follow the instructions in the readme.md file of the [kiwi-node-server](https://github.com/Musicoll/kiwi-node-server) repository

#### Run the document session server

First create a `.json` config file somewhere (ie a `dev.json` file in a `/config` directory next to the server binary).
The `.json` file must contain:
- A `port` entry to specify the session port to use to serve documents.
- A `backend_directory` entry to specify directory in which documents are saved.

ex:
```json
{
  "port": 9090,
  "backend_directory": "./server_backend"
}
```

You can then launch the server by typing:

```shell
$ ./Server -f ./config/dev.json
```

#### Launch Kiwi client App

- Launch the kiwi app then change open the preference panel and setup the network config.
- You should see a list of documents in the Document Browser window.
- Click on a document to open it.

### Credits:

 - Some of the application icons are *designed by Freepik and distributed by Flaticon*.
