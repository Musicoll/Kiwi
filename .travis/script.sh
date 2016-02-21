#!/bin/bash

COVERALL_SUPPORT="OFF"
if [ $TRAVIS_OS_NAME == 'linux' ] && [ $COMPILER == 'gcc' ]; then
  $COVERALL_SUPPORT="ON"
fi

TARGET="test_core test_dsp"
if [ $TRAVIS_BRANCH == 'dev-dsp' ]; then
  $TARGET="test_dsp"
elif [ $TRAVIS_BRANCH == 'dev-core' ]; then
  $TARGET="test_core"
fi

echo "---------------------"
echo "OS=" $TRAVIS_OS_NAME
echo "BRANCH=" $TRAVIS_BRANCH
echo "COVERALL_SUPPORT=" $COVERALL_SUPPORT
echo "TARGET="$TARGET
echo "-------------------"

export CXX=$COMPILER
make clean
cmake -DCOVERALL=$COVERALL_SUPPORT ./
cmake --build ./ --target $TARGET
