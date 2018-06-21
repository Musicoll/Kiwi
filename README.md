<p align="center">
  <h1 align="center">
    <img src="http://musicoll.github.io/Kiwi/ressources/img/kiwi_icon.png" height=40px/> Kiwi
  </h1>
  <p align="center">
    A real-time collaborative audio patching software
  </p>
  <p align="center">
    <a href="https://travis-ci.org/Musicoll/Kiwi"><img src="https://travis-ci.org/Musicoll/Kiwi.svg?branch=master" alt="Travis"></a>
    <a href="https://ci.appveyor.com/project/CICM/kiwi/history"><img src="https://ci.appveyor.com/api/projects/status/github/Musicoll/Kiwi?branch=master&svg=true" alt="Appveyor CI"></a>
  </p>
  <p align="center">
    <a href="http://kiwi.mshparisnord.fr/"><img src="https://img.shields.io/badge/@-website-blue.svg" alt="Website"></a>
    <a href="http://musicoll.github.io/Kiwi"><img src="https://img.shields.io/badge/@-documentation-blue.svg" alt="Documentation"></a>
    <a href="https://github.com/Musicoll/Kiwi/releases"><img src="https://img.shields.io/badge/@-download-blue.svg" alt="Download"></a>
  </p>
</p>

### Presentation

Kiwi is part of the French ANR-funded [MUSICOLL](http://musicoll.mshparisnord.org/) project that unites the [CICM](http://cicm.mshparisnord.org/) and [OhmForce](https://www.ohmforce.com/Company.do). The main goal of this project is to study how collaboration can enhance the way digital audio music composition is taught and more generally how it can improve music creation workflow. It enables several creators to work simultaneously on a same patch hosted online and accessible from any connected terminal.

<img title="register" src="https://raw.github.com/Musicoll/Kiwi/master/docs/ressources/img/Kiwi-v1.0.0.jpg" height=400px/>

### Compilation

  1. Clone the project  
```
git clone https://github.com/Musicoll/Kiwi.git
cd Kiwi
git submodule update --init --recursive
```

  2. Install and build dependencies
    - MacOS
```
# BOOST
cd ThirdParty
curl -L https://sourceforge.net/projects/boost/files/boost/1.63.0/boost_1_63_0.tar.gz -o ./boost.tar.gz
tar zxf ./boost.tar.gz && mv ./boost_1_63_0 boost && cd boost
./bootstrap.sh toolset=clang macosx-version-min=10.9 link=static
./b2 address-model=64 --with-system stage
cd ../..
# LLVM
cd ThirdParty
curl http://releases.llvm.org/5.0.0/clang+llvm-5.0.0-x86_64-apple-darwin.tar.xz -o ./llvm.tar.xz
tar zxvf ./llvm.tar.xz && mv ./clang+llvm-5.0.0-x86_64-apple-darwin llvm
cd ../..
```
    - Linux
```
# BOOST
cd ThirdParty
curl -L https://sourceforge.net/projects/boost/files/boost/1.63.0/boost_1_63_0.tar.gz -o ./boost.tar.gz
tar zxf ./boost.tar.gz && mv ./boost_1_63_0 boost && cd boost
./bootstrap.sh toolset=gcc link=static
./b2 --with-system stage
cd ../..
# LLVM
cd ThirdParty
curl -o ./ThirdParty/llvm.tar.xz http://releases.llvm.org/5.0.0/clang+llvm-5.0.0-linux-x86_64-ubuntu14.04.tar.xz
tar xvf ./ThirdParty/llvm.tar.xz && mv ./clang+llvm-5.0.0-linux-x86_64-ubuntu14.04 ThirdParty/llvm
```

    - Windows
```
# BOOST
# LLVM
cd ThirdParty
curl -L https://github.com/pierreguillot/llvm-win/releases/download/v6.0.0/llvm-windows-x64-mt.zip -o llvm.zip
7z x llvm.zip
rename llvm-windows-x64-mt llvm
```

### Organizations

- [ANR MUSICOLL](http://musicoll.mshparisnord.org)
- [CICM](http://cicm.mshparisnord.org)
- [OhmForce](https://www.ohmforce.com/Company.do)
- [MSH Paris Nord](https://www.mshparisnord.fr)
- [Université Paris 8](https://www.univ-paris8.fr)

### Authors

- [Eliott PARIS](https://github.com/eliottparis)
- [Pierre GUILLOT](https://github.com/pierreguillot)
- [Jean MILLOT](https://github.com/jean-millot)

### Credits

- [Flip](http://developer.irisate.com/)
- [Juce](https://github.com/WeAreROLI/JUCE)
- [Beast](https://github.com/boostorg/Beast/)
- [concurrentqueue](https://github.com/cameron314/concurrentqueue)
- [json](https://github.com/nlohmann/json)
- [CMake](https://cmake.org/)
- [LLVM](http://llvm.org)
- [FAUST](https://github.com/grame-cncm/faust.git)
- [Icon by Freepik](https://www.flaticon.com/)

### Papers

- [JIM 2017 - Kiwi : Vers un environnement de creation musicale temps reel collaboratif](https://hal.archives-ouvertes.fr/hal-01550190/document)
- [JIM 2018 - Enseigner le patching de manière collective avec le logiciel collaboratif Kiwi](https://hal.archives-ouvertes.fr/hal-01791492/document).
- [Teaching, investigating, creating: MUSICOLL](https://hal.archives-ouvertes.fr/hal-01581698v1)


### License

The Kiwi application is licensed under the terms of the [GNU GPL-3.0](https://github.com/Musicoll/Kiwi/blob/master/Licence.md) license. However, Kiwi also depends on the [Flip](http://developer.irisate.com/) library which is NOT open-source.
