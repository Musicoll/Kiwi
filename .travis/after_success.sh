#!/bin/bash

COVERALL_SUPPORT="OFF"
if [ $TRAVIS_OS_NAME == 'linux' ] && [ $COMPILER == 'g++-4.9' ]; then
  COVERALL_SUPPORT="ON"
fi

if [ $COVERALL_SUPPORT == "ON" ]; then

  COVERALL_EXCLUDE_MODULES=""
  if [ $TRAVIS_BRANCH == 'dev-dsp' ]; then
    COVERALL_EXCLUDE_MODULES="-e Modules/KiwiCore -e Modules/KiwiGraphics"
  elif [ $TRAVIS_BRANCH == 'dev-core' ]; then
    COVERALL_EXCLUDE_MODULES="-e Modules/KiwiDsp -e Modules/KiwiGraphics"
  elif [ $TRAVIS_BRANCH == 'dev-graphics' ]; then
  COVERALL_EXCLUDE_MODULES="-e Modules/KiwiCore -e Modules/KiwiDsp"
  fi
  coveralls -E 'build/CMakeFiles/*.cxx' -E 'build/CMakeFiles/*.cpp' -e ThirdParty -e Client -e test $COVERALL_EXCLUDE_MODULES  --gcov-options '\-lp' --gcov 'gcov-4.9'
fi
