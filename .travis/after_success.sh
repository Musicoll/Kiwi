#!/bin/bash

COVERALL_SUPPORT="OFF"
if [ $TRAVIS_OS_NAME == 'linux' ] && [ $COMPILER == 'g++-4.9' ]; then
  COVERALL_SUPPORT="ON"
fi

if [ $COVERALL_SUPPORT == "ON" ]; then
  CALL_E_CMAKE="-E \'/CMakeFiles/*.cpp\' -E \'/CMakeFiles/*.cxx\'"
  CALL_E_REPO="-e Client -e test -e ThridParty"
  CALL_E_OPTION="--gcov-options \'gcov-4.9\' --gcov \'\-lp\'"

  if [ $TRAVIS_BRANCH == 'dev-dsp' ]; then
    coveralls $CALL_E_CMAKE $CALL_E_REPO -e Modules/KiwiCore -e Modules/KiwiGraphics $CALL_E_OPTION
  elif [ $TRAVIS_BRANCH == 'dev-core' ]; then
    coveralls $CALL_E_CMAKE $CALL_E_REPO -e Modules/KiwiDsp -e Modules/KiwiGraphics $CALL_E_OPTION
  elif [ $TRAVIS_BRANCH == 'dev-graphics' ]; then
    coveralls $CALL_E_CMAKE $CALL_E_REPO -e Modules/KiwiDsp -e Modules/KiwiCore $CALL_E_OPTION
  else
    coveralls $CALL_E_CMAKE $CALL_E_REPO $CALL_E_OPTION
  fi

fi
