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

```
git clone https://github.com/Musicoll/Kiwi.git
cd Kiwi
git submodule update --init --recursive
cd Kiwi
mkdir Build && cd Build
cmake ..
cmake --build .
```

Using the system/personal dependencies

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
