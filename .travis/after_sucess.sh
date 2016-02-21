#!/bin/bash

GCOV_VERSION="\'gcov-4.9\'"
GCOV_OPTION="\'\-lp\'"

COVERALL_SUPPORT="OFF"
if [ $TRAVIS_OS_NAME == 'linux' ] && [ $COMPILER == 'g++-4.9' ]; then
  COVERALL_SUPPORT="ON"
  COVERALL_EXCEPTCPP="\'/CMakeFiles/*.cpp\'"
  COVERALL_EXCEPTCXX="\'/CMakeFiles/*.cxx\'"
fi

COVERALL_DIRECTORY="/Modules/"
if [ $TRAVIS_BRANCH == 'dev-dsp' ]; then
  COVERALL_DIRECTORY="/Modules/KiwiDsp/"
elif [ $TRAVIS_BRANCH == 'dev-core' ]; then
  COVERALL_DIRECTORY="/Modules/KiwiCore/"
fi

if [ $COVERALL_SUPPORT == "ON" ]; then
  coveralls -i $COVERALL_DIRECTORY --gcov-options $GCOV_OPTION --gcov $GCOV_VERSION
fi
