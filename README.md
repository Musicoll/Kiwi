## Kiwi

[![Build Status](https://travis-ci.org/Musicoll/Kiwi.svg?branch=master)](https://travis-ci.org/Musicoll/Kiwi)  
[![Build status](https://ci.appveyor.com/api/projects/status/github/Musicoll/Kiwi?branch=master&svg=true)](https://ci.appveyor.com/project/CICM/kiwi)  
[![Coverage Status](https://coveralls.io/repos/github/Musicoll/Kiwi/badge.svg?branch=master)](https://coveralls.io/github/Musicoll/Kiwi?branch=master)  

[![Documentation](https://img.shields.io/badge/docs-doxygen-blue.svg)](http://musicoll.github.io/Kiwi/)  
[![Release](https://img.shields.io/github/release/Musicoll/Kiwi.svg)](https://github.com/Musicoll/Kiwi/releases)  
[![license](https://img.shields.io/github/license/Musicoll/Kiwi.svg?maxAge=2592000)](https://github.com/Musicoll/Kiwi/blob/master/LICENSE)  
[![Website](https://img.shields.io/website/http/shields.io.svg?maxAge=2592000)](http://musicoll.mshparisnord.org)


### Modules Initialization
```
git submodule update --init --recursive ThirdParty/flip-public
git submodule update --init --recursive ThirdParty/Juce
```

### Compilation
```
python ./Config/configure.py   
python ./Config/build.py -c Release
python ./Config/run_test.py -c Release (optional)
```

### Run the Application
* Launch the *server*
* Type *open* in the server console
* Launch the Kiwi Application
* Change the user id in Kiwi/Settings if needed
* Join the document
* Play...
* Type *close* in the server console
