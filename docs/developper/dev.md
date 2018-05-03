# Developper guide

### Table of Contents
1. [Building Kiwi](#build)
2. [Code](#code)
3. [Server](#server)
3. [Scripts](#scripts)

### Building Kiwi<a name="build">

##### Python

Our build system uses python 2.7 to execute scripts. Please download and install python before going any further. python version 2.7.11 is tested.

##### Cloning

Use git clone comment to clone the repository.

```shell
git clone --recursive https://github.com/Musicoll/Kiwi.git
```

##### Configuring

Before building kiwi, you'll need to create a project from source code. Configuring will download certain dependencies (boost/flip) into /ThirdParty and create a /Build containing the project. We create a Visual Studio project on Windows, a Xcode project on MacOs and a makefile on linux. To configure execute the following command.

```shell
python ./Script/configure.py
```

##### Building

To build Kiwi, you can either open your code editor and build the project or use our build script.
Here is a list of tested compilers depending on your platform.

- MacOs: clang 7.3.1
- Windows: msvc 19.0
- Linus: gcc-4.9

```shell
python ./Script/build.py
```

To build a specific target you can use argument -t and you can choose your configuration (Debug/Release) using -c argument.

ex:

```shell
python ./Script/build.py -t Server -c Debug
```

##### Tests

Tests are ran as a post build command of target Tests. To run tests.

```shell
python ./Srcipts/build.py -t Tests -c Release
```


### Code<a name="code"></a>

The documentation of Kiwi's classes can be found [here](../html/index.html).

### Server<a name="server"></a>

The Kiwi client application needs running API and document session servers to work. The API server is used by Kiwi to give informations about document and users. The document session server

#### Api Server

To run the Node.js server, follow the instructions in the readme.md file of the [kiwi-node-server](https://github.com/Musicoll/kiwi-node-server) repository

#### Document session server

First create a `.json` config file somewhere (ie a `dev.json` file in a `/config` directory next to the server binary).

The `.json` file must contain:
- A `session_port` entry to specify the session port to use to serve documents.
- A `backend_directory` entry to specify directory in which documents are saved.
- A `open_token` which is given by the API server and used by the document server to verify the users identity. (Shall be the same for API and document server).
- A `kiwi_version` that is the compatible kiwi version. Shall be the same as the app version of the binary that will communicate with the server.

The api server also uses a json file for its settings. Its easier to use the same json file for both the API server and the document server for some informations are shared between the two servers.

ex:
```json
{
    "session_port": 1000,
	"backend_directory": "server_backend",
	"open_token": "etienned@o",
	"kiwi_version": "v1.0.0"
}
```

You can then launch the server by typing in a terminal.

```shell
$ ./Server -f ./config/dev.json
```

### Scripts<a name="scripts"></a>

##### Documentation

The documentation can be generated using [Doxygen](http://www.stack.nl/~dimitri/doxygen/index.html).
It is generated in directory /docs in html format.

```shell
# Execute doxygen from root directory.
$ Doxygen ./docs/Doxyfile
```
##### Ressources

You can generate binary data for ressources with the `Scripts/ressource.py` script.
This script takes 2 optional arguments:
 - `-i / --input` : The ressources input directory, Default is `./Ressources/BinaryRes`.
 - `-o / --output` : The output directory in which the binary data files will be generated,
 Default is `./Client/Source/KiwiApp_Ressources`.

 ex:

 ```shell
 $ cd {kiwi_directory_path}
 # with default paths:
 $ python ./Scripts/ressource.py
 # or with custom paths
 $ python ./Scripts/ressource.py -i input/ressources/path -o output/binary/path
 ```
##### Windows installer

An installer for Kiwi's software is generated for windows operating system. [Inno setup](http://www.jrsoftware.org/isinfo.php) is used to generated the installer .exe file.


To manually generate the windows installer for 32 and 64 bit, first make sure that you have compiled Kiwi Win32 and 64x binaries in mode Release. Install inno setup on your machine and locate the folder containing "ISCC.exe" and invoke it the command line specifying either [Kiwi-Directory]/Scripts/setup-Win32.iss to generate the 32 bit installeror /Scripts/setup-x64.iss to generate the 64 bit insaller.

ex:
```shell
$ "C:\Program Files (x86)\Inno Setup 5\ISCC.exe" Scripts\setup-x64.iss
```

The installer is generated in the respective build directories i.e [Kiwi-Directory]\Build\Release\[Platform]\KiwiBuild\[Config].
