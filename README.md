## Kiwi

[![Build Status](https://travis-ci.org/Musicoll/Kiwi.svg?branch=master)](https://travis-ci.org/Musicoll/Kiwi)
[![Build status](https://ci.appveyor.com/api/projects/status/github/Musicoll/Kiwi?branch=master&svg=true)](https://ci.appveyor.com/project/CICM/kiwi)
[![Coverage Status](https://coveralls.io/repos/github/Musicoll/Kiwi/badge.svg?branch=master)](https://coveralls.io/github/Musicoll/Kiwi?branch=master)

[![Documentation](https://img.shields.io/badge/docs-doxygen-blue.svg)](http://musicoll.github.io/Kiwi/)  
[![Release](https://img.shields.io/github/release/Musicoll/Kiwi.svg)](https://github.com/Musicoll/Kiwi/releases)  
[![license](https://img.shields.io/github/license/Musicoll/Kiwi.svg?maxAge=2592000)](https://github.com/Musicoll/Kiwi/blob/master/LICENSE)  
[![Website](https://img.shields.io/website/http/shields.io.svg?maxAge=2592000)](http://musicoll.mshparisnord.org)


### Modules Initialization
```shell
$ git submodule update --init --recursive ThirdParty/Juce ThirdParty/cpr
```

### Compilation

```shell
python ./Config/configure.py [-flip (public|private)] (default public)
python ./Config/build.py -c Release
python ./Config/run_test.py -c Release (optional)
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

#### (Re)generate binary data

You can generate binary data for ressources with the `Config/ressource.py` script.  
This script takes 2 required arguments:
 - `-i / --input` : The ressources input directory
 - `-o / --output` : The output directory in which the binary data files will be generated.

 ex:

 ```sh
 $ cd {kiwi_directory_path}
 $ python ./Config/ressource.py -i ./Ressources/BinaryRes -o ./Client/Source
 ```

### Credits:

 - Some of the application icons are *designed by Freepik and distributed by Flaticon*.
