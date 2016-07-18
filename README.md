## Kiwi

[![Build Status](https://travis-ci.org/Musicoll/Kiwi.svg?branch=master)](https://travis-ci.org/Musicoll/Kiwi)  
[![Build status](https://ci.appveyor.com/api/projects/status/github/Musicoll/Kiwi?branch=master&svg=true)](https://ci.appveyor.com/project/eliottparis/kiwi)  
[![Coverage Status](https://coveralls.io/repos/github/Musicoll/Kiwi/badge.svg?branch=master)](https://coveralls.io/github/Musicoll/Kiwi?branch=master)  
[![Documentation](https://img.shields.io/badge/docs-doxygen-blue.svg)](http://musicoll.github.io/Kiwi/)

**Modules Initialization**
```
git submodule update --init --recursive ThirdParty/flip-public
git submodule update --init --recursive ThirdParty/Juce
```

**Compilation**
```
python ./Config/configure.py   
python ./Config/build.py -c Release
python ./Config/run_test.py -c Release (optional)
```

**License**: GNU GENERAL PUBLIC LICENSE - see LICENSE   
