#!/bin/bash

COVERALL_SUPPORT="OFF"
if [ $TRAVIS_OS_NAME == 'linux' ] && [ $COMPILER == 'g++-4.9' ]; then
  COVERALL_SUPPORT="ON"
fi

if [ $COVERALL_SUPPORT == "ON" ]; then

  if [ $TRAVIS_BRANCH == 'dev-dsp' ]; then
    coveralls -E '.*/build/CMakeFiles/*.cxx' -E '.*/build/CMakeFiles/*.cpp' -e ThirdParty -e Client -e test -e Modules/KiwiCore -e Modules/KiwiGraphics --gcov-options '\-lp' --gcov 'gcov-4.9'
  elif [ $TRAVIS_BRANCH == 'dev-core' ]; then
    coveralls -E '.*/build/CMakeFiles/*.cxx' '.*/build/CMakeFiles/*.cpp' -e ThirdParty -e Client -e test  -e Modules/KiwiDsp -e Modules/KiwiGraphics --gcov-options '\-lp' --gcov 'gcov-4.9'
  elif [ $TRAVIS_BRANCH == 'dev-graphics' ]; then
    coveralls -E '.*/build/CMakeFiles/*.cxx' '.*/build/CMakeFiles/*.cpp' -e ThirdParty -e Client -e test  -e Modules/KiwiDsp -e Modules/KiwiGraphics --gcov-options '\-lp' --gcov 'gcov-4.9'
  else
    coveralls -E '.*/build/CMakeFiles/*.cxx' '.*/build/CMakeFiles/*.cpp' -e ThirdParty -e Client -e test  --gcov-options '\-lp' --gcov 'gcov-4.9'
  fi

fi
