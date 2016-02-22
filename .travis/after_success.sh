#!/bin/bash

COVERALLS_SUPPORT="OFF"
if [ $TRAVIS_OS_NAME == 'linux' ] && [ $COMPILER == 'g++-4.9' ]; then
  COVERALLS_SUPPORT="ON"
fi

if [ $COVERALLS_SUPPORT == "ON" ]; then

  COVERALLS_EXCLUDE_MODULES="-e build/CMakeFiles/feature_tests.cxx -e /build/CMakeFiles/3.2.2/CompilerIdCXX/CMakeCXXCompilerId.cpp -e ThirdParty -e Client -e test "
  if [ $TRAVIS_BRANCH == 'dev-dsp' ]; then
    COVERALLS_EXCLUDE_MODULES+="-e Modules/KiwiCore -e Modules/KiwiGraphics"
  elif [ $TRAVIS_BRANCH == 'dev-core' ]; then
    COVERALL_EXCLUDE_MODULES+="-e Modules/KiwiDsp -e Modules/KiwiGraphics"
  elif [ $TRAVIS_BRANCH == 'dev-graphics' ]; then
  COVERALL_EXCLUDE_MODULES+="-e Modules/KiwiCore -e Modules/KiwiDsp"
  fi
  coveralls $COVERALL_EXCLUDE_MODULES  --gcov-options '\-lp' --gcov 'gcov-4.9'
fi
