#!/bin/bash

COVERALL_SUPPORT="OFF"
if [ $TRAVIS_OS_NAME == 'linux' ] && [ $COMPILER == 'g++-4.9' ]; then
  COVERALL_SUPPORT="ON"
fi

COVERALL_DIRECTORY="Modules/"
if [ $TRAVIS_BRANCH == 'dev-dsp' ]; then
  COVERALL_DIRECTORY="Modules/KiwiDsp/"
elif [ $TRAVIS_BRANCH == 'dev-core' ]; then
  COVERALL_DIRECTORY="Modules/KiwiCore/"
fi

echo ""
echo "---------------------"
echo "COVERALL_SUPPORT=" $COVERALL_SUPPORT
echo "COVERALL_DIRECTORY="$COVERALL_DIRECTORY
echo "---------------------"
echo ""

if [ $COVERALL_SUPPORT == "ON" ]; then
  coveralls -i $COVERALL_DIRECTORY --gcov-options 'gcov-4.9' --gcov '\-lp'
fi
