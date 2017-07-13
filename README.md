## Kiwi

Kiwi is a real-time collaborative audio patching software.  
It enables several creators to work simultaneously on a same patch hosted online and accessible from any connected terminal.

> Kiwi is part of the French ANR-funded [**MUSICOLL**](http://musicoll.mshparisnord.org/) project that unites the [CICM](http://cicm.mshparisnord.org/) and [OhmForce](https://www.ohmforce.com/Company.do). The main goal of this project is to study how collaboration can enhance the way digital audio music composition is taught and more generally how it can improve music creation workflow.

![Kiwi App](https://raw.github.com/Musicoll/Kiwi/master/docs/Ressources/Kiwi-v0.1.0.png "Kiwi v0.1.0")

[![Build Status](https://travis-ci.org/Musicoll/Kiwi.svg?branch=master)](https://travis-ci.org/Musicoll/Kiwi)
[![Build status](https://ci.appveyor.com/api/projects/status/github/Musicoll/Kiwi?branch=master&svg=true)](https://ci.appveyor.com/project/CICM/kiwi)
[![Coverage Status](https://coveralls.io/repos/github/Musicoll/Kiwi/badge.svg?branch=master)](https://coveralls.io/github/Musicoll/Kiwi?branch=master) [![Documentation](https://img.shields.io/badge/docs-doxygen-blue.svg)](http://musicoll.github.io/Kiwi/html)  [![Release](https://img.shields.io/github/release/Musicoll/Kiwi.svg)](https://github.com/Musicoll/Kiwi/releases)  [![license](https://img.shields.io/github/license/Musicoll/Kiwi.svg?maxAge=2592000)](https://github.com/Musicoll/Kiwi/blob/master/Licence.md)  [![Website](https://img.shields.io/website/http/shields.io.svg?maxAge=2592000)](http://musicoll.mshparisnord.org)

> Warning: Kiwi is in a heavy development stage and everything is subject to change without notice.

You can take a look at the version changelog [here](https://github.com/Musicoll/Kiwi/wiki/Changelog).

### Download

If you want to test Kiwi, you can download a version on the [Release Page](https://github.com/Musicoll/Kiwi/releases).

### Build from sources

You can also build Kiwi from sources.

Kiwi scripts and build system uses python (v2.7 or higher) and cmake (v2.8.7 or higher). To build Kiwi from source:

- clone this repository:

```shell
$ git clone --recursive https://github.com/Musicoll/Kiwi.git
```

- Read these instructions to install dependencies for [Mac](https://github.com/Musicoll/Kiwi/wiki/Installation-%28Mac%29), [Windows](https://github.com/Musicoll/Kiwi/wiki/Installation-%28Windows%29) or [Linux](https://github.com/Musicoll/Kiwi/wiki/Installation-%28Linux%29)
- You should then be able to [compile Kiwi]().

### Objects

A list of the kiwi objects can be found [here](https://github.com/Musicoll/Kiwi/wiki/List-of-Objects).

### Roadmap

- [ ] Authentication System.
- [ ] Private space.
- [ ] GUI Objects.
- [ ] Sub-Patchers.
- [ ] Add more objects.

---

### Authors

[![Eliott Paris](https://avatars.githubusercontent.com/u/1750257?s=90)](https://github.com/eliottparis) | [![Pierre Guillot](https://avatars.githubusercontent.com/u/1409918?s=90)](https://github.com/pierreguillot) | [![Jean Millot](https://avatars.githubusercontent.com/u/16612690?s=90)](https://github.com/jean-millot)
---|---|---
[Eliott Paris](https://github.com/eliottparis) | [Pierre Guillot](https://github.com/pierreguillot) | [Jean Millot](https://github.com/jean-millot)

---

### Papers

- [JIM 2017](https://jim2017.sciencesconf.org/data/Eliott_Paris2017aa.pdf) (fr).

---

### Credits

Kiwi uses the following ThirdParty libraries:

- [Flip](http://developer.irisate.com/)
- [Juce](https://github.com/WeAreROLI/JUCE)
- [Beast](https://github.com/vinniefalco/Beast/)
- [concurrentqueue](https://github.com/cameron314/concurrentqueue)
- [json](https://github.com/nlohmann/json)

> Most of the application icons are *designed by Freepik and distributed by Flaticon*.

---

### License

> You can check out the full license [here](https://github.com/Musicoll/Kiwi/blob/master/Licence.md)

The Kiwi application is licensed under the terms of the **GPLv3** license. However, Kiwi also depends on the **Flip** library (developed by [irisate](https://irisate.com/)) which is NOT open-source. You will need to download a public version on their [website](http://developer.irisate.com/) to compile.
