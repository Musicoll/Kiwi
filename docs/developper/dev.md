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

* MacOs: clang 7.3.1
* Windows: msvc 19.0
* Linus: gcc-4.9

```shell
python ./Script/build.py
```

To build a specific target you can use argument -t and you can choose your configuration (Debug/Release) using -c argument.

ex:

```shell
python ./Script/build.py -t Server -c Debug
```

##### Tests

Tests are ran as a post build command of target Tests. To run run tests

```shell
python ./Srcipts/build.py -t Tests -c Release
```


### Code<a name="code"></a>

The documentation of Kiwi's classes can be found [here](html/index.html).

### Server<a name="server"></a>
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
